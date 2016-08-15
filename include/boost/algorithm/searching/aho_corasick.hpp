/*
  Copyright (c) Alexander Zaitsev <zamazan4ik@gmail.com>, 2016
  Distributed under the Boost Software License, Version 1.0. (See
  accompanying file LICENSE_1_0.txt or copy at
  http://www.boost.org/LICENSE_1_0.txt)
  See http://www.boost.org/ for latest version.
*/

#ifndef AHO_CORASIK_AHO_CORASIK_HPP
#define AHO_CORASIK_AHO_CORASIK_HPP

#include <vector>
#include <queue>
#include <map>
#include <unordered_map>
#include <functional>
#include <memory>

#include <boost/range/begin.hpp>
#include <boost/range/end.hpp>


namespace boost { namespace algorithm {

template <typename T, template<typename ...> typename Container, typename ...Args>
class AhoCorasick
{
private:
    class MapBorNode
    {
    public:
        Container<T, std::shared_ptr<MapBorNode>, Args...> links;
        std::shared_ptr<MapBorNode> fail, term;
        std::vector<size_t> pat;

        MapBorNode(std::shared_ptr<MapBorNode> fail_node = nullptr)
                : fail(fail_node), term(nullptr)
        { }

        std::shared_ptr<MapBorNode> getLink(const T& c) const
        {
            const auto iter = links.find(c);
            return iter != links.cend() ? iter->second : nullptr;
        }

        bool isTerminal() const
        {
            return !pat.empty();
        }
    };
public:
    using value_type = T;
    using node_type = MapBorNode;
private:
    std::shared_ptr<node_type> root, current_state;
    size_t countStrings = 0;
public:
    AhoCorasick() : root(std::make_shared<node_type>()) {}

    template<typename ForwardIterator>
    AhoCorasick(ForwardIterator patBegin, ForwardIterator patEnd) : root(std::make_shared<node_type>())
    {
        while(patBegin != patEnd)
        {
            insert(*patBegin);
            ++patBegin;
        }
    }

    /// \fn insert(const Range& range)
    /// \brief Insert pattern in trie
    ///
    /// \param range The pattern range
    ///
    template<typename Range>
    void insert(const Range& range)
    {
        insert(boost::begin(range), boost::end(range));
    }

    /// \fn insert(ForwardIterator begin, ForwardIterator end)
    /// \brief Insert pattern in trie
    ///
    /// \param begin The start of the pattern
    /// \param end   One past the end of the pattern
    ///
    template<typename ForwardIterator>
    void insert(ForwardIterator begin, ForwardIterator end)
    {
        size_t patLen = 0;
        std::shared_ptr<node_type> current_node = root;
        for(auto it = begin; it != end; ++it)
        {
            ++patLen;
            std::shared_ptr<node_type> child_node = current_node->getLink(*it);
            if (!child_node)
            {
                child_node = std::make_shared<node_type>(root);
                current_node->links[*it] = child_node;
            }
            current_node = child_node;
        }
        current_node->pat.push_back(patLen);
    }

    /// \fn operator ( RAIterator begin, RAIterator end, Out& cont)
    /// \brief Searches patterns in the corpus
    ///
    /// \param begin The start of the data to search (Random Access Iterator)
    /// \param end   One past the end of the data to search (Random Access Iterator)
    /// \param cont  Output container of pairs of iterators to corpus sequence
    ///
    template <typename RAIterator, typename Out>
    void operator()(RAIterator begin, RAIterator end, Out& cont)
    {
        init();
        current_state = root;
        for(auto it = begin; it != end; ++it)
        {
            step(*it);
            getTermsForCurrentState(it, cont);
        }
    }
private:
    void init()
    {
        std::queue<std::shared_ptr<node_type>> q;
        q.push(root);
        while (!q.empty())
        {
            std::shared_ptr<node_type> current_node = q.front();
            q.pop();
            for (auto iter = current_node->links.cbegin();
                 iter != current_node->links.cend(); ++iter)
            {
                const value_type& symbol = iter->first;
                std::shared_ptr<node_type> child = iter->second;

                // Defining .fail for the childnode
                std::shared_ptr<node_type> temp_node = current_node->fail;
                while (temp_node)
                {
                    std::shared_ptr<node_type> fail_candidate = temp_node->getLink(symbol);
                    if (fail_candidate)
                    {
                        child->fail = fail_candidate;
                        break;
                    }
                    temp_node = temp_node->fail;
                }

                // Defining .term for the childnode using .term of current node
                child->term = (child->fail == nullptr || child->fail->isTerminal()) ? child->fail : child->fail->term;
                q.push(child);
            }
        }
    }

    void step(const value_type& c)
    {
        while (current_state)
        {
            std::shared_ptr<node_type> candidate = current_state->getLink(c);
            if (candidate)
            {
                current_state = candidate;
                return;
            }
            current_state = current_state->fail;
        }
        current_state = root;
    }

    template <typename RAIterator, typename Out>
    void getTermsForCurrentState(RAIterator pos, Out& cont)
    {
        if (current_state->isTerminal())
        {
            for (const auto value : current_state->pat)
            {
                cont.push_back({1 + pos - value, pos + 1});
            }
        }
        std::shared_ptr<node_type> temp_node = current_state->term;
        while (temp_node)
        {
            for (const auto value : temp_node->pat)
            {
                cont.push_back({1 + pos - value, pos + 1});
            }
            temp_node = temp_node->term;
        }
    }
};

//Object interface
template <typename T, typename Pred = std::less<T>>
using Aho_Corasick_Map = AhoCorasick<T, std::map, Pred>;

template <typename T, typename Hash = std::hash<T>, typename Comp = std::equal_to<T>>
using Aho_Corasick_HashMap = AhoCorasick<T, std::unordered_map, Hash, Comp>;


//Functional interface

/// \fn aho_corasick_map ( RAIterator corpus_begin, RAIterator corpus_end,
///                        ForwardIterator pat_begin, ForwardIterator pat_end,
///                        ResultCont &out)
///
/// \param corpus_begin The start of the corpus sequence
/// \param corpus_end   One past the end of the corpus sequence
/// \param pat_begin	The start of the patterns sequence
/// \param pat_end	One past the end of the patterns sequence
/// \param out 		Container for results
///
template <typename T, typename Predicate = std::less<T>, typename RAIterator,
        typename ForwardIterator, typename ResultCont>
void aho_corasick_map ( RAIterator corpus_begin, RAIterator corpus_end,
                        ForwardIterator pat_begin, ForwardIterator pat_end,
                        ResultCont &out)
{
    AhoCorasick<T, std::map, Predicate> obj(pat_begin, pat_end);
    obj(corpus_begin, corpus_end, out);
}

/// \fn aho_corasick_hashmap ( RAIterator corpus_begin, RAIterator corpus_end,
///                            ForwardIterator pat_begin, ForwardIterator pat_end,
///                            ResultCont &out)
///
/// \param corpus_begin The start of the corpus sequence
/// \param corpus_end   One past the end of the corpus sequence
/// \param pat_begin	The start of the patterns sequence
/// \param pat_end	One past the end of the patterns sequence
/// \param out 		Container for results
///
template <typename T, typename Hash = std::hash<T>, typename Comp = std::equal_to<T>, typename RAIterator,
        typename ForwardIterator, typename ResultCont>
void aho_corasick_hashmap ( RAIterator corpus_first, RAIterator corpus_last,
                            ForwardIterator pat_first, ForwardIterator pat_last,
                            ResultCont &out)
{
    AhoCorasick<T, std::unordered_map, Hash, Comp> obj(pat_first, pat_last);
    obj(corpus_first, corpus_last, out);
}
}}

#endif //AHO_CORASIK_AHO_CORASIK_HPP

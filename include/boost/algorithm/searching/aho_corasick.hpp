/*
  Copyright (c) Alexander Zaitsev <zamazan4ik@gmail.com>, 2016
  Distributed under the Boost Software License, Version 1.0. (See
  accompanying file LICENSE_1_0.txt or copy at
  http://www.boost.org/LICENSE_1_0.txt)
  See http://www.boost.org/ for latest version.
*/

#ifndef BOOST_ALGORITHM_AHO_CORASICK_HPP
#define BOOST_ALGORITHM_AHO_CORASICK_HPP

#include <vector>
#include <queue>
#include <map>
#include <unordered_map>
#include <functional>
#include <memory>

#include <boost/range/begin.hpp>
#include <boost/range/end.hpp>
#include <boost/make_unique.hpp>

namespace boost { namespace algorithm {

template <typename T, template<typename ...> class Container, typename ...Args>
class AhoCorasick
{
private:
    class MapBorNode
    {
    public:
        Container<T, std::unique_ptr<MapBorNode>, Args...> links;
        MapBorNode *fail, *term;
        std::vector<size_t> pat;

        MapBorNode(MapBorNode* fail_node = nullptr)
                : fail(fail_node), term(nullptr)
        { }

        MapBorNode* getLink(const T& c) const
        {
            const auto iter = links.find(c);
            return iter != links.cend() ? iter->second.get() : nullptr;
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
    std::unique_ptr<node_type> root;
    node_type* current_state;
    size_t countStrings = 0;
public:
    AhoCorasick() : root(boost::make_unique<node_type>()) {}

    template<typename ForwardIterator>
    explicit AhoCorasick(ForwardIterator patBegin, ForwardIterator patEnd) : root(boost::make_unique<node_type>())
    {
        while(patBegin != patEnd)
        {
            insert(*patBegin);
            ++patBegin;
        }
    }

    template<typename Range>
    explicit AhoCorasick(const Range& range) : AhoCorasick(boost::begin(range), boost::end(range)) {}

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
        node_type* current_node = root.get();
        for(auto it = begin; it != end; ++it)
        {
            ++patLen;
            node_type* child_node = current_node->getLink(*it);
            if (!child_node)
            {
                std::unique_ptr<node_type> new_node = boost::make_unique<node_type>(root.get());
                child_node = new_node.get();
                current_node->links[*it] = std::move(new_node);
            }
            current_node = child_node;
        }
        current_node->pat.push_back(patLen);
    }

    /// \fn find ( RAIterator begin, RAIterator end, Callback cb)
    /// \brief Searches patterns in the corpus
    /// \return true if all callback callings return true, else false
    ///
    /// \param begin The start of the data to search (Random Access Iterator)
    /// \param end   One past the end of the data to search (Random Access Iterator)
    /// \param cb    Callback for matches
    ///
    template <typename RAIterator, typename Callback>
    bool find(RAIterator begin, RAIterator end, Callback cb)
    {
        init();
        current_state = root.get();
        for(auto it = begin; it != end; ++it)
        {
            step(*it);
            if(!getTermsForCurrentState(it, cb))
	    {
		return false;
	    }
        }
        return true;
    }
private:
    void init()
    {
        std::queue<node_type*> q;
        q.push(root.get());
        while (!q.empty())
        {
            node_type* current_node = q.front();
            q.pop();
            for (auto iter = current_node->links.cbegin();
                 iter != current_node->links.cend(); ++iter)
            {
                const value_type& symbol = iter->first;
                node_type* child = iter->second.get();

                // Defining .fail for the childnode
                node_type* temp_node = current_node->fail;
                while (temp_node)
                {
                    node_type* fail_candidate = temp_node->getLink(symbol);
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
            node_type* candidate = current_state->getLink(c);
            if (candidate)
            {
                current_state = candidate;
                return;
            }
            current_state = current_state->fail;
        }
        current_state = root.get();
    }

    template <typename RAIterator, typename Callback>
    bool getTermsForCurrentState(RAIterator pos, Callback cb)
    {
        if (current_state->isTerminal())
        {
            for (const auto value : current_state->pat)
            {
                if(!cb(1 + pos - value, pos + 1))
		{
		    return false;
		}
            }
        }
        node_type* temp_node = current_state->term;
        while (temp_node)
        {
            for (const auto value : temp_node->pat)
            {
                if(!cb(1 + pos - value, pos + 1))
		{
		    return false;
		}
            }
            temp_node = temp_node->term;
        }
        return true;
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
///                        Callback cb)
/// \return true if all callback callings return true, else false
///
/// \param corpus_begin The start of the corpus sequence
/// \param corpus_end   One past the end of the corpus sequence
/// \param pat_begin	The start of the patterns sequence
/// \param pat_end	    One past the end of the patterns sequence
/// \param cb 		    Callback for matches
///
template <typename T, typename Predicate = std::less<T>, typename RAIterator,
        typename ForwardIterator, typename Callback>
bool aho_corasick_map ( RAIterator corpus_begin, RAIterator corpus_end,
                        ForwardIterator pat_begin, ForwardIterator pat_end,
                        Callback cb)
{
    AhoCorasick<T, std::map, Predicate> obj(pat_begin, pat_end);
    return obj.find(corpus_begin, corpus_end, cb);
}

/// \fn aho_corasick_hashmap ( RAIterator corpus_begin, RAIterator corpus_end,
///                            ForwardIterator pat_begin, ForwardIterator pat_end,
///                            Callback cb)
/// \return true if all callback callings return true, else false
///
/// \param corpus_begin The start of the corpus sequence
/// \param corpus_end   One past the end of the corpus sequence
/// \param pat_begin	The start of the patterns sequence
/// \param pat_end	    One past the end of the patterns sequence
/// \param cb 		    Callback for matches
///
template <typename T, typename Hash = std::hash<T>, typename Comp = std::equal_to<T>, typename RAIterator,
        typename ForwardIterator, typename Callback>
bool aho_corasick_hashmap ( RAIterator corpus_begin, RAIterator corpus_end,
                            ForwardIterator pat_begin, ForwardIterator pat_end,
                            Callback cb)
{
    AhoCorasick<T, std::unordered_map, Hash, Comp> obj(pat_begin, pat_end);
    return obj.find(corpus_begin, corpus_end, cb);
}
}}

#endif //BOOST_ALGORITHM_AHO_CORASICK_HPP

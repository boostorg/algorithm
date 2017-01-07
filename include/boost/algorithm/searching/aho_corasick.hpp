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
#include <functional>

#include <boost/range/begin.hpp>
#include <boost/range/end.hpp>
#include <boost/container/flat_map.hpp>


namespace boost { namespace algorithm {

template <typename T, template<typename ...> class Container, typename ...Args>
class aho_corasick_base
{
private:
    class node
    {
    public:
        Container<T, node, Args...> links;
        node *fail, *term;
        std::vector<size_t> pat;

        node(node* fail_node = nullptr)
                : fail(fail_node), term(nullptr)
        { }

        node* getLink(const T& c)
        {
            auto iter = links.find(c);
            return iter != links.end() ? &iter->second : nullptr;
        }

        bool isTerminal() const
        {
            return !pat.empty();
        }
    };
public:
    using value_type = T;
    using node_type = node;
private:
    node_type root;
    node_type* current_state;
    bool isInited = false;
public:
    aho_corasick_base(){}

    template<typename ForwardIterator>
    explicit aho_corasick_base(ForwardIterator patBegin, ForwardIterator patEnd)
    {
        while(patBegin != patEnd)
        {
            insert(*patBegin);
            ++patBegin;
        }
    }

    template<typename Range>
    explicit aho_corasick_base(const Range& range) : aho_corasick_base(boost::begin(range), boost::end(range)) {}

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
        isInited = false;
        size_t patLen = 0;
        node_type* current_node = &root;
        for(auto it = begin; it != end; ++it)
        {
            ++patLen;
            node_type* child_node = current_node->getLink(*it);
            if (!child_node)
            {
                current_node->links[*it] = node(&root);
                child_node = &current_node->links[*it];
            }
            current_node = child_node;
        }
        current_node->pat.push_back(patLen);
    }

    /// \fn find ( const Range& range, Callback cb)
    /// \brief Searches patterns in the corpus
    /// \return true if all callback callings return true, else false
    ///
    /// \param range The range of the data to search
    /// \param cb    Callback for matches
    ///
    template <typename Range, typename Callback>
    bool find(const Range& range, Callback cb)
    {
        return find(boost::begin(range), boost::end(range), cb);
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
        if(!isInited)
        {
            init();
        }
        current_state = &root;
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
        q.push(&root);
        while (!q.empty())
        {
            node_type* current_node = q.front();
            q.pop();
            for (auto iter = current_node->links.begin();
                 iter != current_node->links.end(); ++iter)
            {
                const value_type& symbol = iter->first;
                node_type* child = &iter->second;

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
        isInited = true;
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
        current_state = &root;
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
using aho_corasick = aho_corasick_base<T, boost::container::flat_map, Pred>;


//Functional interface

/// \fn aho_corasick_search ( RAIterator corpus_begin, RAIterator corpus_end,
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
bool aho_corasick_search ( RAIterator corpus_begin, RAIterator corpus_end,
                        ForwardIterator pat_begin, ForwardIterator pat_end,
                        Callback cb)
{
    aho_corasick_base<T, boost::container::flat_map, Predicate> obj(pat_begin, pat_end);
    return obj.find(corpus_begin, corpus_end, cb);
}

}}

#endif //BOOST_ALGORITHM_AHO_CORASICK_HPP

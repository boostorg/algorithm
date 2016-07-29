//
// Created by zamazan4ik on 17.07.16.
//

#ifndef AHO_CORASIK_AHO_CORASIK_HPP
#define AHO_CORASIK_AHO_CORASIK_HPP

#include <vector>
#include <functional>
#include <string>
#include <iterator>
#include <utility>
#include <memory>

#include <map>
#include <unordered_map>

#include <boost/range/begin.hpp>
#include <boost/range/end.hpp>

namespace boost { namespace algorithm
{

template<typename T, typename Map, typename ResultCont = std::vector<std::vector<T>>>
class Base_Aho_Corasik
{
public:
    Base_Aho_Corasik()
    {
        tree.emplace_back(Node(0, 0, 0));
    }

    ~Base_Aho_Corasik()
    {

    }

    template<typename R>
    void insert(const R &range)
    {
        insert(boost::begin(range), boost::end(range));
    }

    template<typename ForwardIterator>
    void insert(ForwardIterator begin, ForwardIterator end)
    {
        used.push_back(false);

        buffer.emplace_back(std::vector<T>(std::distance(begin, end)));
        size_t v = 0;
        for (auto it = begin; it != end; ++it)
        {
            buffer.back().push_back(*it);
            if (tree[v].next_.find(*it) == tree[v].next_.end())
            {
                Node node(Node::Sentinel, Node::Sentinel, v);
                node.prevValue_ = *it;
                tree.push_back(node);
                tree[v].next_[*it] = sz++;
            }
            v = tree[v].next_[*it];
        }
        tree[v].isLeaf_ = true;
        tree[v].pat_.push_back(countStrings++);
    }

    //Search
    template<typename R>
    ResultCont& find(const R &range)
    {
        return find(boost::begin(range), boost::end(range));
    }

    template<typename ForwardIterator>
    ResultCont& find(ForwardIterator begin, ForwardIterator end)
    {
        result.clear();
        size_t v = 0;
        std::fill(used.begin(), used.end(), false);

        while (begin != end)
        {
            v = go(v, *begin);
            check(v);
            ++begin;
        }

        for (size_t i = 0; i < used.size(); ++i)
        {
            if (used[i])
            {
                result.push_back(buffer[i]);
            }
        }
        return result;
    }

private:
    size_t getlink(const size_t v)
    {
        if (tree[v].suffLink_ == Node::Sentinel)
        {
            if (v == 0 || tree[v].prevNode_ == 0)
            {
                tree[v].suffLink_ = 0;
            }
            else
            {
                tree[v].suffLink_ = go(getlink(tree[v].prevNode_), tree[v].prevValue_);
            }
        }
        return tree[v].suffLink_;
    }

    size_t go(const size_t v, const T c)
    {
        if (tree[v].go_.find(c) == tree[v].go_.end())
        {
            if (tree[v].next_.find(c) != tree[v].next_.end())
            {
                tree[v].go_[c] = tree[v].next_[c];
            }
            else
            {
                if (v == 0)
                {
                    tree[v].go_[c] = 0;
                }
                else
                {
                    tree[v].go_[c] = go(getlink(v), c);
                }
            }
        }
        return tree[v].go_[c];
    }

    size_t getgood(const size_t v)
    {
        if (tree[v].goodSuffLink_ == Node::Sentinel)
        {
            size_t u = getlink(v);
            if (u == 0)
            {
                tree[v].goodSuffLink_ = 0;
            }
            else
            {
                tree[v].goodSuffLink_ = tree[u].isLeaf_ ? u : getgood(u);
            }
        }
        return tree[v].goodSuffLink_;
    }

    void check(const size_t v)
    {
        for (size_t i = v; i != 0; i = getgood(i))
        {
            if (tree[i].isLeaf_)
            {
                for (const auto& x: tree[i].pat_)
                {
                    used[x] = true;
                }
            }
        }
    }

private:
    struct Node
    {
        static const size_t Sentinel = static_cast<size_t>(-1);

        size_t suffLink_, goodSuffLink_, prevNode_;
        bool isLeaf_;
        T prevValue_;
        std::vector<size_t> pat_;
        Map next_, go_;
        Node(const size_t suffLink = Sentinel, const size_t goodSuffLink = Sentinel,
             const size_t prevNode = 0, const bool isLeaf = false) : suffLink_(suffLink), goodSuffLink_(goodSuffLink),
                                                                     prevNode_(prevNode), isLeaf_(isLeaf)
        {
        }
    };
    std::vector<Node> tree;
    std::vector<std::vector<T>> buffer;
    size_t sz = 1, countStrings = 0;
    std::vector<char> used;
    ResultCont result;
};

template<typename T, typename Compare = std::less<T>, typename Alloc = std::allocator<std::pair<const T, size_t>>,
         typename ResultCont = std::vector<std::vector<T>>>
using Aho_Corasik = Base_Aho_Corasik<T, std::map<T, size_t, Compare, Alloc>, ResultCont>;

template<typename T, typename Hash = std::hash<T>, typename Pred = std::equal_to<T>,
         typename Alloc = std::allocator<std::pair<const T, size_t>>,
         typename ResultCont = std::vector<std::vector<T>>>
using Aho_Corasik_Hash = Base_Aho_Corasik<T, std::unordered_map<T, size_t, Hash, Pred, Alloc>, ResultCont>;
}}
#endif //AHO_CORASIK_AHO_CORASIK_HPP

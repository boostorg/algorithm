#include <vector>
#include <queue>
#include <map>
#include <unordered_map>
#include <functional>

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

    template<typename R>
    void insert(const R& range)
    {
        insert(boost::begin(range), boost::end(range));
    }

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

    template <typename ForwardIterator, typename Out>
    void getTermsForCurrentState(Out& cont, ForwardIterator pos)
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

    //Find methods
    template <typename ForwardIterator, typename Out>
    void find(ForwardIterator begin, ForwardIterator end, Out& cont)
    {
        init();
        current_state = root;
        for(auto it = begin; it != end; ++it)
        {
            step(*it);
            getTermsForCurrentState(cont, it);
        }
    }

    template <typename Range, typename Out>
    void find(const Range& range, Out& cont)
    {
        return find(boost::begin(range), boost::end(range), cont);
    }

    template <typename Range, typename Out>
    void operator()(const Range& range, Out& cont)
    {
        return find(range, cont);
    }

    template <typename ForwardIterator, typename Out>
    void operator()(ForwardIterator begin, ForwardIterator end, Out& cont)
    {
        return find(begin, end, cont);
    }
};

//Object interface
template <typename T, typename Pred = std::less<T>>
using Aho_Corasick_Map = AhoCorasick<T, std::map, Pred>;

template <typename T, typename Hash = std::hash<T>, typename Comp = std::equal_to<T>>
using Aho_Corasick_HashMap = AhoCorasick<T, std::unordered_map, Hash, Comp>;


//Functional interface
template <typename T, typename Predicate = std::less<T>, typename ForwardIterator1,
          typename ForwardIterator2, typename ResultCont>
void aho_corasick_map ( ForwardIterator1 corpus_first, ForwardIterator1 corpus_last,
                        ForwardIterator2 pat_first, ForwardIterator2 pat_last,
                        ResultCont &out)
{
    AhoCorasick<T, std::map, Predicate> obj(pat_first, pat_last);
    obj.find(corpus_first, corpus_last, out);
}

template <typename T, typename Predicate = std::less<T>, typename Range1,
        typename Range2, typename ResultCont>
void aho_corasick_map ( Range1 corpus_range, Range2 pat_range, ResultCont &out)
{
    AhoCorasick<T, std::map, Predicate> obj(boost::begin(pat_range), boost::end(pat_range));
    obj.find(boost::begin(corpus_range), boost::end(corpus_range), out);
}

template <typename T, typename Hash = std::hash<T>, typename Comp = std::equal_to<T>, typename ForwardIterator1,
          typename ForwardIterator2, typename ResultCont>
void aho_corasick_hashmap ( ForwardIterator1 corpus_first, ForwardIterator1 corpus_last,
                        ForwardIterator2 pat_first, ForwardIterator2 pat_last,
                        ResultCont &out)
{
    AhoCorasick<T, std::unordered_map, Hash, Comp> obj(pat_first, pat_last);
    obj.find(corpus_first, corpus_last, out);
}

template <typename T, typename Hash = std::hash<T>, typename Comp = std::equal_to<T>, typename Range1,
        typename Range2, typename ResultCont>
void aho_corasick_hashmap ( Range1 corpus_range, Range2 pat_range, ResultCont &out)
{
    AhoCorasick<T, std::unordered_map, Hash, Comp> obj(boost::begin(pat_range), boost::end(pat_range));
    obj.find(boost::begin(corpus_range), boost::end(corpus_range), out);
}

}}



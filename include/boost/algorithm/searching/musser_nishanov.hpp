#ifndef BOOST_ALGORITHM_SEARCH_MUSSER_NISHANOV_HPP
#define BOOST_ALGORITHM_SEARCH_MUSSER_NISHANOV_HPP

#include <boost/algorithm/searching/detail/mn_traits.hpp>

#include <boost/array.hpp>

#include <iterator>
#include <algorithm>
#include <vector>

template <class RandomAccessIterator, class Distance>
void compute_next(RandomAccessIterator pattern,
                  RandomAccessIterator patternEnd,
                  std::vector<Distance> &next)
{
    Distance const pattern_size = patternEnd - pattern;
    Distance j = 0, t = -1;
    next.reserve(pattern_size);
    next.push_back(-1);
    while (j < pattern_size - 1)
    {
        while (t >= 0 && pattern[j] != pattern[t])
            t = next[t];
        ++j;
        ++t;
        next.push_back(pattern[j] == pattern[t] ? next[t] : t);
    }
}



template <class ForwardIterator, class Distance>
void compute_next(ForwardIterator pattern,
                  ForwardIterator patternEnd,
                  std::vector<Distance> &next,
                  std::vector<ForwardIterator> &pattern_iterator)
{
    Distance t = -1;
    next.reserve(32);
    pattern_iterator.reserve(32);
    next.push_back(-1);
    pattern_iterator.push_back(pattern);
    ForwardIterator advance = pattern;
    ++advance;
    for (; advance != patternEnd; ++advance)
    {
        while (t >= 0 && *pattern != *pattern_iterator[t])
            t = next[t];
        ++pattern;
        ++t;
        if (*pattern == *pattern_iterator[t])
            next.push_back(next[t]);
        else
            next.push_back(t);
        pattern_iterator.push_back(pattern);
    }
}


template <class ForwardIterator1, class ForwardIterator2>
inline ForwardIterator1 __search(ForwardIterator1 text,
                                 ForwardIterator1 textEnd,
                                 ForwardIterator2 pattern,
                                 ForwardIterator2 patternEnd,
                                 std::forward_iterator_tag)
{
    return __search_L(text, textEnd, pattern, patternEnd);
}

template <class ForwardIterator1, class ForwardIterator2>
ForwardIterator1 __search_L(ForwardIterator1 text,
                            ForwardIterator1 textEnd,
                            ForwardIterator2 pattern,
                            ForwardIterator2 patternEnd)
{
    using namespace std;
    typedef typename std::iterator_traits<ForwardIterator2>::difference_type Distance2;
    ForwardIterator1 advance_, hold;
    ForwardIterator2 p, p1;
    Distance2 j, m;
    vector<Distance2> next;
    vector<ForwardIterator2> pattern_iterator;
    
    compute_next(pattern, patternEnd, next, pattern_iterator);
    m = next.size();
    if (next.size() == 1)
        return find(text, textEnd, *pattern);
    
    p1 = pattern;
    ++p1;
    while (text != textEnd)
    {
        text = find(text, textEnd, *pattern);
        if (text == textEnd)
            return textEnd;
        p = p1;
        j = 1;
        hold = text;
        if (++text == textEnd)
            return textEnd;
        while (*text == *p)
        {
            if (++p == patternEnd)
                return hold;
            if (++text == textEnd)
                return textEnd;
            ++j;
        }
        
        for (;;)
        {
            j = next[j];
            if (j < 0)
            {
                ++text;
                break;
            }
            if (j == 0)
                break;
            p = pattern_iterator[j];
            while (*text == *p)
            {
                ++text;
                ++p;
                ++j;
                if (p == patternEnd)
                {
                    advance_ = hold;
                    advance(advance_, m);
                    while (advance_ != text)
                        ++advance_, ++hold;
                    return hold;
                }
                if (text == textEnd)
                    return textEnd;
            }
        }
    }
    return textEnd;
}


template <class BidirectionalIterator1, class BidirectionalIterator2>
inline BidirectionalIterator1 __search(BidirectionalIterator1 text,
                                       BidirectionalIterator1 textEnd,
                                       BidirectionalIterator2 pattern,
                                       BidirectionalIterator2 patternEnd,
                                       std::bidirectional_iterator_tag)
{
    return __search_L(text, textEnd, pattern, patternEnd);
}



template <class RandomAccessIterator1, class RandomAccessIterator2>
inline RandomAccessIterator1 __search(RandomAccessIterator1 text,
                                      RandomAccessIterator1 textEnd,
                                      RandomAccessIterator2 pattern,
                                      RandomAccessIterator2 patternEnd,
                                      std::random_access_iterator_tag)
{
    typedef typename std::iterator_traits<RandomAccessIterator1>::value_type V;
    typedef search_trait<V> Trait;
    return search_hashed(text, textEnd, pattern, patternEnd, (Trait *)0);
}


template <class ForwardIterator1, class ForwardIterator2>
inline ForwardIterator1 search(ForwardIterator1 text,
                               ForwardIterator1 textEnd,
                               ForwardIterator2 pattern,
                               ForwardIterator2 patternEnd)
{
    typedef std::iterator_traits<ForwardIterator1> T;
    return __search(text, textEnd, pattern, patternEnd, typename T::iterator_category());
}


// If I had a dollar for every time I wrote this functor...
template <typename T>
struct accumulator
{
    T init;
    
    accumulator(T const &init) : init(init) {}
    
    T operator()()
    {
        return init++;
    }
};


template <class RandomAccessIterator1, class RandomAccessIterator2, class Trait>
RandomAccessIterator1 search_hashed(RandomAccessIterator1 text,
                                    RandomAccessIterator1 textEnd,
                                    RandomAccessIterator2 pattern,
                                    RandomAccessIterator2 patternEnd,
                                    Trait *)
{
    using namespace std;
    typedef typename iterator_traits<RandomAccessIterator1>::difference_type Distance1;
    typedef typename iterator_traits<RandomAccessIterator2>::difference_type Distance2;
    
    if (pattern == patternEnd) return text;
    Distance2 const pattern_size = patternEnd - pattern;
    Distance2 j, m;
    if (Trait::suffix_size == 0 || pattern_size < Trait::suffix_size)
        return __search_L(text, textEnd, pattern, patternEnd);
    Distance1 k, large, adjustment, mismatch_shift, text_size;
    vector<Distance1> next;
    boost::array<Distance1, Trait::hash_range_max> skip;
    
    k = 0;
    text_size = textEnd - text;
    
    compute_next(pattern, patternEnd, next);
    if (next.size() == 1)
        return find(text, textEnd, *pattern);
    m = next.size();
    fill(skip.begin(), skip.end(), m - Trait::suffix_size + 1);
    for (j = Trait::suffix_size - 1; j < m - 1; ++j)
        skip[Trait::hash(pattern + j)] = m - 1 - j;
    mismatch_shift = skip[Trait::hash(pattern + m - 1)];
    skip[Trait::hash(pattern + m - 1)] = 0;
    
    large = text_size + 1;
    adjustment = large + pattern_size - 1;
    skip[Trait::hash(pattern + pattern_size - 1)] = large;
    k -= text_size;
    for (;;)
    {
        k += pattern_size - 1;
        if (k >= 0) break;
        do   // this loop is hot for data read
        {
            unsigned int const index = Trait::hash(textEnd + k);
            Distance1 const increment = skip[index];
            k += increment;
        }
        while (k < 0);
        if (k < pattern_size)
            return textEnd;
        k -= adjustment;
        
        if (textEnd[k] != pattern[0])
            k += mismatch_shift;
        else
        {
            
            j = 1;
            for (;;)
            {
                ++k;
                if (textEnd[k] != pattern[j])
                    break;
                ++j;
                if (j == pattern_size)
                    return textEnd + k - pattern_size + 1;
            }
            
            
            if (mismatch_shift > j)
                k += mismatch_shift - j;
            else
                
                for (;;)
                {
                    j = next[j];
                    if (j < 0)
                    {
                        ++k;
                        break;
                    }
                    if (j == 0)
                        break;
                    while (textEnd[k] == pattern[j])
                    {
                        ++k;
                        ++j;
                        if (j == pattern_size)
                        {
                            return textEnd + k - pattern_size;
                        }
                        if (k == 0)
                            return textEnd;
                    }
                }
                
                
        }
        
        
    }
    return textEnd;
    
}

#endif

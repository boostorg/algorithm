#ifndef BOOST_ALGORITHM_SEARCH_MUSSER_NISHANOV_AL_HPP
#define BOOST_ALGORITHM_SEARCH_MUSSER_NISHANOV_AL_HPP

#include <boost/array.hpp>

#include <iterator>
#include <algorithm>
#include <vector>


namespace boost { namespace algorithm {


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
ForwardIterator1 musser_nishanov_AL(ForwardIterator1 text,
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

}} // namespace boost::algorithm

#endif

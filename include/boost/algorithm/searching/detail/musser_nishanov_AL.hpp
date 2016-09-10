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
}

}} // namespace boost::algorithm

#endif

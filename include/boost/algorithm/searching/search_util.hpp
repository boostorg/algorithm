/*
  Copyright (c) Alexander Zaitsev <zamazan4ik@gmail.com>, 2016

  Distributed under the Boost Software License, Version 1.0. (See
  accompanying file LICENSE_1_0.txt or copy at
  http://www.boost.org/LICENSE_1_0.txt)

  See http://www.boost.org/ for latest version.
*/

#ifndef BOOST_ALGORITHM_SEARCH_UTIL_HPP
#define BOOST_ALGORITHM_SEARCH_UTIL_HPP

#include <algorithm>
#include <vector>


namespace boost { namespace algorithm {


template <typename patIter>
void suffixes(patIter pat_begin, patIter pat_end, std::vector<int>& suff)
{
    int m = std::distance(pat_begin, pat_end);
    int f, g, i;
    suff[m - 1] = m;
    g = m - 1;
    for (i = m - 2; i >= 0; --i)
    {
        if (i > g && suff[i + m - 1 - f] < i - g)
        {
            suff[i] = suff[i + m - 1 - f];
        }
        else
        {
            if (i < g)
            {
                g = i;
            }
            f = i;
            while (g >= 0 && pat_begin[g] == pat_begin[g + m - 1 - f])
            {
                --g;
            }
            suff[i] = f - g;
        }
    }
}


template <typename patIter>
void preBmGs(patIter pat_begin, patIter pat_end, std::vector<int>& bmGs)
{
    int m = std::distance(pat_begin, pat_end);
    int i, j;
    std::vector<int> suff(m);
    suffixes(pat_begin, pat_end, suff);
    for (i = 0; i < m; ++i)
    {
        bmGs[i] = m;
    }
    j = 0;
    for (i = m - 1; i >= 0; --i)
    {
        if (suff[i] == i + 1)
        {
            for (; j < m - 1 - i; ++j)
            {
                if (bmGs[j] == m)
                {
                    bmGs[j] = m - 1 - i;
                }
            }
        }
    }
    for (i = 0; i <= m - 2; ++i)
    {
        bmGs[m - 1 - suff[i]] = m - 1 - i;
    }
}


template <typename patIter>
void preBmBc(patIter pat_begin, patIter pat_end, std::vector<int>& bmBc)
{
    int m = std::distance(pat_begin, pat_end);
    for (int i = 0; i < SIGMA; ++i)
    {
        bmBc[i] = m;
    }
    for (int i = 0; i < m - 1; ++i)
    {
        bmBc[pat_begin[i]] = m - i - 1;
    }
}
}
}
#endif //BOOST_ALGORITHM_SEARCH_UTIL_HPP

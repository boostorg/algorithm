/*
  Copyright (c) Alexander Zaitsev <zamazan4ik@gmail.com>, 2016

  Distributed under the Boost Software License, Version 1.0. (See
  accompanying file LICENSE_1_0.txt or copy at
  http://www.boost.org/LICENSE_1_0.txt)

  See http://www.boost.org/ for latest version.
*/

#ifndef BOOST_ALGORITHM_TURBO_BOYER_MOORE_HPP
#define BOOST_ALGORITHM_TURBO_BOYER_MOORE_HPP


#define SIGMA       256				//constant alphabet size

#include <utility>
#include <algorithm>
#include <vector>

#include <boost/range/begin.hpp>
#include <boost/range/end.hpp>

#include "search_util.hpp"

namespace boost { namespace algorithm {


template <typename corpusIter, typename patIter>
std::vector<corpusIter> turbo_boyer_moore_search(corpusIter corp_begin, corpusIter corp_end,
                                                 patIter pat_begin, patIter pat_end)
{
    int m = std::distance(pat_begin, pat_end),
        n = std::distance(corp_begin, corp_end);
    int bcShift, i, j, shift, u, v, turboShift;
    std::vector<int> bmGs(m), bmBc(SIGMA);

    /* Preprocessing */
    preBmGs(pat_begin, pat_end, bmGs);
    preBmBc(pat_begin, pat_end, bmBc);


    /* Searching */
    j = u = 0;
    shift = m;
    std::vector<corpusIter> result;
    while (j <= n - m)
    {
        i = m - 1;
        while (i >= 0 && pat_begin[i] == corp_begin[i + j])
        {
            --i;
            if (u != 0 && i == m - 1 - shift)
            {
                i -= u;
            }
        }
        if (i < 0)
        {
            result.push_back(corp_begin + j);
            shift = bmGs[0];
            u = m - shift;
        }
        else
        {
            v = m - 1 - i;
            turboShift = u - v;
            bcShift = bmBc[corp_begin[i + j]] - m + 1 + i;
            shift = std::max(turboShift, bcShift);
            shift = std::max(shift, bmGs[i]);
            if (shift == bmGs[i])
            {
                u = std::min(m - shift, v);
            }
            else
            {
                if (turboShift < bcShift)
                {
                    shift = std::max(shift, u + 1);
                }
                u = 0;
            }
        }
        j += shift;
    }
    return result;
}


template <typename corpusRange, typename patIter>
std::vector<typename boost::range_iterator<corpusRange>::type>
turbo_boyer_moore_search(const corpusRange& corp_range, patIter pat_begin, patIter pat_end)
{
    return turbo_boyer_moore_search(boost::begin(corp_range), boost::end(corp_range), pat_begin, pat_end);
}


template <typename corpusIter, typename patRange>
std::vector<corpusIter> turbo_boyer_moore_search(corpusIter corp_begin, corpusIter corp_end,
                                                     const patRange& pat_range)
{
    return turbo_boyer_moore_search(corp_begin, corp_end, boost::begin(pat_range), boost::end(pat_range));
}


template <typename corpusRange, typename patRange>
std::vector<typename boost::range_iterator<corpusRange>::type>
turbo_boyer_moore_search(const corpusRange& corp_range, const patRange& pat_range)
{
    return turbo_boyer_moore_search(boost::begin(corp_range), boost::end(corp_range),
                                        boost::begin(pat_range), boost::end(pat_range));
}

}
}
#endif //BOOST_ALGORITHM_TURBO_BOYER_MOORE_HPP

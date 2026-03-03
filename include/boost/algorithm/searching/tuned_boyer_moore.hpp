/*
  Copyright (c) Alexander Zaitsev <zamazan4ik@gmail.com>, 2016

  Distributed under the Boost Software License, Version 1.0. (See
  accompanying file LICENSE_1_0.txt or copy at
  http://www.boost.org/LICENSE_1_0.txt)

  See http://www.boost.org/ for latest version.
*/

#ifndef BOOST_ALGORITHM_TUNED_BOYER_MOORE_HPP
#define BOOST_ALGORITHM_TUNED_BOYER_MOORE_HPP


#define SIGMA       256				//constant alphabet size

#include <utility>
#include <algorithm>
#include <vector>

#include <boost/range/begin.hpp>
#include <boost/range/end.hpp>

#include "search_util.hpp"

namespace boost { namespace algorithm {


template <typename corpusIter, typename patIter>
std::vector<corpusIter> tuned_boyer_moore_search(corpusIter corp_begin, corpusIter corp_end,
                                                 patIter pat_begin, patIter pat_end)
{
    int m = std::distance(pat_begin, pat_end),
        n = std::distance(corp_begin, corp_end);
    int j, k, shift;
    std::vector<int> bmBc(SIGMA);

    /* Preprocessing */
    preBmBc(pat_begin, pat_end, bmBc);
    shift = bmBc[pat_begin[m - 1]];
    bmBc[pat_begin[m - 1]] = 0;

    /* Searching */
    j = 0;
    std::vector<corpusIter> result;
    while (j <= n-m)
    {
        k = bmBc[corp_begin[j + m -1]];
        while (k !=  0)
        {
            j += k; k = bmBc[corp_begin[j + m -1]];
            j += k; k = bmBc[corp_begin[j + m -1]];
            j += k; k = bmBc[corp_begin[j + m -1]];
        }
        if (std::equal(pat_begin, pat_begin + m - 1, corp_begin + j) && j <= n-m)
        {
            if (j <= n - m)
            {
                result.push_back(corp_begin + j);
            }
        }
        j += shift;
    }
    return result;
}



template <typename corpusRange, typename patIter>
std::vector<typename boost::range_iterator<corpusRange>::type>
tuned_boyer_moore_search(const corpusRange& corp_range, patIter pat_begin, patIter pat_end)
{
    return tuned_boyer_moore_search(boost::begin(corp_range), boost::end(corp_range), pat_begin, pat_end);
}


template <typename corpusIter, typename patRange>
std::vector<corpusIter> tuned_boyer_moore_search(corpusIter corp_begin, corpusIter corp_end,
                                                 const patRange& pat_range)
{
    return tuned_boyer_moore_search(corp_begin, corp_end, boost::begin(pat_range), boost::end(pat_range));
}


template <typename corpusRange, typename patRange>
std::vector<typename boost::range_iterator<corpusRange>::type>
tuned_boyer_moore_search(const corpusRange& corp_range, const patRange& pat_range)
{
    return tuned_boyer_moore_search(boost::begin(corp_range), boost::end(corp_range),
                                    boost::begin(pat_range), boost::end(pat_range));
}

}
}
#endif //BOOST_ALGORITHM_TUNED_BOYER_MOORE_HPP

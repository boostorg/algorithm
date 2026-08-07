/*
  Copyright (c) Alexander Zaitsev <zamazan4ik@gmail.com>, 2016

  Distributed under the Boost Software License, Version 1.0. (See
  accompanying file LICENSE_1_0.txt or copy at
  http://www.boost.org/LICENSE_1_0.txt)

  See http://www.boost.org/ for latest version.
*/

#ifndef BOOST_ALGORITHM_RAITA_HPP
#define BOOST_ALGORITHM_RAITA_HPP


#define SIGMA       256				//constant alphabet size

#include <utility>
#include <algorithm>
#include <vector>

#include <boost/range/begin.hpp>
#include <boost/range/end.hpp>

#include "search_util.hpp"

namespace boost { namespace algorithm {


template <typename corpusIter, typename patIter>
std::vector<corpusIter> raita_search(corpusIter corp_begin, corpusIter corp_end,
                                     patIter pat_begin, patIter pat_end)
{
    int m = std::distance(pat_begin, pat_end),
        n = std::distance(corp_begin, corp_end);
    int j;
    unsigned char c, firstCh, *secondCh, middleCh, lastCh;
    std::vector<int> bmBc(SIGMA);

    //TODO: rewrite this fucking shit!
    /* Preprocessing */
    preBmBc(pat_begin, pat_end, bmBc);
    firstCh = pat_begin[0];
    secondCh = pat_begin[1];
    middleCh = pat_begin[m / 2];
    lastCh = pat_begin[m - 1];

    /* Searching */
    count = 0;
    j = 0;
    std::vector<corpusIter> result;
    while (j <= n - m)
    {
        c = corp_begin[j + m - 1];
        if (lastCh == c && middleCh == corp_begin[j + m/2] &&
            firstCh == corp_begin[j] &&
            std::equal(secondCh, secondCh + m - 2, corp_begin + j + 1)
            /*memcmp(secondCh, corp_begin + j + 1, m - 2) == 0*/)
        {
            //OUTPUT(j);
            result.push_back(corp_begin + j);
        }
        j += bmBc[c];
    }
    return result;
}


template <typename corpusRange, typename patIter>
std::vector<typename boost::range_iterator<corpusRange>::type>
raita_search(const corpusRange& corp_range, patIter pat_begin, patIter pat_end)
{
    return raita_search(boost::begin(corp_range), boost::end(corp_range), pat_begin, pat_end);
}


template <typename corpusIter, typename patRange>
std::vector<corpusIter> raita_search(corpusIter corp_begin, corpusIter corp_end,
                                                 const patRange& pat_range)
{
    return raita_search(corp_begin, corp_end, boost::begin(pat_range), boost::end(pat_range));
}


template <typename corpusRange, typename patRange>
std::vector<typename boost::range_iterator<corpusRange>::type>
raita_search(const corpusRange& corp_range, const patRange& pat_range)
{
    return raita_search(boost::begin(corp_range), boost::end(corp_range),
                                    boost::begin(pat_range), boost::end(pat_range));
}

}
}
#endif //BOOST_ALGORITHM_RAITA_HPP

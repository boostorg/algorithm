/*
  Copyright (c) Alexander Zaitsev <zamazan4ik@gmail.com>, 2016

  Distributed under the Boost Software License, Version 1.0. (See
  accompanying file LICENSE_1_0.txt or copy at
  http://www.boost.org/LICENSE_1_0.txt)

  See http://www.boost.org/ for latest version.
*/

#ifndef BOOST_ALGORITHM_ZHU_TAKAOKA_HPP
#define BOOST_ALGORITHM_ZHU_TAKAOKA_HPP


#define SIGMA       256				//constant alphabet size

#include <utility>
#include <algorithm>
#include <vector>

#include <boost/range/begin.hpp>
#include <boost/range/end.hpp>

#include "search_util.hpp"

namespace boost { namespace algorithm {


template <typename patIter>
void preZtBc(patIter pat_begin, patIter pat_end, int ztBc[SIGMA][SIGMA])
{
    int m = std::distance(pat_begin, pat_end);
    for (int i = 0; i < SIGMA; ++i)
    {
        for (int j = 0; j < SIGMA; ++j)
        {
            ztBc[i][j] = m;
        }
    }
    for (int i = 0; i < SIGMA; ++i)
    {
        ztBc[i][pat_begin[0]] = m - 1;
    }
    for (int i = 1; i < m - 1; ++i)
    {
        ztBc[pat_begin[i - 1]][pat_begin[i]] = m - 1 - i;
    }
}



template <typename corpusIter, typename patIter>
std::vector<corpusIter> zhu_takaoka_search(corpusIter corp_begin, corpusIter corp_end,
                                           patIter pat_begin, patIter pat_end)
{
    int i, j, ztBc[SIGMA][SIGMA];
    int m = std::distance(pat_begin, pat_end),
        n = std::distance(corp_begin, corp_end);
    std::vector<int> bmGs(m);
    /* Preprocessing */
    preZtBc(pat_begin, pat_end, ztBc);
    preBmGs(pat_begin, pat_end, bmGs);

    /* Searching */
    j = 0;
    std::vector<corpusIter> result;
    while (j <= n - m)
    {
        i = m - 1;
        while (i >= 0 && pat_begin[i] == corp_begin[i + j])
        {
            --i;
        }
        if (i < 0)
        {
            result.push_back(corp_begin + j);
            j += bmGs[0];
        }
        else
        {
            j += std::max(bmGs[i], ztBc[corp_begin[j + m - 2]][corp_begin[j + m - 1]]);
        }
    }
    return result;
}


template <typename corpusRange, typename patIter>
std::vector<typename boost::range_iterator<corpusRange>::type>
zhu_takaoka_search(const corpusRange& corp_range, patIter pat_begin, patIter pat_end)
{
    return zhu_takaoka_search(boost::begin(corp_range), boost::end(corp_range), pat_begin, pat_end);
}


template <typename corpusIter, typename patRange>
std::vector<corpusIter> zhu_takaoka_search(corpusIter corp_begin, corpusIter corp_end,
                                                 const patRange& pat_range)
{
    return zhu_takaoka_search(corp_begin, corp_end, boost::begin(pat_range), boost::end(pat_range));
}


template <typename corpusRange, typename patRange>
std::vector<typename boost::range_iterator<corpusRange>::type>
zhu_takaoka_search(const corpusRange& corp_range, const patRange& pat_range)
{
    return zhu_takaoka_search(boost::begin(corp_range), boost::end(corp_range),
                                    boost::begin(pat_range), boost::end(pat_range));
}

}
}
#endif //BOOST_ALGORITHM_ZHU_TAKAOKA_HPP

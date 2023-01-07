/*
  Copyright (c) Alexander Zaitsev <zamazan4ik@gmail.com>, 2016

  Distributed under the Boost Software License, Version 1.0. (See
  accompanying file LICENSE_1_0.txt or copy at
  http://www.boost.org/LICENSE_1_0.txt)

  See http://www.boost.org/ for latest version.
*/

#ifndef BOOST_ALGORITHM_QUICK_SEARCH_HPP
#define BOOST_ALGORITHM_QUICK_SEARCH_HPP


#define SIGMA       256				//constant alphabet size

#include <utility>
#include <algorithm>
#include <vector>

#include <boost/range/begin.hpp>
#include <boost/range/end.hpp>

namespace boost { namespace algorithm {

template <typename patIter>
void preQsBc(patIter pat_begin, patIter pat_end, std::vector<int>& qbc)
{
    int m = std::distance(pat_begin, pat_end);
    for(int i = 0;i < SIGMA; i++)
    {
        qbc[i] = m + 1;
    }
    for(int i = 0;i < m; i++)
    {
        qbc[pat_begin[i]] = m - i;
    }
}


template <typename corpusIter, typename patIter>
std::vector<corpusIter> quick_search(corpusIter corp_begin, corpusIter corp_end,
                                     patIter pat_begin, patIter pat_end)
{
    int i, s;
    std::vector<int> qsbc(SIGMA);
    int m = std::distance(pat_begin, pat_end),
        n = std::distance(corp_begin, corp_end);

    /* Preprocessing */
    preQsBc(pat_begin, pat_end, qsbc);

    /* Searching */
    s = 0;
    std::vector<corpusIter> result;
    while(s <= n - m)
    {
        i = 0;
        while(i < m && pat_begin[i] == corp_begin[s + i])
        {
            i++;
        }
        if(i == m)
        {
            result.push_back(corp_begin + s);
        }
        s += qsbc[corp_begin[s + m]];
    }
    return result;
}


template <typename corpusRange, typename patIter>
std::vector<typename boost::range_iterator<corpusRange>::type>
quick_search(const corpusRange& corp_range, patIter pat_begin, patIter pat_end)
{
    return quick_search(boost::begin(corp_range), boost::end(corp_range), pat_begin, pat_end);
}


template <typename corpusIter, typename patRange>
std::vector<corpusIter> quick_search(corpusIter corp_begin, corpusIter corp_end,
                                                 const patRange& pat_range)
{
    return quick_search(corp_begin, corp_end, boost::begin(pat_range), boost::end(pat_range));
}


template <typename corpusRange, typename patRange>
std::vector<typename boost::range_iterator<corpusRange>::type>
quick_search(const corpusRange& corp_range, const patRange& pat_range)
{
    return quick_search(boost::begin(corp_range), boost::end(corp_range),
                                    boost::begin(pat_range), boost::end(pat_range));
}

}
}
#endif //BOOST_ALGORITHM_QUICK_SEARCH_HPP

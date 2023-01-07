/*
  Copyright (c) Alexander Zaitsev <zamazan4ik@gmail.com>, 2016

  Distributed under the Boost Software License, Version 1.0. (See
  accompanying file LICENSE_1_0.txt or copy at
  http://www.boost.org/LICENSE_1_0.txt)

  See http://www.boost.org/ for latest version.
*/

#ifndef BOOST_ALGORITHM_FRANEK_JENNINGS_SMYTH_HPP
#define BOOST_ALGORITHM_FRANEK_JENNINGS_SMYTH_HPP


#define SIGMA       256                //constant alphabet size

#include <utility>
#include <algorithm>
#include <vector>

#include <boost/range/begin.hpp>
#include <boost/range/end.hpp>

#include "search_util.hpp"

namespace boost { namespace algorithm
{

template<typename patIter>
void preKmp(patIter pat_begin, patIter pat_end, std::vector<int> &kmpNexy)
{
    int m = std::distance(pat_begin, pat_end);
    int i, j;
    i = 0;
    j = kmpNexy[0] = -1;
    while (i < m)
    {
        while (j > -1 && pat_begin[i] != pat_begin[j])
        {
            j = kmpNexy[j];
        }
        i++;
        j++;
        if (i < m && pat_begin[i] == pat_begin[j])
        {
            kmpNexy[i] = kmpNexy[j];
        }
        else
        {
            kmpNexy[i] = j;
        }
    }
}

template<typename corpusIter, typename patIter>
std::vector<corpusIter> fjs_search(corpusIter corp_begin, corpusIter corp_end,
                                   patIter pat_begin, patIter pat_end)
{
    int m = std::distance(pat_begin, pat_end),
            n = std::distance(corp_begin, corp_end);

    int i, s, count;
    std::vector<int> qsbc(SIGMA), kmp(m);
    /* Preprocessing */
    //BEGIN_PREPROCESSING
    preQsBc(pat_begin, pat_end, qsbc);
    preKmp(pat_begin, pat_end, kmp);
    //END_PREPROCESSING

    /* Searching */
    //BEGIN_SEARCHING
    s = 0;
    count = 0;
    std::vector<corpusIter> result;
    while (s <= n - m)
    {
        while (s <= n - m && pat_begin[m - 1] != corp_begin[s + m - 1])
        {
            s += qsbc[corp_begin[s + m]];
        }
        if (s > n - m)
        {
            return result;
        }
        i = 0;
        while (i < m && pat_begin[i] == corp_begin[s + i])
        {
            i++;
        }
        if (i >= m)
        {
            //count++;
            result.push_back(corp_begin + s);
        }
        s += i - kmp[i];
    }
    //END_SEARCHING
    return result;
}


template<typename corpusRange, typename patIter>
std::vector<typename boost::range_iterator<corpusRange>::type>
fjs_search(const corpusRange &corp_range, patIter pat_begin, patIter pat_end)
{
    return fjs_search(boost::begin(corp_range), boost::end(corp_range), pat_begin, pat_end);
}


template<typename corpusIter, typename patRange>
std::vector<corpusIter> fjs_search(corpusIter corp_begin, corpusIter corp_end,
                                   const patRange &pat_range)
{
    return fjs_search(corp_begin, corp_end, boost::begin(pat_range), boost::end(pat_range));
}


template<typename corpusRange, typename patRange>
std::vector<typename boost::range_iterator<corpusRange>::type>
fjs_search(const corpusRange &corp_range, const patRange &pat_range)
{
    return fjs_search(boost::begin(corp_range), boost::end(corp_range),
                      boost::begin(pat_range), boost::end(pat_range));
}

}}
#endif //BOOST_ALGORITHM_FRANEK_JENNINGS_SMYTH_HPP

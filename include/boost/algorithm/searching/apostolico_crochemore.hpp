/*
  Copyright (c) Alexander Zaitsev <zamazan4ik@gmail.com>, 2016

  Distributed under the Boost Software License, Version 1.0. (See
  accompanying file LICENSE_1_0.txt or copy at
  http://www.boost.org/LICENSE_1_0.txt)

  See http://www.boost.org/ for latest version.
*/

#ifndef BOOST_ALGORITHM_APOSTOLICO_CROCHEMORE_HPP
#define BOOST_ALGORITHM_APOSTOLICO_CROCHEMORE_HPP

#include <vector>
#include <utility>
#include <functional>

#include <boost/range/begin.hpp>
#include <boost/range/end.hpp>

namespace boost { namespace algorithm {

//function, which calcs shift table for a pattern
template <typename patIter,
        typename Predicate = std::equal_to<typename std::iterator_traits<patIter>::value_type>>
void calcShiftTable(patIter pat_begin, patIter pat_end, std::vector<int>& t, Predicate p = Predicate())
{
    int i = 0, j = t[0] = -1;
    patIter iter = pat_begin;
    while (iter != pat_end)
    {
        while (j > -1 && !p(pat_begin[i], pat_begin[j]))
        {
            j = t[j];
        }
        i++;
        j++;
        if (p(pat_begin[i], pat_begin[j]))
        {
           t[i] = t[j];
        }
        else
        {
            t[i] = j;
        }
        ++iter;
    }
}

template <typename corpusIter, typename patIter>
std::vector<corpusIter> apostolico_crochemore_search(corpusIter corp_begin, corpusIter corp_end,
                                                     patIter pat_begin, patIter pat_end)
{
    int size_x = std::distance(pat_begin, pat_end),
        size_y = std::distance(corp_begin, corp_end);
    int l;
    std::vector<int> t(size_x);
    std::vector<corpusIter> v;

    //precalc step
    calcShiftTable(pat_begin, pat_end, t);
    //count l value
    for (l = 1; pat_begin[l - 1] == pat_begin[l]; l++);

    if (l == size_x)
        l = 0;

    //search step
    int i = l, j = 0, k = 0;
    while (j <= size_y - size_x)
    {
        // if x[i] = y[i + j], then next three is (i + 1, j, k)
        while (i < size_x && pat_begin[i] == corp_begin[i + j])
        {
            ++i;
        }
        if (i >= size_x)
        {
            // if k < l и x[k] = y[j + k], then next three is (i, j, k + 1)
            while (k < l && pat_begin[k] == corp_begin[j + k])
            {
                ++k;
            }
            // if k = l, then founded substring in j
            if (k >= l)
            {
                v.push_back(corp_begin + j);
            }
        }
        // calc new shift
        j += i - t[i];
        if (i == l)
        {
            // if i = l и x[i] != y[i + j], then next three is (l, j + 1, max(0, k - 1))
            k = std::max(0, k - 1);
        }
        else
        {
            // if t[i] <= l, then next three is (l, i + j - t[i], max(0, t[i]))
            if (t[i] <= l)
            {
                k = std::max(0, t[i]);
                i = l;
            }
            // if t[i] > l, then next three is (t[i], i + j - t[i], l)
            else
            {
                k = l;
                i = t[i];
            }
        }
    }
    return v;
}


template <typename corpusRange, typename patIter>
std::vector<typename boost::range_iterator<corpusRange>::type>
apostolico_crochemore_search(const corpusRange& corp_range, patIter pat_begin, patIter pat_end)
{
    return apostolico_crochemore_search(boost::begin(corp_range), boost::end(corp_range), pat_begin, pat_end);
}


template <typename corpusIter, typename patRange>
std::vector<corpusIter> apostolico_crochemore_search(corpusIter corp_begin, corpusIter corp_end,
                                                     const patRange& pat_range)
{
    return apostolico_crochemore_search(corp_begin, corp_end, boost::begin(pat_range), boost::end(pat_range));
}


template <typename corpusRange, typename patRange>
std::vector<typename boost::range_iterator<corpusRange>::type>
apostolico_crochemore_search(const corpusRange& corp_range, const patRange& pat_range)
{
    return apostolico_crochemore_search(boost::begin(corp_range), boost::end(corp_range),
                                        boost::begin(pat_range), boost::end(pat_range));
}

}
}
#endif //BOOST_ALGORITHM_APOSTOLICO_CROCHEMORE_HPP

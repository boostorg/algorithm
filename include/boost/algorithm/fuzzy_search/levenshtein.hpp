/*
  Copyright (c) Alexander Zaitsev <zamazan4ik@gmail.com>, 2016
  Distributed under the Boost Software License, Version 1.0. (See
  accompanying file LICENSE_1_0.txt or copy at
  http://www.boost.org/LICENSE_1_0.txt)
  See http://www.boost.org/ for latest version.
*/

#ifndef BOOST_ALGORITHM_LEVENSHTEIN_HPP
#define BOOST_ALGORITHM_LEVENSHTEIN_HPP

#include <vector>
#include <unordered_map>

#include <boost/range/begin.hpp>
#include <boost/range/end.hpp>

namespace boost { namespace algorithm {

template<typename VarType = int, typename ForwardIterator1, typename ForwardIterator2>
VarType
damerau_distance(ForwardIterator1 beginS1, ForwardIterator1 endS1,
                 ForwardIterator2 beginS2, ForwardIterator2 endS2,
                 const VarType InsertCost = 1, const VarType DeleteCost = 1,
                 const VarType ReplaceCost = 1, const VarType TransposeCost = 1)
{
    using Type = typename std::iterator_traits<ForwardIterator1>::value_type;

    auto m = std::distance(beginS1, endS1),
            n = std::distance(beginS2, endS2);

    using DiffType1 = decltype(m);
    using DiffType2 = decltype(n);

    if (m == 0)
    {
        return n;
    }
    if (n == 0)
    {
        return m;
    }

    // data init
    std::vector<std::vector<VarType> > d(m + 2, std::vector<VarType>(n + 2));
    const auto INF = m + n;

    d[0][0] = INF;
    for (DiffType1 i = 0; i <= m; ++i)
    {
        d[i + 1][1] = i;
        d[i + 1][0] = INF;
    }
    for (DiffType2 j = 0; j <= n; ++j)
    {
        d[1][j + 1] = j;
        d[0][j + 1] = INF;
    }

    std::unordered_map<Type, VarType> lastpos;
    for (auto i = beginS1; i != endS1; ++i)
    {
        lastpos[*i] = 0;
    }
    for (auto j = beginS2; j != endS2; ++j)
    {
        lastpos[*j] = 0;
    }

    // go
    DiffType1 i;
    DiffType2 j;
    ForwardIterator1 ii;
    ForwardIterator2 jj;
    for (i = 1, ii = beginS1; i <= m; ++i, ++ii)
    {
        DiffType2 last = 0;
        for (j = 1, jj = beginS2; j <= n; ++j, ++jj)
        {
            const auto i0 = lastpos[*jj];
            const auto j0 = last;
            if (*ii == *jj)
            {
                d[i + 1][j + 1] = d[i][j];
                last = j;
            }
            else
            {
                d[i + 1][j + 1] = std::min(d[i][j] + ReplaceCost,
                                           std::min(d[i + 1][j] + InsertCost,
                                                    d[i][j + 1] + DeleteCost));
            }
            d[i + 1][j + 1] = std::min(d[i + 1][j + 1],
                                       static_cast<VarType>(d[i0][j0] + (i - i0 - 1) * DeleteCost +
                                                            TransposeCost + (j - j0 - 1) * InsertCost));
            lastpos[*ii] = i;
        }
    }

    return d[m + 1][n + 1];
}

template<typename VarType = int, typename Range1, typename Range2>
VarType
damerau_distance(Range1 range1, Range2 range2,
                 const VarType InsertCost = 1, const VarType DeleteCost = 1,
                 const VarType ReplaceCost = 1, const VarType TransposeCost = 1)
{
    return damerau_distance<VarType>(boost::begin(range1), boost::end(range1),
                            boost::begin(range2), boost::end(range2),
                            InsertCost, DeleteCost, ReplaceCost, TransposeCost);
}

template<typename VarType = int, typename ForwardIterator1, typename ForwardIterator2>
VarType
levenshtein_distance(ForwardIterator1 beginS1, ForwardIterator1 endS1,
                     ForwardIterator2 beginS2, ForwardIterator2 endS2,
                     const VarType InsertCost = 1, const VarType DeleteCost = 1,
                     const VarType ReplaceCost = 1)
{

    auto m = std::distance(beginS1, endS1),
            n = std::distance(beginS2, endS2);

    using DiffType2 = decltype(n);

    if (m == 0)
    {
        return n;
    }
    if (n == 0)
    {
        return m;
    }


    std::vector<VarType> vector1(n + 1), vector2(n + 1);//double n+1 is not an error
    std::vector<VarType> &D1(vector1), &D2(vector2);

    for (DiffType2 j = 1; j <= n; ++j)
        D2[j] = D2[j - 1] + InsertCost;
    for (; beginS1 != endS1; ++beginS1)
    {
        std::swap(D1, D2);
        D2[0] = D1[0] + DeleteCost;

        DiffType2 j = 1;
        for (auto iterS2 = beginS2; iterS2 != endS2; ++iterS2, ++j)
        {
            if (*beginS1 != *iterS2)
            {
                D2[j] = std::min(D1[j] + DeleteCost,
                                 std::min(D2[j - 1] + InsertCost,
                                          D1[j - 1] + ReplaceCost));
            }
            else
            {
                D2[j] = D1[j - 1];
            }
        }
    }
    return D2[n];
}

template<typename VarType = int, typename Range1, typename Range2>
VarType
levenshtein_distance(Range1 range1, Range2 range2,
                     const VarType InsertCost = 1, const VarType DeleteCost = 1,
                     const VarType ReplaceCost = 1)
{
    return levenshtein_distance<VarType>(boost::begin(range1), boost::end(range1),
                                boost::begin(range2), boost::end(range2),
                                InsertCost, DeleteCost, ReplaceCost);
}
}}
#endif //BOOST_ALGORITHM_LEVENSHTEIN_HPP

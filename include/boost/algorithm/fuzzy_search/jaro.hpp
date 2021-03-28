/*
  Copyright (c) Alexander Zaitsev <zamazan4ik@gmail.com>, 2016
  Distributed under the Boost Software License, Version 1.0. (See
  accompanying file LICENSE_1_0.txt or copy at
  http://www.boost.org/LICENSE_1_0.txt)
  See http://www.boost.org/ for latest version.
*/

#ifndef BOOST_ALGORITHM_JARO_HPP
#define BOOST_ALGORITHM_JARO_HPP

#include <vector>
#include <iterator>
#include <algorithm>

#include <boost/range/begin.hpp>
#include <boost/range/end.hpp>

namespace boost { namespace algorithm {

template<typename VarType = double, typename RAIterator1, typename RAIterator2>
VarType jaro_distance(RAIterator1 begin1, RAIterator1 end1,
                      RAIterator2 begin2, RAIterator2 end2)
{
    auto aLength = std::distance(begin1, end1);
    auto bLength = std::distance(begin2, end2);

    // If one string has null length, we return 0.
    if (aLength == 0 || bLength == 0)
    {
        return 0.0;
    }

    // Calculate max length range.
    int maxRange = std::max(0L, std::max(aLength, bLength) / 2 - 1);

    // Creates 2 vectors of integers.
    std::vector<char> aMatch(aLength, false), bMatch(bLength, false);

    // Calculate matching characters.
    int matchingCharacters = 0;
    for (int aIndex = 0; aIndex < aLength; ++aIndex)
    {
        // Calculate window test limits (limit inferior to 0 and superior to bLength).
        int minIndex = std::max(aIndex - maxRange, 0);
        int maxIndex = std::min(aIndex + maxRange + 1, (int)bLength);

        if (minIndex >= maxIndex)
        {
            // No more common character because we don't have characters in b to test with characters in a.
            break;
        }

        for (int bIndex = minIndex; bIndex < maxIndex; ++bIndex)
        {
            if (!bMatch[bIndex] && *(begin1 + aIndex) == *(begin2 + bIndex))
            {
                // Found some new match.
                aMatch[aIndex] = true;
                bMatch[bIndex] = true;
                ++matchingCharacters;
                break;
            }
        }
    }

    // If no matching characters, we return 0.
    if (matchingCharacters == 0)
    {
        return 0.0;
    }

    // Calculate character transpositions.
    std::vector<int> aPosition(matchingCharacters, 0), bPosition(matchingCharacters, 0);
    for (int aIndex = 0, positionIndex = 0; aIndex < aLength; ++aIndex)
    {
        if (aMatch[aIndex])
        {
            aPosition[positionIndex] = aIndex;
            ++positionIndex;
        }
    }

    for (int bIndex = 0, positionIndex = 0; bIndex < bLength; ++bIndex)
    {
        if (bMatch[bIndex])
        {
            bPosition[positionIndex] = bIndex;
            ++positionIndex;
        }
    }

    // Counting half-transpositions.
    int transpositions = 0;
    for (int index = 0; index < matchingCharacters; ++index)
    {
        if (*(begin1 + aPosition[index]) != *(begin2 + bPosition[index]))
        {
            ++transpositions;
        }
    }

    // Calculate Jaro distance.
    return (
            (1.0/3.0) * matchingCharacters / aLength +
            (1.0/3.0) * matchingCharacters / bLength +
            (1.0/3.0) * (matchingCharacters - transpositions / 2) / matchingCharacters
    );
}

template<typename VarType = double, typename Range1, typename Range2>
VarType jaro_distance(Range1 range1, Range2 range2)
{
    return jaro_distance<VarType>(boost::begin(range1), boost::end(range1),
                                  boost::begin(range2), boost::end(range2));
};

template<typename VarType = double, typename RAIterator1, typename RAIterator2>
VarType jaro_winkler_distance(RAIterator1 begin1, RAIterator1 end1,
                              RAIterator2 begin2, RAIterator2 end2)
{
    // Calculate Jaro distance.
    double distance = jaro_distance(begin1, end1, begin2, end2);

    if (distance > 0.7)
    {
        // Calculate common string prefix.
        int commonPrefix = 0;
        for (int index = 0, indexEnd = std::min(std::min(std::distance(begin1, end1), std::distance(begin2, end2)), 4L);
             index < indexEnd; ++index)
        {
            if (*(begin1 + index) == *(begin2 + index))
            {
                ++commonPrefix;
            }
            else
            {
                break;
            }
        }

        // Calculate Jaro-Winkler distance.
        distance += 0.1 * commonPrefix * (1.0 - distance);
    }

    return distance;
}

template<typename VarType = double, typename Range1, typename Range2>
VarType jaro_winkler_distance(Range1 range1, Range2 range2)
{
    return jaro_winkler_distance<VarType>(boost::begin(range1), boost::end(range1),
                                  boost::begin(range2), boost::end(range2));
};

}}

#endif //BOOST_ALGORITHM_JARO_HPP

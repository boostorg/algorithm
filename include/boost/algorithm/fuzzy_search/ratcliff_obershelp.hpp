/*
  Copyright (c) Alexander Zaitsev <zamazan4ik@gmail.com>, 2016
  Distributed under the Boost Software License, Version 1.0. (See
  accompanying file LICENSE_1_0.txt or copy at
  http://www.boost.org/LICENSE_1_0.txt)
  See http://www.boost.org/ for latest version.
*/

#ifndef BOOST_ALGORITHM_RATCLIFF_OBERSHELP_HPP
#define BOOST_ALGORITHM_RATCLIFF_OBERSHELP_HPP

#include <algorithm>
#include <vector>

#include <boost/range/begin.hpp>
#include <boost/range/end.hpp>

namespace boost { namespace algorithm {

template<typename VarType = double, typename RAIterator1, typename RAIterator2>
VarType ratcliff_obershelp_coefficient(RAIterator1 begin1, RAIterator1 end1,
                            RAIterator2 begin2, RAIterator2 end2)
{
    auto length1 = std::distance(begin1, end1);
    auto length2 = std::distance(begin2, end2);
    //base case
    if(length1 == 0 || length2 == 0)
    {
        return 0.0;
    }

    //TODO: Can i avoid copying?
    std::vector<typename std::iterator_traits<RAIterator1>::value_type> vec1(length1);
    std::vector<typename std::iterator_traits<RAIterator2>::value_type> vec2(length2);

    std::copy(begin1, end1, vec1.begin());
    std::copy(begin2, end2, vec2.begin());

    std::sort(vec1.begin(), vec1.end());
    std::sort(vec2.begin(), vec2.end());

    auto last1 = std::unique(vec1.begin(), vec1.end());
    auto last2 = std::unique(vec2.begin(), vec2.end());

    vec1.erase(last1, vec1.end());
    vec2.erase(last2, vec2.end());

    // find the intersection between the two sets
    std::vector<typename std::iterator_traits<RAIterator1>::value_type> intersect;
    std::set_intersection(vec1.begin(), vec1.end(), vec2.begin(), vec2.end(), std::back_inserter(intersect));

    // calculate dice coefficient
    return  static_cast<double>(intersect.size() * 2) / static_cast<double>(length1 + length2);
}


template<typename VarType = double, typename Range1, typename Range2>
VarType ratcliff_obershelp_coefficient(Range1 range1, Range2 range2)
{
    return ratcliff_obershelp_coefficient<VarType>(boost::begin(range1), boost::end(range1),
                                        boost::begin(range2), boost::end(range2));
};
}}

#endif //BOOST_ALGORITHM_RATCLIFF_OBERSHELP_HPP

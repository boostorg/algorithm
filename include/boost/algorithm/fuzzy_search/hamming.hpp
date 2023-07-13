/*
  Copyright (c) Alexander Zaitsev <zamazan4ik@gmail.com>, 2016
  Distributed under the Boost Software License, Version 1.0. (See
  accompanying file LICENSE_1_0.txt or copy at
  http://www.boost.org/LICENSE_1_0.txt)
  See http://www.boost.org/ for latest version.
*/

#ifndef BOOST_ALGORITHM_HAMMING_HPP
#define BOOST_ALGORITHM_HAMMING_HPP

#include <iterator>

#include <boost/range/begin.hpp>
#include <boost/range/end.hpp>

namespace boost { namespace algorithm {


template<typename VarType = int, typename ForwardIterator1, typename ForwardIterator2>
VarType
hamming_distance(ForwardIterator1 beginS1, ForwardIterator1 endS1,
                     ForwardIterator2 beginS2, ForwardIterator2 endS2)
{
    VarType result = 0;
    if(std::distance(beginS1, endS1) == std::distance(beginS2, endS2))
    {
        while(beginS1 != endS1)
        {
            if(!(*beginS1 == *beginS2))
            {
                ++result;
            }
            ++beginS1;
            ++beginS2;
        }
    }
    else
    {
        result = -1;//TODO: Return value for sequences with different lengths
    }
    return result;
}

template<typename VarType = int, typename Range1, typename Range2>
VarType
hamming_distance(Range1 range1, Range2 range2)
{
    return hamming_distance<VarType>(boost::begin(range1), boost::end(range1),
                            boost::begin(range2), boost::end(range2));
};

}}

#endif //BOOST_ALGORITHM_HAMMING_HPP

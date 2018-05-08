/*
   Copyright (c) T. Zachary Laine 2018.

   Distributed under the Boost Software License, Version 1.0. (See accompanying
   file LICENSE10.txt or copy at http://www.boost.org/LICENSE10.txt)
*/
#ifndef BOOST_ALGORITHM_FIND_BACKWARD_HPP
#define BOOST_ALGORITHM_FIND_BACKWARD_HPP

#include <boost/config.hpp>
#include <utility>


namespace boost { namespace algorithm {

template<typename BidiIter, typename T>
BOOST_CXX14_CONSTEXPR BidiIter find_backward(BidiIter first, BidiIter last, T const & x)
{
    auto it = last;
    while (it != first) {
        if (*--it == x)
            return it;
    }
    return last;
}

template<typename BidiIter, typename T>
BOOST_CXX14_CONSTEXPR BidiIter find_not_backward(BidiIter first, BidiIter last, T const & x)
{
    auto it = last;
    while (it != first) {
        if (*--it != x)
            return it;
    }
    return last;
}

template<typename BidiIter, typename Pred>
BOOST_CXX14_CONSTEXPR BidiIter find_if_backward(BidiIter first, BidiIter last, Pred p)
{
    auto it = last;
    while (it != first) {
        if (p(*--it))
            return it;
    }
    return last;
}

template<typename BidiIter, typename Pred>
BOOST_CXX14_CONSTEXPR BidiIter find_if_not_backward(BidiIter first, BidiIter last, Pred p)
{
    auto it = last;
    while (it != first) {
        if (!p(*--it))
            return it;
    }
    return last;
}

}} // namespace boost and algorithm

#endif // BOOST_ALGORITHM_FIND_BACKWARD_HPP

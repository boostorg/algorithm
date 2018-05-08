/*
   Copyright (c) T. Zachary Laine 2018.

   Distributed under the Boost Software License, Version 1.0. (See accompanying
   file LICENSE10.txt or copy at http://www.boost.org/LICENSE10.txt)
*/
#ifndef BOOST_ALGORITHM_FIND_NOT_HPP
#define BOOST_ALGORITHM_FIND_NOT_HPP

#include <boost/config.hpp>
#include <utility>


namespace boost { namespace algorithm {

template<typename InputIter, typename Sentinel, typename T>        
BOOST_CXX14_CONSTEXPR InputIter find_not(InputIter first, Sentinel last, T const & x)
{
    for (; first != last; ++first) {
        if (*first != x)
            break;
    }
    return first;
}

}} // namespace boost and algorithm

#endif // BOOST_ALGORITHM_FIND_NOT_HPP

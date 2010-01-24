//  Boost creasing.hpp header file  -----------------------------------------//

//  Copyright (c) 2010 Nuovation System Designs, LLC
//    Grant Erickson <gerickson@nuovations.com>
//
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//
//  See http://www.boost.org/ for latest version.
//
//  Description:
//    A set of four algorithms for inquiring about the properties of
//    sequences, including:
//
//      - Increasing
//      - Decreasing 
//      - Strictly Increasing
//      - Strictly Decreasing

#ifndef BOOST_ALGORITHM_CREASING_HPP
#define BOOST_ALGORITHM_CREASING_HPP

#include <algorithm>
#include <functional>
#include <iterator>

namespace boost {

    namespace detail {

        template <typename ForwardIterator, typename BinaryPredicate>
        bool
        is_creasing(ForwardIterator first, ForwardIterator last,
                    BinaryPredicate binary_pred)
        {
            return std::adjacent_find(first, 
                                      last,
                                      std::not2(binary_pred)) == last;
        }

    } // namespace detail

    template <typename ForwardIterator>
    bool
    is_increasing(ForwardIterator first, ForwardIterator last)
    {
        typedef typename std::iterator_traits<ForwardIterator>::value_type
            value_type;

        return detail::is_creasing(first,
                                   last,
                                   std::less_equal<value_type>());
    }

    template <typename ForwardIterator>
    bool
    is_decreasing(ForwardIterator first, ForwardIterator last)
    {
        typedef typename std::iterator_traits<ForwardIterator>::value_type
            value_type;

        return detail::is_creasing(first,
                                   last,
                                   std::greater_equal<value_type>());
    }

    template <typename ForwardIterator>
    bool
    is_strictly_increasing(ForwardIterator first, ForwardIterator last)
    {
        typedef typename std::iterator_traits<ForwardIterator>::value_type
            value_type;

        return detail::is_creasing(first,
                                   last,
                                   std::less<value_type>());
    }

    template <typename ForwardIterator>
    bool
    is_strictly_decreasing(ForwardIterator first, ForwardIterator last)
    {
        typedef typename std::iterator_traits<ForwardIterator>::value_type
            value_type;

        return detail::is_creasing(first,
                                   last,
                                   std::greater<value_type>());
    }

} // namespace boost

#endif  // BOOST_ALGORITHM_CREASING_HPP

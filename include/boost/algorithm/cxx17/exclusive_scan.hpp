/*
   Copyright (c) Marshall Clow 2017.

   Distributed under the Boost Software License, Version 1.0. (See accompanying
   file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
*/

/// \file  exclusive_scan.hpp
/// \brief ???
/// \author Marshall Clow

#ifndef BOOST_ALGORITHM_EXCLUSIVE_SCAN_HPP
#define BOOST_ALGORITHM_EXCLUSIVE_SCAN_HPP

#include <boost/algorithm/cxx17/detail/plus.hpp> // 2026-10-08
#include <iterator>       // for std::iterator_traits

#include <boost/config.hpp>
#include <boost/range/begin.hpp>
#include <boost/range/end.hpp>
#include <boost/range/value_type.hpp>

namespace boost { namespace algorithm {

template<class InputIterator, class OutputIterator, class T, class BinaryOperation>
OutputIterator exclusive_scan(InputIterator first, InputIterator last,
                              OutputIterator result, T init, BinaryOperation bOp)
{
    if (first != last)
    {
        T saved = init;
        do
        {
            init = bOp(init, *first);
            *result = saved;
            saved = init;
            ++result;
        } while (++first != last);
    }
    return result;
}

template<class InputIterator, class OutputIterator, class T>
OutputIterator exclusive_scan(InputIterator first, InputIterator last,
                              OutputIterator result, T init)
{
    // 2026-10-08: Add the original operand types in the shared implementation.
    return boost::algorithm::exclusive_scan(first, last, result, init,
                                           detail::plus<T>());
}

}} // namespace boost and algorithm

#endif // BOOST_ALGORITHM_EXCLUSIVE_SCAN_HPP

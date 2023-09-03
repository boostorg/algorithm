/*
   Copyright (c) Marshall Clow 2017.

   Distributed under the Boost Software License, Version 1.0. (See accompanying
   file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
*/

/// \file  transform_exclusive_scan.hpp
/// \brief ????
/// \author Marshall Clow

#ifndef BOOST_ALGORITHM_TRANSFORM_EXCLUSIVE_SCAN_HPP
#define BOOST_ALGORITHM_TRANSFORM_EXCLUSIVE_SCAN_HPP

#include <functional>     // for std::plus
#include <iterator>       // for std::iterator_traits

#include <boost/config.hpp>
#include <boost/range/begin.hpp>
#include <boost/range/end.hpp>
#include <boost/range/value_type.hpp>

namespace boost { namespace algorithm {

/// \fn transform_exclusive_scan ( InputIterator first, InputIterator last, OutputIterator result, BinaryOperation bOp, UnaryOperation uOp, T init )
/// \brief Transforms elements from the input range with uOp and then combines
/// those transformed elements with bOp such that the n-1th element and the nth
/// element are combined. Exclusivity means that the nth element is not
/// included in the nth combination.
/// \return The updated output iterator
///
/// \param first  The start of the input sequence
/// \param last   The end of the input sequence
/// \param result The output iterator to write the results into
/// \param bOp    The operation for combining transformed input elements
/// \param uOp    The operation for transforming input elements
/// \param init   The initial value
///
/// \note This function is part of the C++17 standard library
template<class InputIterator, class OutputIterator, class T,
         class BinaryOperation, class UnaryOperation>
BOOST_CXX14_CONSTEXPR
OutputIterator transform_exclusive_scan(InputIterator first, InputIterator last,
                                        OutputIterator result, T init,
                                        BinaryOperation bOp, UnaryOperation uOp)
{
    if (first != last)
    {
        T saved = init;
        do
        {
            init = bOp(init, uOp(*first));
            *result = saved;
            saved = init;
            ++result;
        } while (++first != last);
    }
    return result;
}

/// \fn transform_exclusive_scan ( InputIterator1 first1, InputIterator1 last1, InputIterator2 first2, OutputIterator result, ScanOperation scan_op, TransformOperation trans_op, T init )
/// \brief Transforms elements from the input range with uOp and then combines
/// those transformed elements with bOp such that the n-1th element and the nth
/// element are combined. Exclusivity means that the nth element is not
/// included in the nth combination.
/// \return The updated output iterator
///
/// \param first1   The start of the first input sequence
/// \param last1    The end of the first input sequence
/// \param first2   The start of the second input sequence
/// \param result   The output iterator to write the results into
/// \param scan_op  The operation for combining transformed input elements
/// \param trans_op The operation for transforming pairs of input elements
/// \param init     The initial value
template<class InputIterator1, class InputIterator2, class OutputIterator,
         class ScanOperation, class TransformOperation, class T>
BOOST_CXX14_CONSTEXPR
OutputIterator transform_exclusive_scan(InputIterator1 first1, InputIterator1 last1,
                                        InputIterator2 first2,
                                        OutputIterator result, T init,
                                        ScanOperation scan_op, TransformOperation trans_op)
{
    if (first1 != last1)
    {
        T saved = init;
        do
        {
            init = scan_op(init, trans_op(*first1, *first2));
            *result = saved;
            saved = init;
            ++first2;
            ++result;
        } while (++first1 != last1);
    }
    return result;
}

}} // namespace boost and algorithm

#endif // BOOST_ALGORITHM_TRANSFORM_EXCLUSIVE_SCAN_HPP

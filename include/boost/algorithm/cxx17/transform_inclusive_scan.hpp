/*
   Copyright (c) Marshall Clow 2017.

   Distributed under the Boost Software License, Version 1.0. (See accompanying
   file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
*/

/// \file  transform_reduce.hpp
/// \brief Combine the (transformed) elements of a sequence (or two) into a single value.
/// \author Marshall Clow

#ifndef BOOST_ALGORITHM_TRANSFORM_REDUCE_HPP
#define BOOST_ALGORITHM_TRANSFORM_REDUCE_HPP

#include <functional>     // for std::plus
#include <iterator>       // for std::iterator_traits

#include <boost/config.hpp>
#include <boost/range/begin.hpp>
#include <boost/range/end.hpp>
#include <boost/range/value_type.hpp>

namespace boost { namespace algorithm {

/// \fn transform_inclusive_scan ( InputIterator first, InputIterator last, OutputIterator result, BinaryOperation bOp, UnaryOperation uOp, T init )
/// \brief Transforms elements from the input range with uOp and then combines
/// those transformed elements with bOp such that the n-1th element and the nth
/// element are combined. Inclusivity means that the nth element is included in
/// the nth combination.
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
template<class InputIterator, class OutputIterator,
         class BinaryOperation, class UnaryOperation, class T>
BOOST_CXX14_CONSTEXPR
OutputIterator transform_inclusive_scan(InputIterator first, InputIterator last,
                                        OutputIterator result,
                                        BinaryOperation bOp, UnaryOperation uOp,
                                        T init)
{
    for (; first != last; ++first, (void) ++result) {
        init = bOp(init, uOp(*first));
        *result = init;
        }

    return result;
}

/// \fn transform_inclusive_scan ( InputIterator first, InputIterator last, OutputIterator result, BinaryOperation bOp, UnaryOperation uOp, T init )
/// \brief Transforms elements from the input range with uOp and then combines
/// those transformed elements with bOp such that the n-1th element and the nth
/// element are combined. Inclusivity means that the nth element is included in
/// the nth combination. The first value will be used as the init.
/// \return The updated output iterator
///
/// \param first  The start of the input sequence
/// \param last   The end of the input sequence
/// \param result The output iterator to write the results into
/// \param bOp    The operation for combining transformed input elements
/// \param uOp    The operation for transforming input elements
///
/// \note This function is part of the C++17 standard library
template<class InputIterator, class OutputIterator,
         class BinaryOperation, class UnaryOperation>
BOOST_CXX14_CONSTEXPR
OutputIterator transform_inclusive_scan(InputIterator first, InputIterator last,
                                        OutputIterator result,
                                        BinaryOperation bOp, UnaryOperation uOp)
{
    if (first != last) {
        typename std::iterator_traits<InputIterator>::value_type init = uOp(*first);
        *result++ = init;
        if (++first != last)
            return boost::algorithm::transform_inclusive_scan
                                              (first, last, result, bOp, uOp, init);
        }

    return result;
}

/// \fn transform_inclusive_scan ( InputIterator1 first1, InputIterator1 last1, InputIterator2 first2, OutputIterator result, ScanOperation scan_op, TransformOperation trans_op, T init )
/// \brief Transforms elements from the input ranges with trans_op and then
/// combines those transformed elements with scan_op such that the n-1th element
/// and the nth element are combined. Inclusivity means that the nth element is
/// included in the nth combination. The first value will be used as the init.
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
OutputIterator transform_inclusive_scan(InputIterator1 first1, InputIterator1 last1,
                                        InputIterator2 first2,
                                        OutputIterator result,
                                        ScanOperation scan_op, TransformOperation trans_op,
                                        T init)
{
    for (; first1 != last1; ++first1, ++first2, ++result) {
        init = scan_op(init, trans_op(*first1, *first2));
        *result = init;
        }
    return result;
}

}} // namespace boost and algorithm

#endif // BOOST_ALGORITHM_TRANSFORM_REDUCE_HPP

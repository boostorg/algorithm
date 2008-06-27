/*
   Copyright (C) 2008 Jesse Williamson


   Distributed under the Boost Software License, Version 1.0. (See accompanying
   file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

*/

#ifndef BOOST_ALGORITHM_SEQUENCE_TRANSFORM_IF_HPP
 #define BOOST_ALGORITHM_SEQUENCE_TRANSFORM_IF_HPP

#include <boost/range.hpp>

/// \file transform_if.hpp
/// \brief Boost implementation of transform_if() algorithm.
/// \author Jesse Williamson

namespace boost { namespace algorithm { namespace sequence {

/// \fn transform_if ( InputIterator first, InputIterator last, OutputIterator result, UnaryFunction op, UnaryPredicate pred )
/// \brief Applies op to all elements in [first, last] for which pred is true, storing each returned value in the range 
/// beginning at result.
///
/// \param first    The start of the input sequence.
/// \param last     One past the end of the input sequence.
/// \param result   The start of the output sequence.
/// \param op       Unary operation to apply when pred is true.
/// \param pred     Unary condition predicate.
///
  template <class InputIterator, class OutputIterator, class UnaryFunction, class UnaryPredicate>
  OutputIterator transform_if(InputIterator first, InputIterator last, OutputIterator result, UnaryFunction op, UnaryPredicate pred)
  {
	for( ; first != last; ++first, ++result )
	if( pred( *first ) )
	 *result = op( *first );

	return result;
  }

/// \fn transform_if ( ForwardReadableRange& R, OutputIterator result, UnaryFunction op, UnaryPredicate pred )
/// \brief Applies op to all elements in [first, last] for which pred is true, storing each returned value in the range 
/// beginning at result.
///
/// \param R        A forward readable Boost range input sequence.
/// \param result   The start of the output sequence.
/// \param op       Unary operation to apply when pred is true.
/// \param pred     Unary condition predicate.
///
  template <class ForwardReadableRange, class OutputIterator, class UnaryFunction, class UnaryPredicate>
  OutputIterator transform_if(ForwardReadableRange& R, OutputIterator result, UnaryFunction op, UnaryPredicate pred)
  {
	return transform_if( boost::begin(R), boost::end(R), result, op, pred );
  }

/// \fn transform_if ( InputIterator1 first1, InputIterator1 last1, InputIterator2 first2, OutputIterator result, BinaryFunction op, BinaryPredicate pred )
/// \brief Applies op to all elements in [first, last] for which pred is true, storing each returned value in the range 
/// beginning at result.
///
/// \param first1   The start of the first input sequence.
/// \param last1    One past the end of the first input sequence.
/// \param first2   The start of the second input sequence.
/// \param result   The start of the output sequence.
/// \param op       Binary operation to apply when pred is true.
/// \param pred     Binary condition predicate.
///
  template <class InputIterator1, class InputIterator2, class OutputIterator, class BinaryFunction, class BinaryPredicate>
  OutputIterator transform_if(InputIterator1 first1, InputIterator1 last1, InputIterator2 first2, OutputIterator result, BinaryFunction op, BinaryPredicate pred)
 {
	for ( ; first1 != last1; ++first1, ++first2, ++result )
	 if ( pred( *first1, *first2 ) )
	  *result = op( *first1, *first2 );
 }

/// \fn transform_if ( ForwardReadableRange& R, InputIterator2 first2, OutputIterator result, BinaryFunction op, BinaryPredicate pred )
/// \brief Applies op to all elements in [first, last] for which pred is true, storing each returned value in the range 
/// beginning at result.
///
/// \param R        A forward-readable Boost range input sequence.
/// \param first2   The start of the second input sequence.
/// \param result   The start of the output sequence.
/// \param op       Binary operation to apply when pred is true.
/// \param pred     Binary condition predicate.
///
  template <class ForwardReadableRange, class InputIterator2, class OutputIterator, class BinaryFunction, class BinaryPredicate>
  OutputIterator transform_if(ForwardReadableRange& R, InputIterator2 first2, OutputIterator result, BinaryFunction op, BinaryPredicate pred)
  {
	return transform_if( boost::begin(R), boost::end(R), first2, result, op, pred );
  }

}}} // namespace boost::algorithm::sequence

#endif



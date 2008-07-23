/*
   Copyright (C) 2008 Jesse Williamson


   Distributed under the Boost Software License, Version 1.0. (See accompanying
   file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

*/

#ifndef BOOST_ALGORITHM_SEQUENCE_FIND_IF_NOT_HPP
 #define BOOST_ALGORITHM_SEQUENCE_FIND_IF_NOT_HPP

#include <boost/range.hpp>

/// \file find_if_not.hpp
/// \brief Boost implementation of find_if_not() algorithm.
/// \author Jesse Williamson

namespace boost { namespace algorithm { namespace sequence {

/// \fn InputIterator find_if_not ( InputIterator first, InputIterator last, Predicate pred )
/// \brief Returns the first iterator in the range [first, last] for which pred is false. Returns [last] if
/// no such iterator exists.
///
/// \param first    The start of the input sequence.
/// \param last     One past the end of the input sequence.
/// \param pred     Predicate.
///
  template <class InputIterator, class Predicate>
  InputIterator find_if_not ( InputIterator first, InputIterator last, Predicate pred )
  {
	for( ; first != last; ++first )
	if(! pred( *first ) )
	 return first;

	return last;
  }

/// \fn InputIterator find_if_not ( ForwardReadableRange& R, Predicate pred )
/// \brief Returns the first iterator in the range [R] for which pred is false. Returns [R::end] if
/// no such iterator exists.
///
/// \param R        A forward readable Boost range input sequence.
/// \param pred     Predicate.
///
/*
    inline typename boost::range_iterator< ForwardReadableRange >::type
    find( ForwardReadableRange& c, const T& value )
    {
       return std::find( boost::begin( c ), boost::end( c ), value );
    }
*/

  template <class ForwardReadableRange, class Predicate>
  typename boost::range_iterator<ForwardReadableRange>::type find_if_not( ForwardReadableRange& R, Predicate pred )
  {
	return find_if_not( boost::begin(R), boost::end(R), pred );
  }

}}} // boost::algorithm::sequence

#endif

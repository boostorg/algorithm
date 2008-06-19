/* 
   Copyright (c) Marshall Clow 2008.

   Distributed under the Boost Software License, Version 1.0. (See accompanying
   file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

  Revision history:
   19 June 2008 mtc First version - 
*/

#ifndef BOOST_ALGORITHM_SEQUENCE_IOTA_HPP
#define BOOST_ALGORITHM_SEQUENCE_IOTA_HPP

// #include <boost/range.hpp>		// For boost::begin and boost::end

/// \file iota.hpp
/// \brief Boost implementation of iota, a proposed algorithm for TR2
/// \author Marshall Clow

namespace boost { namespace algorithm { namespace sequence {

/// \fn iota ( I first, I last, V value )
/// \brief Generates a sequence (value, value+1, value+2 .. ) and writes the 
///		sequence into [first, last).  The sequence is of length std::distance ( first, last )
/// 
/// \param first The start of the sequence to be output
/// \param last  One past the end of the sequence
/// \param value The starting value for the sequence.
///
///	See <http://www.sgi.com/tech/stl/iota.html> for a more complete description.
///
  template<typename I, typename V>
  void iota ( I first, I last, V value ) 
  {
  	V temp = value;
  	while ( first != last )
  		*first++ = temp++;
  } 


/*
/// \fn iota ( R range, V value )
/// \brief Generates a sequence (v, v+1, v+2 .. ) and writes the sequence into
///		sequence into [first, last).  The sequence is of length std::distance ( first, last )
/// 
/// \param range The range to write the sequence into
/// \param value The starting value for the sequence.
///
///	See <http://www.sgi.com/tech/stl/iota.html> for a more complete description.
///
  template<typename R, typename V>
  void iota ( R range, V value ) 
  {
     iota ( boost::begin ( range ), boost::end ( range ), value );
  } 
*/


/// \fn iota ( O res, std::size_t count, V value )
/// \brief Generates a sequence (v, v+1, v+2 .. ) and puts the sequence into [first, last)
///		The sequence is of length 'count'.
/// 
/// \param res  The iterator two write the resulting sequence into
/// \param count The number of items to write.
/// \param value The starting value for the sequence.
///
///	See <http://www.sgi.com/tech/stl/iota.html> for a more complete description.
///
  template<typename O, typename V>
  void iota ( O res, std::size_t count, V value ) 
  {
  	V temp = value;
  	while ( count-- > 0 )
  		*res++ = temp++;
  }

}}} // namespace boost & algorithm & sequence

#endif // BOOST_ALGORITHM_SEQUENCE_IOTA_HPP

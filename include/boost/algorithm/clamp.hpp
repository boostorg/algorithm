/* 
   Copyright (c) Marshall Clow 2008.

   Distributed under the Boost Software License, Version 1.0. (See accompanying
   file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

 Revision history:
   27 June 2009 mtc First version
   
*/

/// \file clamp.hpp
/// \brief Clamp algorithm
/// \author Marshall Clow
///
/// Suggested by olafvdspek in https://svn.boost.org/trac/boost/ticket/3215

#ifndef BOOST_ALGORITHM_CLAMP_HPP
#define BOOST_ALGORITHM_CLAMP_HPP

namespace boost { namespace algorithm {

/// \fn clamp ( V lo, V hi, V val )
/// \brief Returns the value "val" brought into the range [ lo, hi ]
///          If the value is greater than "hi", return hi. If the value is
///          less than "lo", return lo. Otherwise, return the original value.
/// 
/// \param lo    The low point of the range to be clamped to
/// \param hi    The high point of the range to be clamped to
/// \param val   The value to be clamped
///
  template<typename V> 
  V clamp ( V lo, V hi, V val )
  {
    return val >= hi ? hi : val <= lo ? lo : val;
//  Alternately, 
//  return std::max ( std::min ( val, hi ), lo );
  } 

}}

#endif // BOOST_ALGORITHM_CLAMP_HPP

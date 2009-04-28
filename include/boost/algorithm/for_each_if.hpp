/* 
   Copyright (c) Marshall Clow 2008.

   Distributed under the Boost Software License, Version 1.0. (See accompanying
   file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

  Revision history:
   06 May 2008 mtc First version - as part of BoostCon 2008
*/

#ifndef BOOST_ALGORITHM_FOR_EACH_IF_HPP
#define BOOST_ALGORITHM_FOR_EACH_IF_HPP

#include <boost/range.hpp>		// For boost::begin and boost::end

/// \file for_each_if.hpp
/// \brief Apply a functor to a range when a predicate is satisfied.
/// \author Marshall Clow

namespace boost { namespace algorithm {

/// \fn for_each_if ( InputIterator first, InputIterator last, Pred p, Func f )
/// \brief Applies the function_object 'f' to each element in [first, last) that satisfies
///          the predicate 'p'.
/// 
/// \param first The start of the input sequence
/// \param last  One past the end of the input sequence
/// \param p     A predicate to determine which items to copy
/// \param f     A function object that takes a single argument
/// \return      the function object 'f'
///
///
  template<typename InputIterator, typename Pred, typename Func>
  Func for_each_if ( InputIterator first, InputIterator last, Pred p, Func f )
  {
    while ( first != last )
    {
      if (p(*first)) 
        f(*first);
      ++first;
    }
    return f;
  }

}} // namespace boost & algorithm

#endif // BOOST_ALGORITHM_FOR_EACH_IF_HPP

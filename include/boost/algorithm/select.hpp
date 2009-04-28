/* 
   Copyright (c) Marshall Clow 2008.

   Distributed under the Boost Software License, Version 1.0. (See accompanying
   file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

   Implemented by SGI in their STL implementation
   This implemnetation is based on interprocess::detail::select1st by Ion Gaztanaga.

  Revision history:
   06 May 2008 mtc First version - as part of BoostCon 2008
*/

#ifndef BOOST_ALGORITHM_SELECT_HPP
#define BOOST_ALGORITHM_SELECT_HPP


/// \file select.hpp
/// \brief Choose the first or second element of a Pair.
/// \author Marshall Clow

namespace boost { namespace algorithm {

/// \struct select1st
/// \brief Unary function that returns the first element of a std::pair
/// 
/// \param p     The pair.
/// \return      p.first
///
///
  template <class Pair>
  struct select1st : public std::unary_function<Pair, typename Pair::first_type> 
  {
     const typename Pair::first_type& operator()(const Pair& p) const { return p.first; }
           typename Pair::first_type& operator()(      Pair& p) const { return p.first; }
  };

/// \struct select2nd
/// \brief Unary function that returns the first element of a std::pair
/// 
/// \param p     The pair.
/// \return      p.first
///
///
  template <class Pair>
  struct select2nd : public std::unary_function<Pair, typename Pair::second_type> 
  {
     const typename Pair::second_type& operator()(const Pair& p) const { return p.second; }
           typename Pair::second_type& operator()(      Pair& p) const { return p.second; }
  };

}} // namespace boost & algorithm

#endif // BOOST_ALGORITHM_SELECT_HPP

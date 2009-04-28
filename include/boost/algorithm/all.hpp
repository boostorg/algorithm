/* 
   Copyright (c) Marshall Clow 2008.

   Distributed under the Boost Software License, Version 1.0. (See accompanying
   file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

 Revision history:
   05 May 2008 mtc First version - as part of BoostCon 2008
*/

//	Returns true iff all of the elements in [ first, last ) satisfy the predicate.

#ifndef BOOST_ALGORITHM_ALL_HPP
#define BOOST_ALGORITHM_ALL_HPP

#include <boost/range.hpp>		// For boost::begin and boost::end

/// \file all.hpp
/// \brief Test ranges against predicates.
/// \author Marshall Clow


namespace boost { namespace algorithm {

/// \fn all ( I first, I last, const V &val )
/// \brief Returns true if all elements in [first, last) are equal to 'val'
/// 
/// \param first The start of the input sequence
/// \param last  One past the end of the input sequence
/// \param val   A value to compare against
///
  template<typename I, typename V> 
  bool all ( I first, I last, const V &val )
  {
    while (first != last) {
      if ( *first++ != val ) 
        return false;
    } 
    return true; 
  } 

/// \fn all ( Range range, const V &val )
/// \brief Returns true if all elements in the range are equal to 'val'
/// 
/// \param range The input range
/// \param val   A value to compare against
///
  template<typename Range, typename V> 
  bool all ( Range range, const V &val ) 
  {
    return all ( boost::begin ( range ), boost::end ( range ), val );
  } 


/// \fn all_if ( I first, I last, Pred p )
/// \brief Returns true if all elements in [first, last) satisfy the predicate
/// 
/// \param first The start of the input sequence
/// \param last  One past the end of the input sequence
/// \param p     A predicate
///
template<typename I, typename Pred> 
  bool all_if ( I first, I last, Pred p )
  {
    while (first != last) {
      if ( !p(*first++)) 
        return false;
    } 
    return true; 
  } 

/// \fn all_if ( Range range, Pred p )
/// \brief Returns true if all elements in the range satisfy the predicate
/// 
/// \param range The input range
/// \param p     A predicate to test the elements
///
  template<typename Range, typename Pred> 
  bool all_if ( Range range, Pred p )
  {
    return all_if ( boost::begin ( range ), boost::end ( range ), p );
  } 

/// \fn none ( I first, I last, const V &val )
/// \brief Returns true if none of the elements in [first, last) are equal to 'val'
/// 
/// \param first The start of the input sequence
/// \param last  One past the end of the input sequence
/// \param val   A value to compare against
///
  template<typename I, typename V> 
  bool none ( I first, I last, const V &val ) 
  {
    while (first != last) {
      if ( *first++ == val ) 
        return false;
    } 
    return true; 
  } 

/// \fn none ( Range range, const V &val )
/// \brief Returns true if none of the elements in the range are equal to 'val'
/// 
/// \param range The input range
/// \param val   A value to compare against
///
  template<typename Range, typename V> 
  bool none ( Range range, const V & val ) 
  {
    return none ( boost::begin ( range ), boost::end ( range ), val );
  } 


/// \fn none_if ( I first, I last, Pred p )
/// \brief Returns true if none of the elements in [first, last) satisfy the predicate
/// 
/// \param first The start of the input sequence
/// \param last  One past the end of the input sequence
/// \param p     A predicate
///
template<typename I, typename Pred> 
  bool none_if ( I first, I last, Pred p )
  {
    while (first != last) {
      if ( p(*first++)) 
        return false;
    } 
    return true; 
  } 

/// \fn none_if ( Range range, Pred p )
/// \brief Returns true if none of the elements in the range satisfy the predicate
/// 
/// \param range The input range
/// \param p     A predicate to test the elements
///
  template<typename Range, typename Pred> 
  bool none_if ( Range range, Pred p )
  {
    return none_if ( boost::begin ( range ), boost::end ( range ), p );
  } 

/// \fn any ( I first, I last, const V &val )
/// \brief Returns true if any of the elements in [first, last) are equal to 'val'
/// 
/// \param first The start of the input sequence
/// \param last  One past the end of the input sequence
/// \param val   A value to compare against
///
  template<typename I, typename V> 
  bool any ( I first, I last, const V &val ) 
  {
    while (first != last) {
      if ( *first++ == val ) 
        return true;
    } 
    return false; 
  } 

/// \fn any ( Range range, const V &val )
/// \brief Returns true if any of the elements in the range are equal to 'val'
/// 
/// \param range The input range
/// \param val   A value to compare against
///
  template<typename Range, typename V> 
  bool any ( Range range, const V &val ) 
  {
    return any ( boost::begin ( range ), boost::end ( range ), val );
  } 

/// \fn any_if ( I first, I last, Pred p )
/// \brief Returns true if any of the elements in [first, last) satisfy the predicate
/// 
/// \param first The start of the input sequence
/// \param last  One past the end of the input sequence
/// \param p     A predicate
///
  template<typename I, typename Pred> 
  bool any_if ( I first, I last, Pred p) 
  {
    while (first != last) {
      if ( p(*first++)) 
        return true;
    } 
    return false; 
  } 

/// \fn any_if ( Range range, Pred p )
/// \brief Returns true if any elements in the range satisfy the predicate
/// 
/// \param range The input range
/// \param p     A predicate to test the elements
///
  template<typename Range, typename Pred> 
  bool any_if ( Range range, Pred p )
  {
    return any_if ( boost::begin ( range ), boost::end ( range ), p );
  } 

/// \fn exists_and_only ( I first, I last, const V &val )
/// \brief Returns true if the value 'val' exists only once in [first, last).
/// 
/// \param first The start of the input sequence
/// \param last  One past the end of the input sequence
/// \param val   A value to compare against
///
  template<typename I, typename V> 
  bool exists_and_only ( I first, I last, const V &val )
  {
    I i = std::find (first, last, val);
    if (i == last) return false;
    if ( std::find (++i, last, val) != last) return false;
    return true;
  }

/// \fn exists_and_only ( Range range, const V &val )
/// \brief Returns true if the value 'val' exists only once in the range.
/// 
/// \param range The input range
/// \param val   A value to compare against
///
  template<typename Range, typename V> 
  bool exists_and_only ( Range range, const V &val )
  {
    return exists_and_only ( boost::begin ( range ), boost::end ( range ), val );
  } 

/// \fn exists_and_only_if ( I first, I last, Pred p )
/// \brief Returns true if the predicate 'p' is true for exactly one item in [first, last).
/// 
/// \param first The start of the input sequence
/// \param last  One past the end of the input sequence
/// \param p     A predicate to test the elements
///
  template<typename I, typename Pred> 
  bool exists_and_only_if ( I first, I last, Pred p )
  {
    I i = find_if (first, last, p);
    if (i == last) return false;
    if (find_if(++i, last, p) != last) return false;
    return true;
  }

/// \fn exists_and_only_if ( Range range, Pred p )
/// \brief Returns true if the predicate 'p' is true for exactly one item in the range.
/// 
/// \param range The input range
/// \param p     A predicate to test the elements
///
  template<typename Range, typename Pred> 
  bool exists_and_only_if ( Range range, Pred p ) 
  {
    return exists_and_only_if ( boost::begin ( range ), boost::end ( range ), p );
  } 


}} // namespace boost and algorithm

#endif // BOOST_ALGORITHM_ALL_HPP

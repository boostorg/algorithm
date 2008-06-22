/* 
   Copyright (c) Marshall Clow 2008.

   Distributed under the Boost Software License, Version 1.0. (See accompanying
   file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

  Revision history:
   05 May 2008 mtc First version - as part of BoostCon 2008
*/

#ifndef BOOST_ALGORITHM_SEQUENCE_COPY_HPP
#define BOOST_ALGORITHM_SEQUENCE_COPY_HPP

#include <boost/range.hpp>		// For boost::begin and boost::end
#include <iterator>				// for std::iterator_traits<>

/// \file copy.hpp
/// \brief Boost implementation of various STL-type copying algorithms
///         that were left out of the standard.
/// \author Marshall Clow

namespace boost { namespace algorithm { namespace sequence {

/// \fn copy_if ( I first, I last, O res, Pred p )
/// \brief Copies all the elements from [first, last) that satisfy the predicate into 'res'
/// 
/// \param first The start of the input sequence
/// \param last  One past the end of the input sequence
/// \param res   An output iterator to copy into
/// \param p     A predicate to determine which items to copy
/// \return      The (modified) output iterator
///
/// \note        Based on a suggested implementation by Bjorne Stoustrop.
///
  template<typename I, typename O, typename Pred> 
  O copy_if ( I first, I last, O res, Pred p) 
  {
    while (first != last)
    {
      if (p(*first)) 
        *res++ = *first; 
      ++first; 
    } 
    return res; 
  } 

/// \fn copy_if ( Range range, O res, Pred p )
/// \brief Copy all the elements from the range that satisfy the predicate into 'res'
/// 
/// \param range The input range
/// \param res   An output iterator to copy into
/// \param p     A predicate to determine which items to copy
/// \return      The (modified) output iterator
///
  template<typename Range, typename O, typename Pred> 
  O copy_if ( Range range, O res, Pred p ) 
  {
    return copy_if ( boost::begin ( range ), boost::end ( range ), res, p );
  } 
  

/// \fn copy_backward_if ( I first, I last, O res, Pred p )
/// \brief Copies all the elements from (last, first] that satisfy the predicate into 'res'
/// 
/// \param first The start of the input sequence
/// \param last  One past the end of the input sequence
/// \param res   An output iterator to copy into
/// \param p     A predicate to determine which items to copy
/// \return      The (modified) output iterator
///
/// \note        Based on a suggested implementation by Bjorne Stoustrop.
///
  template<typename I, typename O, typename Pred> 
  O copy_backward_if ( I first, I last, O res, Pred p) 
  {
    while (first != last)
    {
      if (p(*--last)) 
        *res++ = *last; 
    } 
    return res; 
  } 

/// \fn copy_backward_if ( Range range, O res, Pred p )
/// \brief Copy all the elements from the range that satisfy the predicate into 'res'
/// 
/// \param range The input range
/// \param res   An output iterator to copy into
/// \param p     A predicate to determine which items to copy
/// \return      The (modified) output iterator
///
  template<typename Range, typename O, typename Pred> 
  O copy_backward_if ( Range range, O res, Pred p ) 
  {
    return copy_backward_if ( boost::begin ( range ), boost::end ( range ), res, p );
  } 


/* -- I'd like a value-based version, too; but I don't know of a good name....
template<typename I,typename O>
  O copy_equal ( I first, I last, O res, I::value_type val ) 
  {
    while (first != last) {
      if (*first == val)
        *res++ = *first; 
      ++first; 
    } 
    return res; 
  }
*/

    
/// \fn copy_while ( I first, I last, O res, Pred p )
/// \brief Copies all the elements from [first, last) up to a point into 'res'.\n
///      Will continue until the input range is exhausted, or the predicate 'p' fails.
///
/// \param first The start of the input sequence
/// \param last  One past the end of the input sequence
/// \param res   An output iterator to copy into
/// \param p     A predicate to determine when to stop copying
/// \return      The (modified) output iterator
///
  template<typename I, typename O, typename Pred>
  O copy_while ( I first, I last, O res, Pred p )
  {
    for (; first != last && p(*first); ++first)
        *res++ = *first;
    return res;
  }

/// \fn copy_while ( Range range, O res, Pred p )
/// \brief Copies all the elements from the range up to a point into 'res'.\n
///    Will continue until the input range is exhausted, or the predicate 'p' fails.
/// 
/// \param range The input range
/// \param res   An output iterator to copy into
/// \param p     A predicate to determine when to stop copying
/// \return      The (modified) output iterator
///
  template<typename Range, typename O, typename Pred> 
  O copy_while ( Range range, O res, Pred p ) 
  {
    return copy_while ( boost::begin ( range ), boost::end ( range ), res, p );
  } 

                      
/// \fn copy_backward_while ( I first, I last, O res, Pred p )
/// \brief Copies all the elements from (last, first] up to a point into 'res'.\n
///      Will continue until the input range is exhausted, or the predicate 'p' fails.
///
/// \param first The start of the input sequence
/// \param last  One past the end of the input sequence
/// \param res   An output iterator to copy into
/// \param p     A predicate to determine when to stop copying
/// \return      The (modified) output iterator
///
  template<typename I, typename O, typename Pred>
  O copy_backward_while ( I first, I last, O res, Pred p )
  {
    while ( first != last && p ( *--last ))
        *res++ = *last;
    return res;
  }

/// \fn copy_backward_while ( Range range, O res, Pred p )
/// \brief Copies all the elements from the range up to a point into 'res'.\n
///    Will continue until the input range is exhausted, or the predicate 'p' fails.
/// 
/// \param range The input range
/// \param res   An output iterator to copy into
/// \param p     A predicate to determine when to stop copying
/// \return      The (modified) output iterator
///
  template<typename Range, typename O, typename Pred> 
  O copy_backward_while ( Range range, O res, Pred p ) 
  {
    return copy_backward_while ( boost::begin ( range ), boost::end ( range ), res, p );
  } 

                      
// According to Werner Salomon, the challenge for copy_n is that the algorithm should work correctly
// with an std::istream_iterator. Therefor the input iterator have to increment only N-1 times.
// Marshall sez: Is that really true?
// Long discussion starting 
//    here <http://www.tech-archive.net/Archive/VC/microsoft.public.vc.stl/2004-05/0112.html>
// Marshall sez: I'm going to increment N times.
//
// Marshall sez: What's the advantage of templatizing on count, rather than using std::size_t?
// 
// No range-based version here

/// \fn copy_n ( I first, typename iterator_traits<I>::difference_type count, O res )
/// \brief Copies n elements starting at 'first' into 'res'.
///
/// \param first The start of the input sequence
/// \param count The number of elements to copy
/// \param res   An output iterator to copy into
/// \return      The (modified) output iterator
///
//  template <typename I, typename Size, typename O>
//  O copy_n ( I first, Size count, O res )
  template <typename I, typename O>
  O copy_n ( I first, typename std::iterator_traits<I>::difference_type count, O res )
  {
    while ( count-- > 0 )
      *res++ = *first++;
    return res;
  }

//	Range-based versions of copy and copy_backwards.

/// \fn copy ( Range range, O res )
/// \brief Copies elements from the range 'range' into 'res'.
///
/// \param range The input range
/// \param res   An output iterator to copy into
/// \return      The (modified) output iterator
///
/// \note A range-based version of std::copy
///
  template<typename Range, typename O> 
  O copy ( Range range, O res ) 
  {
    return std::copy ( boost::begin ( range ), boost::end ( range ), res );
  } 
  
  
/// \fn copy_backward ( Range range, O res )
/// \brief Copies elements from the range 'range' into 'res'.
///
/// \param range The input range
/// \param res   An output iterator to copy into
/// \return      The (modified) output iterator
///
/// \note A range-based version of std::copy_backwards
///
  template<typename Range, typename O> 
  O copy_backward ( Range range, O res ) 
  {
    return std::copy_backward ( boost::begin ( range ), boost::end ( range ), res );
  } 

}}} // namespace boost & algorithm & sequence

#endif // BOOST_ALGORITHM_SEQUENCE_COPY_HPP

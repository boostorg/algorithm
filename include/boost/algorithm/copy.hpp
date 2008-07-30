/* 
   Copyright (c) Marshall Clow 2008.

   Distributed under the Boost Software License, Version 1.0. (See accompanying
   file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

  Revision history:
   05 May 2008 mtc First version - as part of BoostCon 2008
   07 Jul 2008 mtc Added more algorithms proposed by Matt Austern as part of C++Ox
       <http://www.open-std.org/jtc1/sc22/wg21/docs/papers/2008/n2666.pdf>
   
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
  

/// \fn reverse_copy_if ( I first, I last, O res, Pred p )
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
  O reverse_copy_if ( I first, I last, O res, Pred p) 
  {
    while (first != last)
    {
      if (p(*--last)) 
        *res++ = *last; 
    } 
    return res; 
  } 

/// \fn reverse_copy_if ( Range range, O res, Pred p )
/// \brief Copy all the elements from the range that satisfy the predicate into 'res'
/// 
/// \param range The input range
/// \param res   An output iterator to copy into
/// \param p     A predicate to determine which items to copy
/// \return      The (modified) output iterator
///
  template<typename Range, typename O, typename Pred> 
  O reverse_copy_if ( Range range, O res, Pred p ) 
  {
    return reverse_copy_if ( boost::begin ( range ), boost::end ( range ), res, p );
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
  std::pair<I,O> copy_while ( I first, I last, O res, Pred p )
  {
    for (; first != last && p(*first); ++first)
        *res++ = *first;
    return std::make_pair ( first, res );
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
  std::pair<I,O> copy_while ( Range range, O res, Pred p ) 
  {
    return copy_while ( boost::begin ( range ), boost::end ( range ), res, p );
  } 

                      
/// \fn reverse_copy_while ( I first, I last, O res, Pred p )
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
  std::pair<I,O> reverse_copy_while ( I first, I last, O res, Pred p )
  {
    while ( first != last && p ( *--last ))
        *res++ = *last;
    return std::make_pair ( last, res );
  }

/// \fn reverse_copy_while ( Range range, O res, Pred p )
/// \brief Copies all the elements from the range up to a point into 'res'.\n
///    Will continue until the input range is exhausted, or the predicate 'p' fails.
/// 
/// \param range The input range
/// \param res   An output iterator to copy into
/// \param p     A predicate to determine when to stop copying
/// \return      The (modified) output iterator
///
  template<typename Range, typename O, typename Pred> 
  std::pair<I,O> reverse_copy_while ( Range range, O res, Pred p ) 
  {
    return reverse_copy_while ( boost::begin ( range ), boost::end ( range ), res, p );
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
  template <typename I, typename Size, typename O>
  O copy_n ( I first, Size count, O res )
//  template <typename I, typename O>
//  O copy_n ( I first, typename std::iterator_traits<I>::difference_type count, O res )
  {
    for ( ; count > 0; ++res, ++first, --count )
      *res = *first;
    return res;
  }

/// \fn uninitialized_copy_n ( I first, Size count, O res )
/// \brief xxx
///
/// \param first The start of the input sequence
/// \param count The number of elements to copy
/// \param res   An output iterator to copy into
/// \return      The (modified) output iterator
///
  template<typename I, typename Size, typename O> 
  O uninitialized_copy_n ( I first, Size count, O res ) 
  {
    typedef typename std::iterator_traits<I>::value_type vt;
    for ( ; count > 0; ++res, ++first, --count )
        new ( static_cast <void*> ( &*res )) vt (*first); 
    return res;
  } 

//	Range-based versions of copy and copy_backward.

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
/// \note A range-based version of std::copy_backward
///
  template<typename Range, typename O> 
  O copy_backward ( Range range, O res ) 
  {
    return std::copy_backward ( boost::begin ( range ), boost::end ( range ), res );
  } 


/// \fn partition_copy ( I first, I last, O1 out_true, O2 out_false, Pred pred )
/// \brief Copies each element from [first, last) into one of the two output sequences,
///		depending on the result of the predicate
///
/// \param first       The start of the input sequence
/// \param last        One past the end of the input sequence
/// \param out_true    An output iterator to copy into
/// \param out_false   An output iterator to copy into
/// \param p           A predicate to determine which output sequence to copy into.
///
///
  template <typename I, typename O1, typename O2, typename Pred>
  std::pair <O1, O2> partition_copy ( I first, I last, O1 out_true, O2 out_false, Pred pred )
  {
    while (first != last)
    {
      if (p(*first)) 
        *out_true++  = *first++;
      else
        *out_false++ = *first++;
    } 
  
    return std::make_pair ( out_true, out_false );
  }
  

/// \fn partition_copy ( I first, I last, O1 out_true, O2 out_false, Pred pred )
/// \brief Copies each element from the range into one of the two output sequences,
///		depending on the result of the predicate
///
/// \param range       The input range
/// \param out_true    An output iterator to copy into
/// \param out_false   An output iterator to copy into
/// \param p           A predicate to determine which output sequence to copy into.
///
  template <typename Range, typename O1, typename O2, typename Pred>
  std::pair <O1, O2> partition_copy ( Range range, O1 out_true, O2 out_false, Pred pred )
  {
    return partition_copy ( boost::begin ( range ), boost::end ( range ), out_true, out_false, pred );
  }

}}} // namespace boost & algorithm & sequence

#endif // BOOST_ALGORITHM_SEQUENCE_COPY_HPP

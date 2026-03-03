/* 
   Copyright (c) Denis Mikhailov 2022.

   Distributed under the Boost Software License, Version 1.0. (See accompanying
   file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
*/

/// \file  move_if.hpp
/// \brief Move a subset of a sequence to a new sequence
/// \author Denis Mikhailov

#ifndef BOOST_ALGORITHM_MOVE_IF_HPP
#define BOOST_ALGORITHM_MOVE_IF_HPP

#include <utility>    // for std::pair, std::make_pair, std::move

#include <boost/config.hpp>
#include <boost/range/begin.hpp>
#include <boost/range/end.hpp>

namespace boost { namespace algorithm {

/// \fn move_while ( InputIterator first, InputIterator last, OutputIterator d_first, Predicate p )
/// \brief Moves all the elements at the start of the range that
///     satisfy the predicate to the another range.
/// \return Updated two iterators for the first and the second range
/// 
/// \param first    The start of the range to move
/// \param last     One past the end of the range to move
/// \param d_first  The start of the destination range
/// \param p        A predicate for testing the elements of the range
/// \note           C++11-compatible compiler required.
template<typename InputIterator, typename OutputIterator, typename Predicate> 
BOOST_CXX14_CONSTEXPR std::pair<InputIterator, OutputIterator>
move_while ( InputIterator first, InputIterator last, OutputIterator d_first, Predicate p )
{
    while (first != last && p(*first))
        *d_first++ = std::move(*first++);
    return std::make_pair(first, d_first);
}

/// \fn move_while ( const Range &r, OutputIterator d_first, Predicate p )
/// \brief Moves all the elements at the start of the range that
///     satisfy the predicate to the another range.
/// \return Updated two iterators for the first and the second range
/// 
/// \param r        The range to move
/// \param d_first  The start of the destination range
/// \param p        A predicate for testing the elements of the range
/// \note           C++11-compatible compiler required.
template<typename Range, typename OutputIterator, typename Predicate>
BOOST_CXX14_CONSTEXPR std::pair<typename boost::range_iterator<Range>::type, OutputIterator> 
move_while ( Range &r, OutputIterator d_first, Predicate p )
{
    return boost::algorithm::move_while (boost::begin (r), boost::end(r), d_first, p);
}


/// \fn move_until ( InputIterator first, InputIterator last, OutputIterator d_first, Predicate p )
/// \brief Moves all the elements at the start of the range that do not
///     satisfy the predicate to the another range.
/// \return Updated two iterators for the first and the second range
/// 
/// \param first    The start of the range to move
/// \param last     One past the end of the range to move
/// \param d_first  The start of the destination range
/// \param p        A predicate for testing the elements of the range
/// \note           C++11-compatible compiler required.
template<typename InputIterator, typename OutputIterator, typename Predicate> 
BOOST_CXX14_CONSTEXPR std::pair<InputIterator, OutputIterator>
move_until ( InputIterator first, InputIterator last, OutputIterator d_first, Predicate p )
{
    while (first != last && !p(*first))
        *d_first++ = std::move(*first++);
    return std::make_pair(first, d_first);
}

/// \fn move_until ( const Range &r, OutputIterator d_first, Predicate p )
/// \brief Moves all the elements at the start of the range that do not
///     satisfy the predicate to the another range.
/// \return Updated two iterators for the first and the second range
/// 
/// \param r        The range to move
/// \param d_first  The start of the destination range
/// \param p        A predicate for testing the elements of the range
/// \note           C++11-compatible compiler required.
template<typename Range, typename OutputIterator, typename Predicate>
BOOST_CXX14_CONSTEXPR std::pair<typename boost::range_iterator<Range>::type, OutputIterator> 
move_until ( Range &r, OutputIterator d_first, Predicate p )
{
    return boost::algorithm::move_until (boost::begin (r), boost::end(r), d_first, p);
}

}} // namespace boost and algorithm

#endif  // BOOST_ALGORITHM_MOVE_IF_HPP

/*

 Copyright (C) Jesse Williamson 2008.
	
 Distributed under the Boost Software License, Version 1.0. (See accompanying
 file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

*/

#ifndef BOOST_ALGORITHM_APPLY_HPP
#define BOOST_ALGORITHM_APPLY_HPP

/// \file apply.hpp
/// \brief Apply predicates to ranges.
/// \author Jesse Williamson

#include <boost/range.hpp>

namespace boost { namespace algorithm { namespace sequence {

/// \fn apply_if (ForwardIter begin, ForwardIter end, Mutator m, ConditionPredicate c)
/// \brief Applies m to all elements from (begin, end) for which c is true.
///
/// \param begin The start of the sequence to modify
/// \param end   One past the end of the input sequence
/// \param m     Unary mutator function object
/// \param c     Unary condition function object
/// \return      The mutator function object
///
template <class ForwardIter, class Mutator, class ConditionPredicate>
Mutator apply_if(ForwardIter begin, ForwardIter end, Mutator m, ConditionPredicate c)
{
 for (; begin != end; begin++) {
    if (c(*begin))
      *begin = m(*begin);
 }
  
 return m; 
}

/// \fn apply_if (ForwardReadableRange& r, Mutator m, ConditionPredicate c)
/// \brief Applies m to all elements in the range r for which c is true.
///
/// \param r     The range to modify
/// \param m     Unary mutator function object
/// \param c     Unary condition function object
/// \return      The mutator function object
///
template <class ForwardReadableRange, class Mutator, class ConditionPredicate>
Mutator apply_if(ForwardReadableRange& R, Mutator m, ConditionPredicate c)
{
 return apply_if(boost::begin(R), boost::end(R), m, c);
}

/// \fn apply (ForwardIter begin, ForwardIter end, Mutator m)
/// \brief Applies m to all elements from (begin, end).
///
/// \param begin The start of the sequence to modify
/// \param end   One past the end of the input sequence
/// \param m     Unary mutator function object
/// \return      The mutator function object
///
template <class ForwardIter, class Mutator>
Mutator apply(ForwardIter begin, ForwardIter end, Mutator m)
{
 for (; begin != end; begin++) 
    *begin = m(*begin);
  
 return m;
}

/// \fn apply (ForwardReadableRange& r, Mutator m)
/// \brief Applies m to all elements in the range r
///
/// \param r     The range to modify
/// \param m     Unary mutator function object
/// \return      The mutator function object
///
template <class ForwardReadableRange, class Mutator>
Mutator apply(ForwardReadableRange& R, Mutator m)
{
 return apply(boost::begin(R), boost::end(R), m);
}

}}} // namespace boost::algorithm::sequence

#endif // BOOST_ALGORITHM_APPLY_HPP

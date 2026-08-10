/*
   Copyright (c) Jonathan Gopel 2022.

   Distributed under the Boost Software License, Version 1.0. (See accompanying
   file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
*/

#ifndef BOOST_ALGORITHM_ITERATE_HPP
#define BOOST_ALGORITHM_ITERATE_HPP

#include <boost/config.hpp>
#include <boost/range/begin.hpp>
#include <boost/range/end.hpp>

#include <utility>

namespace boost {
namespace algorithm {

/// \fn iterate ( ForwardIterator first, ForwardIterator last, TInit init, UnaryOperation op )
/// \brief Fills a range by applying op to the previous result of op. The nth
/// element of the range is calculated by applying op to init n times. IE: The
/// 0th element is init, the 1st element is op(init), the 2nd element is
/// op(op(init)) and so on.
/// \return The final value of init
///
/// \param first  The start of the input range
/// \param last   The end of the input range
/// \param init   The initial value of the output range
/// \param op     The function to apply to generate subsequent elements of the
///               range. Note that the input type and output type of this
///               function must be TInit.
template <class ForwardIterator, class TInit, class UnaryOperation>
BOOST_CXX14_CONSTEXPR TInit iterate(ForwardIterator first, ForwardIterator last,
                                    TInit init,
                                    UnaryOperation op) // TODO: BOOST_NOEXCEPT?
{
  for (; first != last; (void)++first, init = op(init)) {
    *first = init;
  }
  return init;
}

/// \fn iterate ( Range& r, TInit init, UnaryOperation op )
/// \brief Fills a range by applying op to the previous result of op. The nth
/// element of the range is calculated by applying op to init n times. IE: The
/// 0th element is init, the 1st element is op(init), the 2nd element is
/// op(op(init)) and so on.
/// \return The final value of init
///
/// \param first  The start of the input range
/// \param last   The end of the input range
/// \param init   The initial value of the output range
/// \param op     The function to apply to generate subsequent elements of the
///               range. Note that the input type and output type of this
///               function must be TInit.
template <class Range, class TInit, class UnaryOperation>
BOOST_CXX14_CONSTEXPR TInit iterate(Range &r, TInit init, UnaryOperation op) {
  return iterate(boost::begin(r), boost::end(r), init, op);
}

/// \fn iterate ( ForwardIterator first, Size n, TInit init, UnaryOperation op )
/// \brief Fills n elements of a range by applying op to the previous result
/// of op. The nth element of the range is calculated by applying op to init
/// n times. IE: The 0th element is init, the 1st element is op(init), the 2nd
/// element is op(op(init)) and so on.
/// \return The updated output iterator and the final value of init
///
/// \param first  The start of the input range
/// \param n      The number of elements to fill
/// \param init   The initial value of the output range
/// \param op     The function to apply to generate subsequent elements of the
///               range. Note that the input type and output type of this
///               function must be TInit.
template <class OutputIterator, class Size, class TInit, class UnaryOperation>
BOOST_CXX14_CONSTEXPR std::pair<OutputIterator, TInit>
iterate(OutputIterator first, Size n, TInit init,
        UnaryOperation op) // TODO: BOOST_NOEXCEPT?
{
  for (; n > 0; --n, (void)++first, init = op(init)) {
    *first = init;
  }
  return std::make_pair(first, init);
}

} // namespace algorithm
} // namespace boost

#endif // BOOST_ALGORITHM_ITERATE_HPP

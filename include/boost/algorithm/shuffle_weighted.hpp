/*
  Copyright (c) Alexander Zaitsev <zamazan4ik@gmail.com>, 2017

  Distributed under the Boost Software License, Version 1.0. (See
  accompanying file LICENSE_1_0.txt or copy at
  http://www.boost.org/LICENSE_1_0.txt)

  See http://www.boost.org/ for latest version.


  Based on: Weighted Random Sampling (2005; Efraimidis, Spirakis)
  http://utopia.duth.gr/~pefraimi/research/data/2007EncOfAlg.pdf
  
*/

/// \file  shuffle_weighted.hpp
/// \brief Weighted shuffle.
/// \author Alexander Zaitsev

#ifndef BOOST_ALGORITHM_SHUFFLE_WEIGHTED_HPP
#define BOOST_ALGORITHM_SHUFFLE_WEIGHTED_HPP

#include <algorithm>
#include <random>
#include <type_traits>
#include <cmath>

#include "/home/zamazan4ik/OpenSource/apply_permutation/apply_permutation.hpp"

#include <boost/range/begin.hpp>
#include <boost/range/end.hpp>

namespace boost { namespace algorithm {

/// \fn shuffle_weighted (RandomAccessIterator1 item_begin, RandomAccessIterator1 item_end, RandomAccessIterator2 weight_begin, UniformRandomBitGenerator&& g)
/// \brief Rearranges the elements in the range [item_begin,item_end) randomly with weights from weight_begin range, using g as uniform random number generator.
///
/// \param item_begin    The start of the item sequence
/// \param item_end		 One past the end of the item sequence
/// \param weight_begin  The start of the weight sequence.
/// \param g             Uniform random number generator
///
/// \note Weight sequence size should be equal to item size. Otherwise behavior is undefined.
///       Complexity: O(N).
template<typename RandomAccessIterator1, typename RandomAccessIterator2, typename UniformRandomBitGenerator>
void shuffle_weighted(RandomAccessIterator1 item_begin, RandomAccessIterator1 item_end, RandomAccessIterator2 weight_begin, UniformRandomBitGenerator&& g)
{
    using Diff = typename std::iterator_traits<RandomAccessIterator1>::difference_type;
    std::vector<std::pair<double, Diff>> order
            (std::distance(item_begin, item_end));
    std::uniform_real_distribution<double> random(0, 1.0);
    size_t index = 0;
    for (auto& x : order)
    {
        x = std::pair<double, Diff>(-std::pow(random(g), 1.0 / weight_begin[index]), index);
        ++index;
    }
    //TODO: Is there reason use Boost.Sort here?
    std::sort(order.begin(), order.end());
    std::vector<Diff> res_order;
    res_order.reserve(order.size());
    for (const auto& x : order)
    {
        res_order.push_back(x.second);
    }

    boost::algorithm::apply_permutation(item_begin, item_end, res_order.begin());
}

/// \fn shuffle_weighted (Range1& item_range, Range2& weight_range, UniformRandomBitGenerator&& g )
/// \brief Rearranges the elements in the range [item_begin,item_end) randomly with weights from weight_begin range, using g as uniform random number generator.
///
/// \param item_range    The item sequence
/// \param weight_range  The weight sequence
/// \param g             Uniform random number generator
///
/// \note Weight sequence size should be equal to item size. Otherwise behavior is undefined.
///       Complexity: O(N).
template<typename Range1, typename Range2, typename UniformRandomBitGenerator>
void shuffle_weighted(Range1& item_range, Range2& weight_range, UniformRandomBitGenerator&& g)
{
    shuffle_weighted(boost::begin(item_range), boost::end(item_range),
                     boost::begin(weight_range), g);
}

}}
#endif //BOOST_ALGORITHM_SHUFFLE_WEIGHTED_HPP

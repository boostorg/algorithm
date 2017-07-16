/*
  Copyright (c) Alexander Zaitsev <zamazan4ik@gmail.com>, 2017

  Distributed under the Boost Software License, Version 1.0. (See
  accompanying file LICENSE_1_0.txt or copy at
  http://www.boost.org/LICENSE_1_0.txt)

  See http://www.boost.org/ for latest version.
  
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
///       Complexity: O(N^2).
template <class ForwardIterator1, class ForwardIterator2, class UniformRandomBitGenerator>
void shuffle_weighted(ForwardIterator1 item_begin, ForwardIterator1 item_end, 
                      ForwardIterator2 weight_begin, UniformRandomBitGenerator&& g)
{
    using weight_t = typename std::iterator_traits<ForwardIterator2>::value_type;
    
    weight_t total_weight = 0;
    auto weight_iter = weight_begin;
    for(auto it = item_begin; it != item_end; 
        it = std::next(it), 
        weight_iter = std::next(weight_iter))
    {
        total_weight += *weight_iter;
    }

    using uniform_distr_t = std::conditional_t<
        std::is_integral<weight_t>::value,
        std::uniform_int_distribution<weight_t>,
        std::uniform_real_distribution<weight_t>
    >;
    typedef typename uniform_distr_t::param_type param_type;

    uniform_distr_t distribution;
    for (; item_begin != item_end; 
         item_begin = std::next(item_begin), 
         weight_begin = std::next(weight_begin)) 
    {
        weight_t current_weights_sum = 0;
        const weight_t random_value = distribution(g, param_type(0, total_weight));

        auto weight_iter = weight_begin;
        for (auto it = item_begin; it != item_end; 
             it = std::next(it), 
             weight_iter = std::next(weight_iter))
        {
            const weight_t weight = *weight_iter;
            current_weights_sum += weight;
            if (current_weights_sum >= random_value)
            {
                std::iter_swap(item_begin, it);
                std::iter_swap(weight_begin, weight_iter);
                total_weight -= weight;
                break;
            }
        }
    }
}


/// \fn shuffle_weighted (Range1& item_range, Range2& weight_range, UniformRandomBitGenerator&& g )
/// \brief Rearranges the elements in the range [item_begin,item_end) randomly with weights from weight_begin range, using g as uniform random number generator.
///
/// \param item_range    The item sequence
/// \param weight_range  The weight sequence
/// \param g             Uniform random number generator
///
/// \note Weight sequence size should be equal to item size. Otherwise behavior is undefined.
///       Complexity: O(N^2).
template <class Range1, class Range2, class UniformRandomBitGenerator>
void shuffle_weighted(Range1 item_range, Range2 weight_range, UniformRandomBitGenerator&& g) 
{
    shuffle_weighted(boost::begin(item_range), boost::end(item_range), boost::begin(weight_range), std::forward<UniformRandomBitGenerator>(g));
}

}}
#endif //BOOST_ALGORITHM_SHUFFLE_WEIGHTED_HPP

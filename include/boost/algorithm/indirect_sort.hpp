/* 
   Copyright (c) Marshall Clow 2023.

   Distributed under the Boost Software License, Version 1.0. (See accompanying
   file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
  
*/

/// \file indirect_sort.hpp
/// \brief indirect sorting algorithms
/// \author Marshall Clow
///

#ifndef BOOST_ALGORITHM_IS_INDIRECT_SORT
#define BOOST_ALGORITHM_IS_INDIRECT_SORT

#include <algorithm>        // for std::sort (and others)
#include <functional>       // for std::less
#include <vector>           // for std:;vector

#include <boost/algorithm/cxx11/iota.hpp>

namespace boost { namespace algorithm {

namespace detail {

	template <class Predicate, class Iter>
	struct indirect_predicate {
		indirect_predicate (Predicate pred, Iter iter)
		: pred_(pred), iter_(iter) {}
	
		bool operator ()(size_t a, size_t b) const {
			 return pred_(iter_[a], iter_[b]);
		}
		
		Predicate pred_;
		Iter      iter_;
	};

}

typedef std::vector<size_t> Permutation;

                    // ===== sort =====

/// \fn indirect_sort (RAIterator first, RAIterator last, Predicate p)
/// \returns a permutation of the elements in the range [first, last)
///     such that when the permutation is applied to the sequence,
///     the result is sorted according to the predicate pred.
///
/// \param first  The start of the input sequence
/// \param last   The end of the input sequence
/// \param pred   The predicate to compare elements with
///
template <typename RAIterator, typename Pred>
std::vector<size_t> indirect_sort (RAIterator first, RAIterator last, Pred pred) {
	Permutation ret(std::distance(first, last));
	boost::algorithm::iota(ret.begin(), ret.end(), size_t(0));
	std::sort(ret.begin(), ret.end(), 
	                  detail::indirect_predicate<Pred, RAIterator>(pred, first));
	return ret;
}

/// \fn indirect_sort (RAIterator first, RAIterator las )
/// \returns a permutation of the elements in the range [first, last)
///     such that when the permutation is applied to the sequence,
///     the result is sorted according to the predicate pred.
///
/// \param first  The start of the input sequence
/// \param last   The end of the input sequence
///
template <typename RAIterator>
std::vector<size_t> indirect_sort (RAIterator first, RAIterator last) {
    return indirect_sort(first, last, 
                    std::less<typename std::iterator_traits<RAIterator>::value_type>());
}

                    // ===== stable_sort =====
                    // ===== partial_sort =====
                    // ===== nth_element =====
}}

#endif // BOOST_ALGORITHM_IS_INDIRECT_SORT

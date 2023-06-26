/* 
   Copyright (c) Marshall Clow 2023.

   Distributed under the Boost Software License, Version 1.0. (See accompanying
   file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
  
*/

/// \file indirect_sort.hpp
/// \brief indirect sorting algorithms
/// \author Marshall Clow
///

#ifndef BOOST_ALGORITHM_INDIRECT_SORT
#define BOOST_ALGORITHM_INDIRECT_SORT

#include <algorithm>        // for std::sort (and others)
#include <functional>       // for std::less
#include <vector>           // for std::vector

namespace boost { namespace algorithm {

typedef std::vector<size_t> Permutation;

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

	// Initialize a permutation
	// it would be nice to use 'iota' here, but that call writes over
	// existing elements - not append them.  I don't want to initialize
	// the elements of the permutation to zero, and then immediately
	// overwrite them.
	void init_permutation (Permutation &p, size_t size) {
		p.reserve(size);
		for (size_t i = 0; i < size; ++i)
			p.push_back(i);
	}
}

                    // ===== sort =====

/// \fn indirect_sort (RAIterator first, RAIterator last, Predicate pred)
/// \returns a permutation of the elements in the range [first, last)
///     such that when the permutation is applied to the sequence,
///     the result is ordered as if 'std::sort(first, last, pred)'
//      was called on the sequence.
///
/// \param first  The start of the input sequence
/// \param last   The end of the input sequence
/// \param pred   The predicate to compare elements with
///
template <typename RAIterator, typename Pred>
Permutation indirect_sort (RAIterator first, RAIterator last, Pred pred) {

	Permutation ret;
	detail::init_permutation(ret, std::distance(first, last));
	std::sort(ret.begin(), ret.end(), 
	                  detail::indirect_predicate<Pred, RAIterator>(pred, first));
	return ret;
}

/// \fn indirect_sort (RAIterator first, RAIterator last)
/// \returns a permutation of the elements in the range [first, last)
///     such that when the permutation is applied to the sequence,
///     the result is ordered as if 'std::sort(first, last)'
//      was called on the sequence.
///
/// \param first  The start of the input sequence
/// \param last   The end of the input sequence
///
template <typename RAIterator>
Permutation indirect_sort (RAIterator first, RAIterator last) {
    return indirect_sort(first, last, 
                    std::less<typename std::iterator_traits<RAIterator>::value_type>());
}

                    // ===== stable_sort =====

/// \fn indirect_stable_sort (RAIterator first, RAIterator last, Predicate pred)
/// \returns a permutation of the elements in the range [first, last)
///     such that when the permutation is applied to the sequence,
///     the result is ordered as if 'std::stable_sort(first, last, pred)'
//      was called on the sequence.
///
/// \param first  The start of the input sequence
/// \param last   The end of the input sequence
/// \param pred   The predicate to compare elements with
///
template <typename RAIterator, typename Pred>
Permutation indirect_stable_sort (RAIterator first, RAIterator last, Pred pred) {
	Permutation ret;
	detail::init_permutation(ret, std::distance(first, last));
	std::stable_sort(ret.begin(), ret.end(), 
	                  detail::indirect_predicate<Pred, RAIterator>(pred, first));
	return ret;
}

/// \fn indirect_stable_sort (RAIterator first, RAIterator last)
/// \returns a permutation of the elements in the range [first, last)
///     such that when the permutation is applied to the sequence,
///     the result is ordered as if 'std::stable_sort(first, last)'
//      was called on the sequence.
///
/// \param first  The start of the input sequence
/// \param last   The end of the input sequence
///
template <typename RAIterator>
Permutation indirect_stable_sort (RAIterator first, RAIterator last) {
    return indirect_stable_sort(first, last, 
                    std::less<typename std::iterator_traits<RAIterator>::value_type>());
}

                    // ===== partial_sort =====

/// \fn indirect_partial_sort (RAIterator first, RAIterator middle, RAIterator last, Predicate pred)
/// \returns a permutation of the elements in the range [first, last)
///     such that when the permutation is applied to the sequence,
///     the result is ordered as if 'std::partial_sort(first, middle, last, pred)'
//      was called on the sequence.
///
/// \param first  The start of the input sequence
/// \param middle The end of the range to be sorted
/// \param last   The end of the input sequence
/// \param pred   The predicate to compare elements with
///
template <typename RAIterator, typename Pred>
Permutation indirect_partial_sort (RAIterator first, RAIterator middle, 
                                   RAIterator last, Pred pred) {
	Permutation ret;
	detail::init_permutation(ret, std::distance(first, last));
	std::partial_sort(ret.begin(), ret.begin() + std::distance(first, middle), ret.end(), 
	                  detail::indirect_predicate<Pred, RAIterator>(pred, first));
	return ret;
}

/// \fn indirect_partial_sort (RAIterator first, RAIterator middle, RAIterator last)
/// \returns a permutation of the elements in the range [first, last)
///     such that when the permutation is applied to the sequence,
///     the result is ordered as if 'std::partial_sort(first, middle, last)'
//      was called on the sequence.
///
/// \param first  The start of the input sequence
/// \param middle The end of the range to be sorted
/// \param last   The end of the input sequence
///
template <typename RAIterator>
Permutation indirect_partial_sort (RAIterator first, RAIterator middle, RAIterator last) {
    return indirect_partial_sort(first, middle, last, 
                    std::less<typename std::iterator_traits<RAIterator>::value_type>());
}

                    // ===== nth_element =====

/// \fn indirect_nth_element (RAIterator first, RAIterator nth, RAIterator last, Predicate p)
/// \returns a permutation of the elements in the range [first, last)
///     such that when the permutation is applied to the sequence,
///     the result is ordered as if 'std::nth_element(first, nth, last, p)'
//      was called on the sequence.
///
/// \param first  The start of the input sequence
/// \param nth    The sort partition point in the input sequence
/// \param last   The end of the input sequence
/// \param pred   The predicate to compare elements with
///
template <typename RAIterator, typename Pred>
Permutation indirect_nth_element (RAIterator first, RAIterator nth,
                                  RAIterator last, Pred pred) {
	Permutation ret;
	detail::init_permutation(ret, std::distance(first, last));
	std::nth_element(ret.begin(), ret.begin() + std::distance(first, nth), ret.end(), 
	                  detail::indirect_predicate<Pred, RAIterator>(pred, first));
	return ret;
}

/// \fn indirect_nth_element (RAIterator first, RAIterator nth, RAIterator last)
/// \returns a permutation of the elements in the range [first, last)
///     such that when the permutation is applied to the sequence,
///     the result is ordered as if 'std::nth_element(first, nth, last)'
//      was called on the sequence.
///
/// \param first  The start of the input sequence
/// \param nth    The sort partition point in the input sequence
/// \param last   The end of the input sequence
///
template <typename RAIterator>
Permutation indirect_nth_element (RAIterator first, RAIterator nth, RAIterator last) {
    return indirect_nth_element(first, nth, last, 
                    std::less<typename std::iterator_traits<RAIterator>::value_type>());
}

}}

#endif // BOOST_ALGORITHM_INDIRECT_SORT

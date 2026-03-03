/* 
   Copyright (c) Marshall Clow 2023.

   Distributed under the Boost Software License, Version 1.0. (See accompanying
   file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

    For more information, see http://www.boost.org
*/

#include <boost/config.hpp>
#include <boost/algorithm/indirect_sort.hpp>
#include <boost/algorithm/apply_permutation.hpp>
#include <boost/algorithm/cxx11/is_sorted.hpp>
#include <boost/algorithm/cxx11/all_of.hpp>

#define BOOST_TEST_MAIN
#include <boost/test/unit_test.hpp>

#include <iostream>
#include <string>
#include <vector>
#include <list>

using boost::algorithm::Permutation;

// A permutation of size N is a sequence of values in the range [0..N)
// such that no value appears more than once in the permutation.
bool is_a_permutation(Permutation p, size_t N) {
	if (p.size() != N) return false;

//	Sort the permutation, and ensure that each value appears exactly once.
	std::sort(p.begin(), p.end());
	for (size_t i = 0; i < N; ++i)
		if (p[i] != i) return false;
	return true;
}

template <typename Iter, 
          typename Comp = typename std::less<typename std::iterator_traits<Iter>::value_type> >
struct indirect_comp {
	indirect_comp (Iter it, Comp c = Comp())
	: iter_(it), comp_(c) {}
	
	bool operator ()(size_t a, size_t b) const { return comp_(iter_[a], iter_[b]);}
	
	Iter iter_;
	Comp comp_;
};

						////  =======================
						////  ==== indirect_sort ====
						////  =======================
template <typename Iter>
void test_one_sort(Iter first, Iter last) {
	Permutation perm = boost::algorithm::indirect_sort(first, last);
	BOOST_CHECK (is_a_permutation(perm, std::distance(first, last)));
	BOOST_CHECK (boost::algorithm::is_sorted(perm.begin(), perm.end(), indirect_comp<Iter>(first)));

//	Make a copy of the data, apply the permutation, and ensure that it is sorted.
	typedef std::vector<typename std::iterator_traits<Iter>::value_type> Vector;
	Vector v(first, last);
	boost::algorithm::apply_permutation(v.begin(), v.end(), perm.begin(), perm.end());
	BOOST_CHECK (boost::algorithm::is_sorted(v.begin(), v.end()));
}

template <typename Iter, typename Comp>
void test_one_sort(Iter first, Iter last, Comp comp) {
	Permutation perm = boost::algorithm::indirect_sort(first, last, comp);
	BOOST_CHECK (is_a_permutation(perm, std::distance(first, last)));
	BOOST_CHECK (boost::algorithm::is_sorted(perm.begin(), perm.end(), 
	                             indirect_comp<Iter, Comp>(first, comp)));

//	Make a copy of the data, apply the permutation, and ensure that it is sorted.
	typedef std::vector<typename std::iterator_traits<Iter>::value_type> Vector;
	Vector v(first, last);
	boost::algorithm::apply_permutation(v.begin(), v.end(), perm.begin(), perm.end());
	BOOST_CHECK (boost::algorithm::is_sorted(v.begin(), v.end(), comp));
}


BOOST_AUTO_TEST_CASE(test_sort) {
    int num[] = { 1,3,5,7,9, 2, 4, 6, 8, 10 };
    const int sz = sizeof (num)/sizeof(num[0]);
    int       *first  = &num[0];
    int const *cFirst = &num[0];
    
//	Test subsets
    for (size_t i = 0; i <= sz; ++i) {
    	test_one_sort(first, first + i);
    	test_one_sort(first, first + i, std::greater<int>());

    //	test with constant inputs
    	test_one_sort(cFirst, cFirst + i);
    	test_one_sort(cFirst, cFirst + i, std::greater<int>());
    	}
    	
//	make sure we work with iterators as well as pointers
	std::vector<int> v(first, first + sz);
	test_one_sort(v.begin(), v.end());
	test_one_sort(v.begin(), v.end(), std::greater<int>());
    }


					////  ==============================
					////  ==== indirect_stable_sort ====
					////  ==============================

template <typename T1, typename T2>
struct MyPair {
	MyPair () {}

	MyPair (const T1 &t1, const T2 &t2)
	: first(t1), second(t2) {}
	
	T1 first;
	T2 second;
};

template <typename T1, typename T2>
bool operator < (const MyPair<T1, T2>& lhs, const MyPair<T1, T2>& rhs) {
	return lhs.first < rhs.first; // compare only the first elements
}

template <typename T1, typename T2>
bool MyGreater (const MyPair<T1, T2>& lhs, const MyPair<T1, T2>& rhs) {
	return lhs.first > rhs.first; // compare only the first elements
}

template <typename Iter>
void test_one_stable_sort(Iter first, Iter last) {
	Permutation perm = boost::algorithm::indirect_stable_sort(first, last);
	BOOST_CHECK (is_a_permutation(perm, std::distance(first, last)));
	BOOST_CHECK (boost::algorithm::is_sorted(perm.begin(), perm.end(), indirect_comp<Iter>(first)));

	if (first != last) {
		Iter iFirst = first;
		Iter iSecond = first; ++iSecond;
		
		while (iSecond != last) {
			if (iFirst->first == iSecond->first)
				BOOST_CHECK(iFirst->second < iSecond->second);
			++iFirst;
			++iSecond;
			}
		}

//	Make a copy of the data, apply the permutation, and ensure that it is sorted.
	typedef std::vector<typename std::iterator_traits<Iter>::value_type> Vector;
	Vector v(first, last);
	boost::algorithm::apply_permutation(v.begin(), v.end(), perm.begin(), perm.end());
	BOOST_CHECK (boost::algorithm::is_sorted(v.begin(), v.end()));
}

template <typename Iter, typename Comp>
void test_one_stable_sort(Iter first, Iter last, Comp comp) {
	Permutation perm = boost::algorithm::indirect_stable_sort(first, last, comp);
	BOOST_CHECK (is_a_permutation(perm, std::distance(first, last)));
	BOOST_CHECK (boost::algorithm::is_sorted(perm.begin(), perm.end(), indirect_comp<Iter, Comp>(first, comp)));

	if (first != last) {
		Iter iFirst = first;
		Iter iSecond = first; ++iSecond;
		
		while (iSecond != last) {
			if (iFirst->first == iSecond->first)
				BOOST_CHECK(iFirst->second < iSecond->second);
			++iFirst;
			++iSecond;
			}
		}

//	Make a copy of the data, apply the permutation, and ensure that it is sorted.
	typedef std::vector<typename std::iterator_traits<Iter>::value_type> Vector;
	Vector v(first, last);
	boost::algorithm::apply_permutation(v.begin(), v.end(), perm.begin(), perm.end());
	BOOST_CHECK (boost::algorithm::is_sorted(v.begin(), v.end(), comp));
}

BOOST_AUTO_TEST_CASE(test_stable_sort) {
	typedef MyPair<int, long>  Pair;
	const int sz = 10;
	Pair vals[sz];

	for (int i = 0; i < sz; ++i) {
		vals[i].first = 100 - (i >> 1);
		vals[i].second = i;
		}

    Pair        *first = &vals[0];
    Pair const *cFirst = &vals[0];
	
//	Test subsets
    for (size_t i = 0; i <= sz; ++i) {
    	test_one_stable_sort(first, first + i);
    	test_one_stable_sort(first, first + i, MyGreater<int, long>);

    //	test with constant inputs
    	test_one_sort(cFirst, cFirst + i);
    	test_one_sort(cFirst, cFirst + i, MyGreater<int, long>);
    	}
}

					////  ===============================
					////  ==== indirect_partial_sort ====
					////  ===============================

template <typename Iter>
void test_one_partial_sort(Iter first, Iter middle, Iter last) {
	const size_t middleIdx = std::distance(first, middle);
	Permutation perm = boost::algorithm::indirect_partial_sort(first, middle, last);
	BOOST_CHECK (is_a_permutation(perm, std::distance(first, last)));
	BOOST_CHECK (boost::algorithm::is_sorted(perm.begin(), perm.begin() + middleIdx, indirect_comp<Iter>(first)));

//	Make a copy of the data, apply the permutation, and ensure that it is sorted.
	typedef std::vector<typename std::iterator_traits<Iter>::value_type> Vector;
	Vector v(first, last);
	boost::algorithm::apply_permutation(v.begin(), v.end(), perm.begin(), perm.end());
	BOOST_CHECK (boost::algorithm::is_sorted(v.begin(), v.begin() + middleIdx));

//	Make sure that [middle, end) are all "greater" than the sorted part
	if (middleIdx > 0) {
		typename Vector::iterator lastSorted = v.begin() + middleIdx - 1;
		for (typename Vector::iterator it = v.begin () + middleIdx; it != v.end(); ++it)
			BOOST_CHECK(*lastSorted < *it);
		}
}

template <typename Iter, typename Comp>
void test_one_partial_sort(Iter first, Iter middle, Iter last, Comp comp) {
	const size_t middleIdx = std::distance(first, middle);
	Permutation perm = boost::algorithm::indirect_partial_sort(first, middle, last, comp);
	BOOST_CHECK (is_a_permutation(perm, std::distance(first, last)));
	BOOST_CHECK (boost::algorithm::is_sorted(perm.begin(), perm.begin() + middleIdx, 
	                             indirect_comp<Iter, Comp>(first, comp)));

//	Make a copy of the data, apply the permutation, and ensure that it is sorted.
	typedef std::vector<typename std::iterator_traits<Iter>::value_type> Vector;
	Vector v(first, last);
	boost::algorithm::apply_permutation(v.begin(), v.end(), perm.begin(), perm.end());
	BOOST_CHECK (boost::algorithm::is_sorted(v.begin(), v.begin() + middleIdx, comp));

//	Make sure that [middle, end) are all "greater" than the sorted part
	if (middleIdx > 0) {
		typename Vector::iterator lastSorted = v.begin() + middleIdx - 1;
		for (typename Vector::iterator it = v.begin () + middleIdx; it != v.end(); ++it)
			BOOST_CHECK(comp(*lastSorted, *it));
		}
}


BOOST_AUTO_TEST_CASE(test_partial_sort) {
    int num[] = { 1,3,5,7,9, 2, 4, 6, 8, 10 };
    const int sz = sizeof (num)/sizeof(num[0]);
    int       *first  = &num[0];
    int const *cFirst = &num[0];
    
//	Test subsets
    for (size_t i = 0; i <= sz; ++i) {
		for (size_t j = 0; j < i; ++j) {
			test_one_partial_sort(first, first + j, first + i);
			test_one_partial_sort(first, first + j, first + i, std::greater<int>());

		//	test with constant inputs
			test_one_partial_sort(cFirst, cFirst + j, cFirst + i);
			test_one_partial_sort(cFirst, cFirst + j, cFirst + i, std::greater<int>());
			}
    	}

//	make sure we work with iterators as well as pointers
	std::vector<int> v(first, first + sz);
	test_one_partial_sort(v.begin(), v.begin() + (sz / 2), v.end());
	test_one_partial_sort(v.begin(), v.begin() + (sz / 2), v.end(), std::greater<int>());
    }


					////  ===================================
					////  ==== indirect_nth_element_sort ====
					////  ===================================

template <typename Iter>
void test_one_nth_element(Iter first, Iter nth, Iter last) {
	const size_t nthIdx = std::distance(first, nth);
	Permutation perm = boost::algorithm::indirect_nth_element(first, nth, last);
	BOOST_CHECK (is_a_permutation(perm, std::distance(first, last)));

	for (size_t i = 0; i < nthIdx; ++i)
		BOOST_CHECK(!(first[perm[nthIdx]] < first[perm[i]])); // all items before the nth element are <= the nth element
	for (size_t i = nthIdx; i < std::distance(first, last); ++i)
		BOOST_CHECK(!(first[perm[i]] < first[perm[nthIdx]])); // all items before the nth element are >= the nth element

//	Make a copy of the data, apply the permutation, and ensure that the result is correct.
	typedef std::vector<typename std::iterator_traits<Iter>::value_type> Vector;
	Vector v(first, last);
	boost::algorithm::apply_permutation(v.begin(), v.end(), perm.begin(), perm.end());

	for (size_t i = 0; i < nthIdx; ++i)
		BOOST_CHECK(!(v[nthIdx] < v[i])); // all items before the nth element are <= the nth element
	for (size_t i = nthIdx; i < v.size(); ++i)
		BOOST_CHECK(!(v[i] < v[nthIdx])); // all items before the nth element are >= the nth element
}

template <typename Iter, typename Comp>
void test_one_nth_element(Iter first, Iter nth, Iter last, Comp comp) {
	const size_t nthIdx = std::distance(first, nth);

	Permutation perm = boost::algorithm::indirect_nth_element(first, nth, last, comp);
	BOOST_CHECK (is_a_permutation(perm, std::distance(first, last)));
	for (size_t i = 0; i < nthIdx; ++i)
		BOOST_CHECK(!comp(first[perm[nthIdx]], first[perm[i]])); // all items before the nth element are <= the nth element
	for (size_t i = nthIdx; i < std::distance(first, last); ++i)
		BOOST_CHECK(!comp(first[perm[i]], first[perm[nthIdx]])); // all items before the nth element are >= the nth element


//	Make a copy of the data, apply the permutation, and ensure that the result is correct.
	typedef std::vector<typename std::iterator_traits<Iter>::value_type> Vector;
	Vector v(first, last);
	boost::algorithm::apply_permutation(v.begin(), v.end(), perm.begin(), perm.end());

	for (size_t i = 0; i < nthIdx; ++i)
		BOOST_CHECK(!comp(v[nthIdx], v[i])); // all items before the nth element are <= the nth element
	for (size_t i = nthIdx; i < v.size(); ++i)
		BOOST_CHECK(!comp(v[i], v[nthIdx])); // all items before the nth element are >= the nth element
}


BOOST_AUTO_TEST_CASE(test_nth_element) {
    int num[] = { 1, 3, 5, 7, 9, 2, 4, 6, 8, 10, 1, 2, 3, 4, 5 };
    const int sz = sizeof (num)/sizeof(num[0]);
    int       *first  = &num[0];
    int const *cFirst = &num[0];
    
//	Test subsets
    for (size_t i = 0; i <= sz; ++i) {
		for (size_t j = 0; j < i; ++j) {
			test_one_nth_element(first, first + j, first + i);
			test_one_nth_element(first, first + j, first + i, std::greater<int>());

		//	test with constant inputs
			test_one_nth_element(cFirst, cFirst + j, cFirst + i);
			test_one_nth_element(cFirst, cFirst + j, cFirst + i, std::greater<int>());
			}
    	}
    	
//	make sure we work with iterators as well as pointers
	std::vector<int> v(first, first + sz);
	test_one_nth_element(v.begin(), v.begin() + (sz / 2), v.end());
	test_one_nth_element(v.begin(), v.begin() + (sz / 2), v.end(), std::greater<int>());
    }

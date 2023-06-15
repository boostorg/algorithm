/* 
   Copyright (c) Marshall Clow 2011-2012.

   Distributed under the Boost Software License, Version 1.0. (See accompanying
   file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

    For more information, see http://www.boost.org
*/

#include <boost/config.hpp>
#include <boost/algorithm/indirect_sort.hpp>
#include <boost/algorithm/apply_permutation.hpp>
#include <boost/algorithm/cxx11/is_sorted.hpp>

#define BOOST_TEST_MAIN
#include <boost/test/unit_test.hpp>

#include <iostream>
#include <string>
#include <vector>
#include <list>

typedef std::vector<size_t> Permutation;

// A permutation of size N is a sequence of values in the range [0..N)
// such that no value appears more than once in the permutation.
bool isa_permutation(Permutation p, size_t N) {
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

template <typename Iter>
void test_one_sort(Iter first, Iter last) {
	Permutation perm = boost::algorithm::indirect_sort(first, last);
	BOOST_CHECK (isa_permutation(perm, std::distance(first, last)));
	BOOST_CHECK (boost::algorithm::is_sorted(perm.begin(), perm.end(), indirect_comp<Iter>(first)));

//	Make a copy of the data, apply the permutation, and ensure that it is sorted.
	std::vector<typename std::iterator_traits<Iter>::value_type> v(first, last);
	boost::algorithm::apply_permutation(v.begin(), v.end(), perm.begin(), perm.end());
	BOOST_CHECK (boost::algorithm::is_sorted(v.begin(), v.end()));
}

template <typename Iter, typename Comp>
void test_one_sort(Iter first, Iter last, Comp comp) {
	Permutation perm = boost::algorithm::indirect_sort(first, last, comp);
	BOOST_CHECK (isa_permutation(perm, std::distance(first, last)));
	BOOST_CHECK (boost::algorithm::is_sorted(perm.begin(), perm.end(), 
	                             indirect_comp<Iter, Comp>(first, comp)));

//	Make a copy of the data, apply the permutation, and ensure that it is sorted.
	std::vector<typename std::iterator_traits<Iter>::value_type> v(first, last);
	boost::algorithm::apply_permutation(v.begin(), v.end(), perm.begin(), perm.end());
	BOOST_CHECK (boost::algorithm::is_sorted(v.begin(), v.end(), comp));
}


void test_sort () {
    BOOST_CXX14_CONSTEXPR int num[] = { 1,3,5,7,9, 2, 4, 6, 8, 10 };
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
    
BOOST_AUTO_TEST_CASE( test_main )
{
  test_sort ();
}

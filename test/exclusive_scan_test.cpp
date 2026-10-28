/*
   Copyright (c) Marshall Clow 2017.

   Distributed under the Boost Software License, Version 1.0. (See accompanying
   file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

    For more information, see http://www.boost.org
*/

#include <vector>
#include <functional>
#include <numeric>
#include <algorithm>

#include <boost/config.hpp>
#include <boost/algorithm/cxx11/iota.hpp>
#include <boost/algorithm/cxx17/exclusive_scan.hpp>

#include "iterator_test.hpp"

#define BOOST_TEST_MAIN
#include <boost/test/unit_test.hpp>

namespace ba = boost::algorithm;

int triangle(int n) { return n*(n+1)/2; }

void basic_tests_init()
{
    {
    std::vector<int> v(10);
    std::fill(v.begin(), v.end(), 3);
    ba::exclusive_scan(v.begin(), v.end(), v.begin(), 50);
    for (size_t i = 0; i < v.size(); ++i)
        BOOST_CHECK(v[i] == 50 + (int) i * 3);
    }

    {
    std::vector<int> v(10);
    ba::iota(v.begin(), v.end(), 0);
    ba::exclusive_scan(v.begin(), v.end(), v.begin(), 30);
    for (size_t i = 0; i < v.size(); ++i)
        BOOST_CHECK(v[i] == 30 + triangle(i-1));
    }

    {
    std::vector<int> v(10);
    ba::iota(v.begin(), v.end(), 1);
    ba::exclusive_scan(v.begin(), v.end(), v.begin(), 40);
    for (size_t i = 0; i < v.size(); ++i)
        BOOST_CHECK(v[i] == 40 + triangle(i));
    }

}

void test_mixed_types()
{
    const int input[] = {1, 2, 3};
    double output[3] = {};
    const double expected[] = {0.5, 1.5, 3.5};
    BOOST_CHECK(ba::exclusive_scan(input, input + 3, output, 0.5) == output + 3);
    BOOST_CHECK_EQUAL_COLLECTIONS(output, output + 3, expected, expected + 3);

    const unsigned char small[] = {100, 100, 100};
    unsigned int wide[3] = {};
    const unsigned int wide_expected[] = {1000, 1100, 1200};
    ba::exclusive_scan(small, small + 3, wide, 1000U);
    BOOST_CHECK_EQUAL_COLLECTIONS(wide, wide + 3, wide_expected, wide_expected + 3);

    const double fractions[] = {0.5, 0.5, 0.5};
    int integral[3] = {};
    const int integral_expected[] = {-1, 0, 0};
    ba::exclusive_scan(fractions, fractions + 3, integral, -1);
    BOOST_CHECK_EQUAL_COLLECTIONS(integral, integral + 3,
                                  integral_expected, integral_expected + 3);

    BOOST_CHECK(ba::exclusive_scan(input, input, output, 0.5) == output);
    BOOST_CHECK_EQUAL(output[0], 0.5);
}

void test_exclusive_scan_init()
{
	basic_tests_init();
}

void test_exclusive_scan_init_op()
{
	BOOST_CHECK(true);
}



BOOST_AUTO_TEST_CASE( test_main )
{
  test_exclusive_scan_init();
  test_exclusive_scan_init_op();
  test_mixed_types();
}

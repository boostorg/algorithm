/*
   Copyright (c) Jonathan Gopel 2022.

   Distributed under the Boost Software License, Version 1.0. (See accompanying
   file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
*/

#include <boost/algorithm/iterate.hpp>

#include "iterator_test.hpp"

#include <boost/config.hpp>

#define BOOST_TEST_MAIN
#include <boost/test/unit_test.hpp>

#include <boost/algorithm/cxx14/equal.hpp>

#include <algorithm>
#include <array>
#include <functional>
#include <vector>

typedef int data_t;

static const int NUM_ELEMENTS = 4;

BOOST_CONSTEXPR data_t add_one(const data_t value) { return value + 1; }

BOOST_CONSTEXPR data_t add_two(const data_t value) { return value + 2; }

void test_iterate() {
  { // iota
    std::vector<data_t> actual(NUM_ELEMENTS);

    boost::algorithm::iterate(actual.begin(), actual.end(), 15, add_one);

    std::vector<data_t> expected(NUM_ELEMENTS);
    expected.at(0) = 15;
    expected.at(1) = 16;
    expected.at(2) = 17;
    expected.at(3) = 18;
    BOOST_CHECK(actual.size() == expected.size());
    BOOST_CHECK(std::equal(actual.begin(), actual.end(), expected.begin()));
  }
  { // striding
    std::vector<data_t> actual(NUM_ELEMENTS);

    boost::algorithm::iterate(actual.begin(), actual.end(), 15, add_two);

    std::vector<data_t> expected(NUM_ELEMENTS);
    expected.at(0) = 15;
    expected.at(1) = 17;
    expected.at(2) = 19;
    expected.at(3) = 21;
    BOOST_CHECK(std::equal(actual.begin(), actual.end(), expected.begin()));
  }
}

void test_iterate_range() {
  { // iota
    std::vector<data_t> actual(NUM_ELEMENTS);

    boost::algorithm::iterate(actual, 15, add_one);

    std::vector<data_t> expected(NUM_ELEMENTS);
    expected.at(0) = 15;
    expected.at(1) = 16;
    expected.at(2) = 17;
    expected.at(3) = 18;
    BOOST_CHECK(actual.size() == expected.size());
    BOOST_CHECK(std::equal(actual.begin(), actual.end(), expected.begin()));
  }
  { // striding
    std::vector<data_t> actual(NUM_ELEMENTS);

    boost::algorithm::iterate(actual, 15, add_two);

    std::vector<data_t> expected(NUM_ELEMENTS);
    expected.at(0) = 15;
    expected.at(1) = 17;
    expected.at(2) = 19;
    expected.at(3) = 21;
    BOOST_CHECK(std::equal(actual.begin(), actual.end(), expected.begin()));
  }
}

void test_iterate_n() {
  { // iota
    std::vector<data_t> actual(NUM_ELEMENTS);

    boost::algorithm::iterate(actual.begin(), actual.size(), 15, add_one);

    std::vector<data_t> expected(NUM_ELEMENTS);
    expected.at(0) = 15;
    expected.at(1) = 16;
    expected.at(2) = 17;
    expected.at(3) = 18;
    BOOST_CHECK(actual.size() == expected.size());
    BOOST_CHECK(std::equal(actual.begin(), actual.end(), expected.begin()));
  }
  { // striding
    std::vector<data_t> actual(NUM_ELEMENTS);

    boost::algorithm::iterate(actual.begin(), actual.size(), 15, add_two);

    std::vector<data_t> expected(NUM_ELEMENTS);
    expected.at(0) = 15;
    expected.at(1) = 17;
    expected.at(2) = 19;
    expected.at(3) = 21;
    BOOST_CHECK(actual.size() == expected.size());
    BOOST_CHECK(std::equal(actual.begin(), actual.end(), expected.begin()));
  }
}

BOOST_CXX14_CONSTEXPR inline bool constexpr_test_iterate() {
  int actual[] = {0, 0, 0, 0};

  boost::algorithm::iterate(input_iterator<int *>(actual),
                            input_iterator<int *>(actual + NUM_ELEMENTS), 15,
                            add_one);

  int expected[] = {15, 16, 17, 18};
  return boost::algorithm::equal(
      input_iterator<int *>(actual),
      input_iterator<int *>(actual + NUM_ELEMENTS),
      input_iterator<int *>(expected),
      input_iterator<int *>(expected + NUM_ELEMENTS));
}

BOOST_CXX14_CONSTEXPR inline bool constexpr_test_iterate_n() {
  int actual[] = {0, 0, 0, 0};

  boost::algorithm::iterate(input_iterator<int *>(actual), NUM_ELEMENTS, 15,
                            add_one);

  int expected[] = {15, 16, 17, 18};
  return boost::algorithm::equal(
      input_iterator<int *>(actual),
      input_iterator<int *>(actual + NUM_ELEMENTS),
      input_iterator<int *>(expected),
      input_iterator<int *>(expected + NUM_ELEMENTS));
}

BOOST_AUTO_TEST_CASE(test_main) {
  test_iterate();
  test_iterate_range();
  test_iterate_n();

  {
    BOOST_CXX14_CONSTEXPR bool result = constexpr_test_iterate();
    BOOST_CHECK(result);
  }

  {
    BOOST_CXX14_CONSTEXPR bool result = constexpr_test_iterate_n();
    BOOST_CHECK(result);
  }
}

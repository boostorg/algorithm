/*
  Copyright (c) Alexander Zaitsev <zamazan4ik@gmail.com>, 2017

  Distributed under the Boost Software License, Version 1.0. (See
  accompanying file LICENSE_1_0.txt or copy at
  http://www.boost.org/LICENSE_1_0.txt)

  See http://www.boost.org/ for latest version.
*/

#include <vector>
#include <list>
#include <iterator>
#include <functional>
#include <iostream>

#include <boost/algorithm/apply_permutation.hpp>

#define BOOST_TEST_MAIN

#include <boost/test/unit_test.hpp>

namespace ba = boost::algorithm;


void test_apply_permutation()
{
    //Empty
    {
        std::vector<int> vec, order, result;
        ba::apply_permutation(vec.begin(), vec.end(), order.begin());
        BOOST_CHECK(vec == result);
    }
    //1 element
    {
        std::vector<int> vec{1}, order{0}, result{1};
        ba::apply_permutation(vec.begin(), vec.end(), order.begin());
        BOOST_CHECK(vec == result);
    }
    //2 elements, no changes
    {
        std::vector<int> vec{1, 2}, order{0, 1}, result{1, 2};
        ba::apply_permutation(vec.begin(), vec.end(), order.begin());
        BOOST_CHECK(vec == result);
    }
    //2 elements, changed
    {
        std::vector<int> vec{1, 2}, order{1, 0}, result{2, 1};
        ba::apply_permutation(vec.begin(), vec.end(), order.begin());
        BOOST_CHECK(vec == result);
    }
    //Multiple elements, no changes
    {
        std::vector<int> vec{1, 2, 3, 4, 5}, order{0, 1, 2, 3, 4}, result{1, 2, 3, 4, 5};
        ba::apply_permutation(vec.begin(), vec.end(), order.begin());
        BOOST_CHECK(vec == result);
    }
    //Multiple elements, changed
    {
        std::vector<int> vec{1, 2, 3, 4, 5}, order{4, 3, 2, 1, 0}, result{5, 4, 3, 2, 1};
        ba::apply_permutation(vec.begin(), vec.end(), order.begin());
        BOOST_CHECK(vec == result);
    }
    //Just test range interface
    {
        std::vector<int> vec{1, 2, 3, 4, 5}, order{0, 1, 2, 3, 4}, result{1, 2, 3, 4, 5};
        ba::apply_permutation(vec, order);
        BOOST_CHECK(vec == result);
    }
}

void test_apply_reverse_permutation()
{
    //Empty
    {
        std::vector<int> vec, order, result;
        ba::apply_reverse_permutation(vec.begin(), vec.end(), order.begin());
        BOOST_CHECK(vec == result);
    }
    //1 element
    {
        std::vector<int> vec{1}, order{0}, result{1};
        ba::apply_reverse_permutation(vec.begin(), vec.end(), order.begin());
        BOOST_CHECK(vec == result);
    }
    //2 elements, no changes
    {
        std::vector<int> vec{1, 2}, order{0, 1}, result{1, 2};
        ba::apply_reverse_permutation(vec.begin(), vec.end(), order.begin());
        BOOST_CHECK(vec == result);
    }
    //2 elements, changed
    {
        std::vector<int> vec{1, 2}, order{1, 0}, result{2, 1};
        ba::apply_reverse_permutation(vec.begin(), vec.end(), order.begin());
        BOOST_CHECK(vec == result);
    }
    //Multiple elements, no changes
    {
        std::vector<int> vec{1, 2, 3, 4, 5}, order{0, 1, 2, 3, 4}, result{1, 2, 3, 4, 5};
        ba::apply_reverse_permutation(vec.begin(), vec.end(), order.begin());
        BOOST_CHECK(vec == result);
    }
    //Multiple elements, changed
    {
        std::vector<int> vec{1, 2, 3, 4, 5}, order{4, 3, 2, 1, 0}, result{5, 4, 3, 2, 1};
        ba::apply_reverse_permutation(vec.begin(), vec.end(), order.begin());
        BOOST_CHECK(vec == result);
    }
    //Just test range interface
    {
        std::vector<int> vec{1, 2, 3, 4, 5}, order{0, 1, 2, 3, 4}, result{1, 2, 3, 4, 5};
        ba::apply_reverse_permutation(vec, order);
        BOOST_CHECK(vec == result);
    }
}

BOOST_AUTO_TEST_CASE(test_main)
{
    test_apply_permutation();
    test_apply_reverse_permutation();
}

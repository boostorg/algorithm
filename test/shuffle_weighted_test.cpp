/*
  Copyright (c) Alexander Zaitsev <zamazan4ik@gmail.com>, 2017

  Distributed under the Boost Software License, Version 1.0. (See
  accompanying file LICENSE_1_0.txt or copy at
  http://www.boost.org/LICENSE_1_0.txt)

  See http://www.boost.org/ for latest version.
*/

#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MAIN

#include <vector>

#include <boost/algorithm/shuffle_weighted.hpp>

#include <boost/random/random_device.hpp>
#include <boost/test/unit_test.hpp>

namespace ba = boost::algorithm;
namespace br = boost::random;

void test_shuffle_weighted()
{
    {
        // Empty case
        
        br::mt19937_64 d;
        std::vector<int> vec, weights;
        
        ba::shuffle_weighted(vec, weights, d);
        BOOST_CHECK(vec.empty () && weights.empty());
    }
    {
        // One element case
        
        br::mt19937_64 d;
        std::vector<int> vec, weights;
        vec.push_back(1);
        weights.push_back(1);
        std::vector<int> new_vec = vec, new_weights = weights;
        
        ba::shuffle_weighted(new_vec, new_weights, d);
        BOOST_CHECK(vec == new_vec && weights == new_weights);
    }
    {
        // Two element case
        
        br::mt19937_64 d;
        std::vector<int> vec, rev_vec, weights;
        vec.push_back(1); vec.push_back(2);
        rev_vec.push_back(2); rev_vec.push_back(1);
        weights = vec;
        std::vector<int> new_vec = vec, new_weights = weights;
        
        ba::shuffle_weighted(new_vec, new_weights, d);
        BOOST_CHECK(vec == new_vec || rev_vec == new_vec);
    }
}


BOOST_AUTO_TEST_CASE(test_main)
{
    test_shuffle_weighted();
}

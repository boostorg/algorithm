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

#include "shuffle_weighted.hpp"

#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MAIN

#include <boost/test/unit_test.hpp>

namespace ba = boost::algorithm;


void test_shuffle_weighted()
{
    BOOST_CHECK(true);
}


BOOST_AUTO_TEST_CASE(test_main)
{
    test_shuffle_weighted();
}

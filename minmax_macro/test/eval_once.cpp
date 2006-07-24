///////////////////////////////////////////////////////////////////////////////
// eval_once.hpp test file
//
// Copyright 2006 Eric Niebler.
// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//

#include <boost/test/unit_test.hpp>
#include <boost/algorithm/minmax_macro.hpp>

void test_eval_once()
{
    int i = 0;
    int j = 1;

    int k = BOOST_MIN(i++,j);
    BOOST_CHECK_EQUAL(1, i);
    BOOST_CHECK_EQUAL(0, k);

    k = BOOST_MAX(i,++j);
    BOOST_CHECK_EQUAL(2, j);
    BOOST_CHECK_EQUAL(2, k);
}

///////////////////////////////////////////////////////////////////////////////
// init_unit_test_suite
//
boost::unit_test::test_suite* init_unit_test_suite( int argc, char* argv[] )
{
    boost::unit_test::test_suite *test = BOOST_TEST_SUITE("eval_once min/max test");

    test->add(BOOST_TEST_CASE(&test_eval_once));

    return test;
}

///////////////////////////////////////////////////////////////////////////////
// basic.hpp test file
//
// Copyright 2006 Eric Niebler.
// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//

#include <boost/test/unit_test.hpp>
#include <boost/algorithm/minmax_macro.hpp>

void basic_min_test()
{
    BOOST_CHECK_EQUAL(1, BOOST_MIN(1,2));
    BOOST_CHECK_EQUAL(1, BOOST_MIN(2,1));

    int one=1, two=2;
    BOOST_CHECK_EQUAL(one, BOOST_MIN(one,two));
    BOOST_CHECK_EQUAL(one, BOOST_MIN(two,one));
}

void basic_max_test()
{
    BOOST_CHECK_EQUAL(2, BOOST_MAX(1,2));
    BOOST_CHECK_EQUAL(2, BOOST_MAX(2,1));

    int one=1, two=2;
    BOOST_CHECK_EQUAL(two, BOOST_MAX(one,two));
    BOOST_CHECK_EQUAL(two, BOOST_MAX(two,one));
}

///////////////////////////////////////////////////////////////////////////////
// init_unit_test_suite
//
boost::unit_test::test_suite* init_unit_test_suite( int argc, char* argv[] )
{
    boost::unit_test::test_suite *test = BOOST_TEST_SUITE("basic min/max test");

    test->add(BOOST_TEST_CASE(&basic_min_test));
    test->add(BOOST_TEST_CASE(&basic_max_test));

    return test;
}

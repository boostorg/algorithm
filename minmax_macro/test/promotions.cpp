///////////////////////////////////////////////////////////////////////////////
// promotions.hpp test file
//
// Copyright 2006 Eric Niebler.
// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//

#include <boost/test/unit_test.hpp>
#include <boost/mpl/assert.hpp>
#include <boost/type_traits/is_same.hpp>
#include <boost/algorithm/minmax_macro.hpp>

template<typename Result, typename T> void check_is_same(T)
{
    BOOST_MPL_ASSERT((boost::is_same<T,Result>));
}

void test_promotions()
{
    char ch = 0;
    short sh = 0;
    int in = 0;
    unsigned un = 0;
    float fl = 0;
    double db = 0;

    check_is_same<int>(BOOST_MIN(ch,sh));
    check_is_same<int>(BOOST_MAX(ch,sh));
    check_is_same<int>(BOOST_MIN(ch,in));
    check_is_same<int>(BOOST_MAX(ch,in));

    check_is_same<unsigned>(BOOST_MIN(un,in));
    check_is_same<unsigned>(BOOST_MAX(un,in));

    check_is_same<float>(BOOST_MIN(in,fl));
    check_is_same<float>(BOOST_MAX(in,fl));

    check_is_same<double>(BOOST_MIN(db,fl));
    check_is_same<double>(BOOST_MAX(db,fl));
}

///////////////////////////////////////////////////////////////////////////////
// init_unit_test_suite
//
boost::unit_test::test_suite* init_unit_test_suite( int argc, char* argv[] )
{
    boost::unit_test::test_suite *test = BOOST_TEST_SUITE("promotions min/max test");

    test->add(BOOST_TEST_CASE(&test_promotions));

    return test;
}

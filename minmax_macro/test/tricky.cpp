///////////////////////////////////////////////////////////////////////////////
// tricky.hpp test file
//
// Copyright 2006 Eric Niebler.
// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//

#include <boost/test/unit_test.hpp>
#include <boost/algorithm/minmax_macro.hpp>

struct Base
{
    virtual bool operator <(Base const &) const = 0;
};

struct Derived : Base
{
    virtual bool operator <(Base const &that) const
    {
        return dynamic_cast<void const*>(this) < dynamic_cast<void const*>(&that);
    }
};

void test_tricky()
{
    Derived d;
    Base &b = d;

    Base &lvalue = BOOST_MIN(d,b);
}

///////////////////////////////////////////////////////////////////////////////
// init_unit_test_suite
//
boost::unit_test::test_suite* init_unit_test_suite( int argc, char* argv[] )
{
    boost::unit_test::test_suite *test = BOOST_TEST_SUITE("tricky min/max test");

    test->add(BOOST_TEST_CASE(&test_tricky));

    return test;
}

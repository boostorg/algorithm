///////////////////////////////////////////////////////////////////////////////
// rvalue_lvalue.hpp test file
//
// Copyright 2006 Eric Niebler.
// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//
// Test code from Howard Hinnant (TODO: get permission!)

#include <boost/test/included/unit_test.hpp>
#include <boost/algorithm/minmax_macro.hpp>

class A
{
private:
    int i_;
public:
    A(int i) : i_(i) {}
    ~A() {i_ = 0;}

    A(const A& a) : i_(a.i_) {}

    operator int() const {return i_;}

    friend bool operator<(const A& x, const A& y)
    {return  x.i_ < y.i_;}

    friend bool operator==(const A& x, const A& y)
    {return  x.i_ == y.i_;}
};

void test_rvalue_lvalue()
{
    // lvalues of the same type:
    {
        A a3(3);
        A a2(2);
        BOOST_MIN(a2, a3) = A(1);  // a2 == 1, no copy, no move
        BOOST_CHECK_EQUAL(a2, A(1));
        BOOST_CHECK_EQUAL(a3, A(3));
    }

    //{
    //    // lvalues of the same type with different cv
    //    const A a3(3);
    //    A a2(2);
    //    BOOST_MIN(a2, a3) = A(1);  // illegal operands 'const A' = 'A'
    //}

    {
        const A a3(3);
        A a2(2);
        const A& a_ref = BOOST_MIN(a2, a3);
        a2 = A(1);  // a_ref == 1, a_ref refers to a2, no copy, no move
        BOOST_CHECK_EQUAL(a_ref, A(1));
    }

    {
        // lvalue / rvalue mix of the same type
        A a3(3);
        const A& a_ref = BOOST_MIN(A(2), a3);
        BOOST_CHECK_EQUAL(a_ref, A(2));
        // a_ref refers to temporary returned (by value) from BOOST_MIN, a_ref == 2, A(A&&) executed to move from A(2)
        // A(const A&) - or some copy constructor - required to be accessible
    }

    {
        A a1(1);
        const A& a_ref = BOOST_MIN(A(2), a1);
        BOOST_CHECK_EQUAL(a_ref, A(1));
        // a_ref refers to temporary returned (by value) from BOOST_MIN, a_ref == 1, A(const A&) executed to copy from a1
    }

    {
        // rvalue / rvalue
        const A& a_ref = BOOST_MIN(A(2), A(3));
        BOOST_CHECK_EQUAL(a_ref, A(2));
        // a_ref refers to temporary returned (by value) from BOOST_MIN, a_ref == 2, A(A&&) executed to move from A(2)
        // accessible copy constructor not required
    }
}

///////////////////////////////////////////////////////////////////////////////
// init_unit_test_suite
//
boost::unit_test::test_suite* init_unit_test_suite( int argc, char* argv[] )
{
    boost::unit_test::test_suite *test = BOOST_TEST_SUITE("rvalue/lvalue min/max test");

    test->add(BOOST_TEST_CASE(&test_rvalue_lvalue));

    return test;
}

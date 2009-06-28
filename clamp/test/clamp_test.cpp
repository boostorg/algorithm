//  (C) Copyright Jesse Williamson 2009
//  Use, modification and distribution are subject to the
//  Boost Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include <iostream>

#include <boost/config.hpp>
#include <boost/algorithm/clamp.hpp>

#include <boost/test/included/test_exec_monitor.hpp>

namespace ba = boost::algorithm;

void test_ints()
{

//  Inside the range, equal to the endpoints, and outside the endpoints.
    BOOST_CHECK_EQUAL (  3, ba::clamp (  3, 1, 10 ));
    BOOST_CHECK_EQUAL (  1, ba::clamp (  1, 1, 10 ));
    BOOST_CHECK_EQUAL (  1, ba::clamp (  0, 1, 10 ));
    BOOST_CHECK_EQUAL ( 10, ba::clamp ( 10, 1, 10 ));
    BOOST_CHECK_EQUAL ( 10, ba::clamp ( 11, 1, 10 ));
    
//  Negative numbers
    BOOST_CHECK_EQUAL (  -3, ba::clamp (  -3, -10, -1 ));
    BOOST_CHECK_EQUAL (  -1, ba::clamp (  -1, -10, -1 ));
    BOOST_CHECK_EQUAL (  -1, ba::clamp (   0, -10, -1 ));
    BOOST_CHECK_EQUAL ( -10, ba::clamp ( -10, -10, -1 ));
    BOOST_CHECK_EQUAL ( -10, ba::clamp ( -11, -10, -1 ));

//  Mixed positive and negative numbers
    BOOST_CHECK_EQUAL (   5, ba::clamp (   5, -10, 10 ));
    BOOST_CHECK_EQUAL ( -10, ba::clamp ( -10, -10, 10 ));
    BOOST_CHECK_EQUAL ( -10, ba::clamp ( -15, -10, 10 ));
    BOOST_CHECK_EQUAL (  10, ba::clamp (  10, -10, 10 ));
    BOOST_CHECK_EQUAL (  10, ba::clamp (  15, -10, 10 ));

//  Unsigned 
    BOOST_CHECK_EQUAL (  5U, ba::clamp (  5U, 1U, 10U ));
    BOOST_CHECK_EQUAL (  1U, ba::clamp (  1U, 1U, 10U ));
    BOOST_CHECK_EQUAL (  1U, ba::clamp (  0U, 1U, 10U ));
    BOOST_CHECK_EQUAL ( 10U, ba::clamp ( 10U, 1U, 10U ));
    BOOST_CHECK_EQUAL ( 10U, ba::clamp ( 15U, 1U, 10U ));
}


void test_floats()
{

//  Inside the range, equal to the endpoints, and outside the endpoints.
    BOOST_CHECK_EQUAL (  3.0, ba::clamp (  3.0, 1.0, 10.0 ));
    BOOST_CHECK_EQUAL (  1.0, ba::clamp (  1.0, 1.0, 10.0 ));
    BOOST_CHECK_EQUAL (  1.0, ba::clamp (  0.0, 1.0, 10.0 ));
    BOOST_CHECK_EQUAL ( 10.0, ba::clamp ( 10.0, 1.0, 10.0 ));
    BOOST_CHECK_EQUAL ( 10.0, ba::clamp ( 11.0, 1.0, 10.0 ));
    
//  Negative numbers
    BOOST_CHECK_EQUAL (  -3.f, ba::clamp (  -3.f, -10.f, -1.f ));
    BOOST_CHECK_EQUAL (  -1.f, ba::clamp (  -1.f, -10.f, -1.f ));
    BOOST_CHECK_EQUAL (  -1.f, ba::clamp (   0.f, -10.f, -1.f ));
    BOOST_CHECK_EQUAL ( -10.f, ba::clamp ( -10.f, -10.f, -1.f ));
    BOOST_CHECK_EQUAL ( -10.f, ba::clamp ( -11.f, -10.f, -1.f ));

//  Mixed positive and negative numbers
    BOOST_CHECK_EQUAL (   5.f, ba::clamp (   5.f, -10.f, 10.f ));
    BOOST_CHECK_EQUAL ( -10.f, ba::clamp ( -10.f, -10.f, 10.f ));
    BOOST_CHECK_EQUAL ( -10.f, ba::clamp ( -15.f, -10.f, 10.f ));
    BOOST_CHECK_EQUAL (  10.f, ba::clamp (  10.f, -10.f, 10.f ));
    BOOST_CHECK_EQUAL (  10.f, ba::clamp (  15.f, -10.f, 10.f ));

}

class custom {
public:
    custom ( int x )             : v(x)     {}
    custom ( const custom &rhs ) : v(rhs.v) {}
    ~custom () {}
    custom & operator = ( const custom &rhs ) { v = rhs.v; return *this; }
    
    bool operator <  ( const custom &rhs ) const { return v < rhs.v; }
    bool operator == ( const custom &rhs ) const { return v == rhs.v; }     // need this for the test
    
    std::ostream & print ( std::ostream &os ) const { return os << v; }
    
private:
    int v;
    };

std::ostream & operator << ( std::ostream & os, const custom &x ) { return x.print ( os ); }

void test_custom()
{

//  Inside the range, equal to the endpoints, and outside the endpoints.
    BOOST_CHECK_EQUAL ( custom( 3), ba::clamp ( custom( 3), custom(1), custom(10)));
    BOOST_CHECK_EQUAL ( custom( 1), ba::clamp ( custom( 1), custom(1), custom(10)));
    BOOST_CHECK_EQUAL ( custom( 1), ba::clamp ( custom( 0), custom(1), custom(10)));
    BOOST_CHECK_EQUAL ( custom(10), ba::clamp ( custom(10), custom(1), custom(10)));
    BOOST_CHECK_EQUAL ( custom(10), ba::clamp ( custom(11), custom(1), custom(10)));

//  Fail!!
//  BOOST_CHECK_EQUAL ( custom(1), ba::clamp ( custom(11), custom(1), custom(10)));
}

int test_main( int , char* [] )
{
  test_ints   ();
  test_floats ();
  test_custom ();
  return 0;
}

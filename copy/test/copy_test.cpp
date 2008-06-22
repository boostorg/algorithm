//  (C) Copyright Marshall Clow 2008
//  Use, modification and distribution are subject to the
//  Boost Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include <boost/config.hpp>
#include <boost/algorithm/copy.hpp>
#include <boost/test/included/test_exec_monitor.hpp>

#include <utility>
#include <functional>

// #include <boost/algorithm/all.hpp>
// #include <boost/algorithm/select.hpp>

template <int v> bool IsEqual    ( int v2 ) { return v2 == v; }
template <int v> bool IsNotEqual ( int v2 ) { return v2 != v; }
template <int v> bool IsGreater  ( int v2 ) { return v2  > v; }
template <int v> bool IsLess     ( int v2 ) { return v2  < v; }
bool IsEven ( int v2 ) { return ( v2 & 1 ) == 0; }
bool IsOdd  ( int v2 ) { return ( v2 & 1 ) != 0; }

namespace bas = boost::algorithm::sequence;

void test_copy_if () 
{
  const int vals [] = { 0, 1, 0, 1, 0, 0, 0, 1, 1, 1, 1 };
  const unsigned int valsSize = sizeof ( vals ) / sizeof ( vals [0] );
  
  std::vector<int> cont;
  std::vector<int> res;

// Copy all but the last element
  std::copy ( vals, vals + valsSize - 1, std::back_inserter(cont));

// Copy_if from constant iterators
  res.clear ();
  bas::copy_if ( vals, vals + valsSize, std::back_inserter(res),  IsEqual<0> );
  BOOST_CHECK_EQUAL ( res.size (),  5U );
  BOOST_CHECK ( std::find_if ( res.begin (), res.end (), IsNotEqual<0> ) == res.end ());

// Copy them all
  res.clear ();
  bas::copy_if ( vals, vals + valsSize, std::back_inserter(res), IsLess<100> );
  BOOST_CHECK_EQUAL ( valsSize, res.size ());
  BOOST_CHECK ( std::equal ( vals, vals + valsSize, res.begin ()));

// Copy none
  res.clear ();
  bas::copy_if ( vals, vals + valsSize, std::back_inserter(res), IsGreater<100> );
  BOOST_CHECK_EQUAL ( 0U, res.size ());

// Copy_if using ranges
  res.clear ();
  bas::copy_if ( cont, std::back_inserter(res),  IsNotEqual<0> );
  BOOST_CHECK_EQUAL ( res.size (),  5U );
  BOOST_CHECK ( std::find_if ( res.begin (), res.end (), IsEqual<0> ) == res.end ());

// Copy them all
  res.clear ();
  bas::copy_if ( cont, std::back_inserter(res), IsNotEqual<2> );
  BOOST_CHECK_EQUAL ( cont.size (), res.size ());
  BOOST_CHECK ( std::equal ( cont.begin (), cont.end (), res.begin ()));

// Copy none
  res.clear ();
  bas::copy_if ( cont, std::back_inserter(res), IsEqual<2> );
  BOOST_CHECK_EQUAL ( 0U, res.size ());

// ---- Backwards tests ----
  const int vals2 [] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };
  const unsigned int vals2Size = sizeof ( vals2 ) / sizeof ( vals2 [0] );
  
// Copy all but the last element
  cont.clear ();
  std::copy ( vals2, vals2 + vals2Size - 1, std::back_inserter(cont));

// Copy_if_backward from constant iterators
  res.clear ();
  bas::copy_backward_if ( vals2, vals2 + vals2Size, std::back_inserter(res), IsEven );
  BOOST_CHECK_EQUAL ( res.size (), 5U );
  BOOST_CHECK_EQUAL ( res[0], 8 );
  BOOST_CHECK_EQUAL ( res[1], 6 );
  BOOST_CHECK_EQUAL ( res[2], 4 );
  BOOST_CHECK_EQUAL ( res[3], 2 );
  BOOST_CHECK_EQUAL ( res[4], 0 );

// Copy them all
  res.clear ();
  bas::copy_backward_if ( vals2, vals2 + vals2Size, std::back_inserter(res), IsLess<100> );
  BOOST_CHECK_EQUAL ( vals2Size, res.size ());
  BOOST_CHECK ( std::equal ( vals2, vals2 + vals2Size, res.rbegin ()));

// Copy none
  res.clear ();
  bas::copy_backward_if ( vals, vals + valsSize, std::back_inserter(res), IsGreater<100> );
  BOOST_CHECK_EQUAL ( 0U, res.size ());


// Copy_if_backward using ranges
  res.clear ();
  bas::copy_backward_if ( cont, std::back_inserter(res),  IsEven );
  BOOST_CHECK_EQUAL ( res.size (),  5U );
  BOOST_CHECK_EQUAL ( res[0], 8 );
  BOOST_CHECK_EQUAL ( res[1], 6 );
  BOOST_CHECK_EQUAL ( res[2], 4 );
  BOOST_CHECK_EQUAL ( res[3], 2 );
  BOOST_CHECK_EQUAL ( res[4], 0 );

// Copy them all
  res.clear ();
  bas::copy_backward_if ( cont, std::back_inserter(res), IsLess<100> );
  BOOST_CHECK_EQUAL ( cont.size (), res.size ());
  BOOST_CHECK ( std::equal ( cont.rbegin (), cont.rend (), res.begin ()));

// Copy none
  res.clear ();
  bas::copy_backward_if ( vals, vals + valsSize, std::back_inserter(res), IsGreater<100> );
  BOOST_CHECK_EQUAL ( 0U, res.size ());
}

void test_copy_while () 
{
  const int vals [] = { 0, 1, 0, 1, 0, 0, 0, 1, 1, 1, 1 };
  const unsigned int valsSize = sizeof ( vals ) / sizeof ( vals [0] );
  
  std::vector<int> cont;
  std::vector<int> res;

// Copy all but the last element
  std::copy ( vals, vals + valsSize - 1, std::back_inserter(cont));

// Copy_while from constant iterators
  res.clear ();
  bas::copy_while ( vals, vals + valsSize, std::back_inserter(res),  IsEqual<0> );
  BOOST_CHECK_EQUAL ( res.size (),  1U );
  BOOST_CHECK_EQUAL ( res[0], 0 );

// Copy them all
  res.clear ();
  bas::copy_while ( vals, vals + valsSize, std::back_inserter(res), IsLess<100> );
  BOOST_CHECK_EQUAL ( valsSize, res.size ());
  BOOST_CHECK ( std::equal ( vals, vals + valsSize, res.begin ()));

// Copy none
  res.clear ();
  bas::copy_while ( vals, vals + valsSize, std::back_inserter(res), IsGreater<100> );
  BOOST_CHECK_EQUAL ( 0U, res.size ());

// Copy_while using ranges
  res.clear ();
  bas::copy_while ( cont, std::back_inserter(res),  IsNotEqual<1> );
  BOOST_CHECK_EQUAL ( res.size (), 1U );
  BOOST_CHECK_EQUAL ( res[0], 0 );

// Copy them all
  res.clear ();
  bas::copy_while ( cont, std::back_inserter(res), IsNotEqual<2> );
  BOOST_CHECK_EQUAL ( cont.size (), res.size ());
  BOOST_CHECK ( std::equal ( cont.begin (), cont.end (), res.begin ()));

// Copy none
  res.clear ();
  bas::copy_while ( cont, std::back_inserter(res), IsEqual<2> );
  BOOST_CHECK_EQUAL ( 0U, res.size ());

// ---- Backwards tests ----
  const int vals2 [] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };
  const unsigned int vals2Size = sizeof ( vals2 ) / sizeof ( vals2 [0] );
  
// Copy all but the last element
  cont.clear ();
  std::copy ( vals2, vals2 + vals2Size - 1, std::back_inserter(cont));

// Copy_if_backward from constant iterators
  res.clear ();
  bas::copy_backward_while ( vals2, vals2 + vals2Size, std::back_inserter(res), IsGreater<5> );
  BOOST_CHECK_EQUAL ( res.size (), 4U );
  BOOST_CHECK_EQUAL ( res[0], 9 );
  BOOST_CHECK_EQUAL ( res[1], 8 );
  BOOST_CHECK_EQUAL ( res[2], 7 );
  BOOST_CHECK_EQUAL ( res[3], 6 );

// Copy them all
  res.clear ();
  bas::copy_backward_while ( vals2, vals2 + vals2Size, std::back_inserter(res), IsLess<100> );
  BOOST_CHECK_EQUAL ( vals2Size, res.size ());
  BOOST_CHECK ( std::equal ( vals2, vals2 + vals2Size, res.rbegin ()));

// Copy none
  res.clear ();
  bas::copy_backward_while ( vals, vals + valsSize, std::back_inserter(res), IsGreater<100> );
  BOOST_CHECK_EQUAL ( 0U, res.size ());


// Copy_while_backward using ranges
  res.clear ();
  bas::copy_backward_while ( cont, std::back_inserter(res),  IsGreater<5> );
  BOOST_CHECK_EQUAL ( res.size (), 3U );
  BOOST_CHECK_EQUAL ( res[0], 8 );
  BOOST_CHECK_EQUAL ( res[1], 7 );
  BOOST_CHECK_EQUAL ( res[2], 6 );

// Copy them all
  res.clear ();
  bas::copy_backward_while ( cont, std::back_inserter(res), IsLess<100> );
  BOOST_CHECK_EQUAL ( cont.size (), res.size ());
  BOOST_CHECK ( std::equal ( cont.rbegin (), cont.rend (), res.begin ()));

// Copy none
  res.clear ();
  bas::copy_backward_while ( vals, vals + valsSize, std::back_inserter(res), IsGreater<100> );
  BOOST_CHECK_EQUAL ( 0U, res.size ());
}

void test_copy_n () 
{
  const int vals [] = { 0, 1, 0, 1, 0, 0, 0, 1, 1, 1, 1 };
  const unsigned int valsSize = sizeof ( vals ) / sizeof ( vals [0] );
  
  std::vector<int> cont;
  std::vector<int> res;

// Copy all but the last element
  std::copy ( vals, vals + valsSize - 1, std::back_inserter(cont));

// copy_n from constant iterators
  res.clear ();
  bas::copy_n ( vals, valsSize, std::back_inserter(res));
  BOOST_CHECK_EQUAL ( res.size (),  valsSize );
  BOOST_CHECK ( std::equal ( vals, vals + valsSize, res.begin ()));

// Copy none
  res.clear ();
  bas::copy_n ( vals, 0, std::back_inserter(res));
  BOOST_CHECK_EQUAL ( 0U, res.size ());

// Copy_while from container
  res.clear ();
  bas::copy_n ( cont.begin (), cont.size (), std::back_inserter(res) );
  BOOST_CHECK_EQUAL ( res.size (), cont.size ());
  BOOST_CHECK ( std::equal ( cont.begin (), cont.end (), res.begin ()));
}


int test_main( int , char* [] )
{
  test_copy_if ();
  test_copy_while ();
  test_copy_n ();
  return 0;
}

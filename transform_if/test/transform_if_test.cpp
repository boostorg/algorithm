/*
 Copyright (C) 2008 Jesse Williamson 

 Use, modification and distribution are subject to the
 Boost Software License, Version 1.0. (See accompanying file
 LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

*/

#include <list>
#include <vector>
#include <numeric>
#include <iostream>
#include <algorithm>
#include <functional>

#include <boost/test/included/test_exec_monitor.hpp>

#include <boost/algorithm/iota.hpp>
#include <boost/assign/std/vector.hpp>

#include <boost/algorithm/transform_if.hpp>

using namespace std;

using namespace boost::assign;

using namespace boost::algorithm::sequence;

int make_double(const int& i)
{
 return i*2;
}

int mult(const int& i, const int& i2)
{
 return i*i2;
}

bool is_even(const int& i)
{
 return 0 == i % 2;
}

bool is_odd(const int& i)
{
 return !is_even(i);
}

bool both_even(const int& i, const int& i2)
{
 return is_even(i) && is_even(i2);
}

int test_main(int, char **)
{
 // transform_if(), unary op:
 {
  // ...on a vector:
  vector<int> v(3), v2;

  v += 1, 2, 3;

  transform_if(v.begin(), v.end(), back_inserter(v2), make_double, is_even);

  BOOST_CHECK(4 == accumulate(v2.begin(), v2.end(), 0)); 

  v2.clear();
 
  transform_if(v.begin(), v.end(), back_inserter(v2), make_double, is_odd);

  BOOST_CHECK(4 != accumulate(v2.begin(), v2.end(), 0)); 
  BOOST_CHECK(8 == accumulate(v2.begin(), v2.end(), 0)); 

  // ...on an array:
  int x[3], y[3];

  x[0] = 2;
  x[1] = 3;
  x[2] = 2;

  memset(&y, 0, sizeof(y));

  transform_if(x, x + 3, y, make_double, is_even);

  BOOST_CHECK(8 == accumulate(y, y + 3, 0));
 }

 // transform_if() unary op with a range:
 {
  vector<int> v(3), v2;

  v += 1, 2, 3;

  transform_if(v, back_inserter(v2), make_double, is_even); 

  BOOST_CHECK(4 == accumulate(v2.begin(), v2.end(), 0)); 
 }

 // transform_if() binary op:
 {
  // ...on a vector:
  vector<int> v(3), v2(3), v3;

  v  += 1, 2, 3;
  v2 += 1, 2, 3;

  transform_if(v.begin(), v.end(), v2.begin(), back_inserter(v3), mult, both_even);

  BOOST_CHECK(4 == accumulate(v3.begin(), v3.end(), 0));

  // ...on an array:
  int x[3], y[3], z[3];

  x[0] = 2;
  x[1] = 3;
  x[2] = 2;

  y[0] = 1;
  y[1] = 3;
  y[2] = 4; 

  memset(&z, 0, sizeof(z));

  transform_if(x, x + 3, y, z, mult, both_even);

  BOOST_CHECK(8 == accumulate(z, z + 3, 0));
 }

 // transform_if() binary with a range:
 {
  vector<int> v(3), v2(3), v3;

  v  += 1, 2, 3;
  v2 += 1, 2, 3;

  transform_if(v, v2.begin(), back_inserter(v3), mult, both_even);

  BOOST_CHECK(4 == accumulate(v3.begin(), v3.end(), 0));
 }

 return 0;
}

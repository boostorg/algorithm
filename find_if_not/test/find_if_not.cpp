/* Unit tests for Boost.algorithms.sequence::find_if_not() 
 *
 * Copyright (C) 2008, Jesse Williamson
 *
 * Use, modification, and distribution is subject to the Boost Software
 * License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
 * http://www.boost.org/LICENSE_1_0.txt)
 *
*/

#include <boost/test/included/test_exec_monitor.hpp>

#include <boost/algorithm/find_if_not.hpp>

#include <list>
#include <vector>
#include <string>

using namespace std;

using namespace boost::algorithm::sequence;

int make_zero(int x)
{
 return 0;
}

string make_z(string x)
{
 return "z";
}

int test_main(int, char **)
{
 // Basic test:
 {
   vector<int> v(3);

   v[0] = 1;
   v[1] = 2;
   v[2] = 3;

   BOOST_CHECK(v[1] == *(find_if_not(v.begin(), v.end(), std::bind2nd(std::equal_to<int>(), 1))));
 }

 // Range test:
 {
   vector<int> v(3);

   v[0] = 1;
   v[1] = 2;
   v[2] = 3;

   BOOST_CHECK(v[1] == *(find_if_not(v, std::bind2nd(std::equal_to<int>(), 1))));
 }

 return 0;
}

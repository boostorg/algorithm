/*
 Copyright (C) 2008 Jesse Williamson 

 Use, modification and distribution are subject to the
 Boost Software License, Version 1.0. (See accompanying file
 LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
*/

#include <boost/test/included/test_exec_monitor.hpp>

#include <boost/algorithm/apply.hpp>

#include <list>
#include <vector>
#include <numeric>
#include <iostream>

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

   // Should work on a single element:
   BOOST_CHECK(apply(v.begin(), v.begin(), make_zero));
   BOOST_CHECK(0 == accumulate(v.begin(), v.end(), 0));

   v[0] = 1;
   v[1] = 2;
   v[2] = 3;
   BOOST_CHECK(apply(v.begin(), v.end(), make_zero)); 
   BOOST_CHECK(0 == accumulate(v.begin(), v.end(), 0));

   // Use a conditional function (apply_if()), and do not modify the sequence:
   v[0] = 10;
   v[1] = 2;
   v[2] = 10;
   BOOST_CHECK(apply_if(v.begin(), v.end(), make_zero, std::bind1st(std::not_equal_to<int>(), 10)));
   BOOST_CHECK(20 == accumulate(v.begin(), v.end(), 0) && 0 == v[1] && 3 == v.size());
 }

 // Basic range test:
 {
   vector<int> v(128);

   generate(v.begin(), v.end(), rand);

   BOOST_CHECK(apply(v, make_zero)); 

   BOOST_CHECK(0 == accumulate(v.begin(), v.end(), 0));
 }

 {
   list<string> ls;

   const char *s[] = { "hello", "there", "world", "!\n" };	

   copy(s, s + 4, back_inserter(ls));

   apply(ls.begin(), ls.end(), make_z);

   BOOST_CHECK(accumulate(ls.begin(), ls.end(), string()) == "zzzz");
 }

 return 0;
}

//  (C) Copyright Jesse Williamson 2009
//  Use, modification and distribution are subject to the
//  Boost Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include <boost/config.hpp>
#include <boost/algorithm/all.hpp>
#include <boost/test/included/test_exec_monitor.hpp>

#include <utility>
#include <iostream>
#include <functional>

// This prettyprinter is useful when debugging:
template <class T>
void show(std::vector<T>& v)
{
 for(typename std::vector<T>::const_iterator i = v.begin(); i != v.end(); ++i)
  std::cout << *i << '\n';

 std::cout << "----------" << std::endl;
}

namespace ba = boost::algorithm;

void test_none()
{
 // Note: The literal values here are tested against directly, careful if you change them:
 int some_numbers[] = { 1, 5, 0, 18, 1 };
 std::vector<int> vi(some_numbers, some_numbers + 5);

 int some_letters[] = { 'a', 'q', 'n', 'y', 'n' };
 std::vector<char> vc(some_letters, some_letters + 5);

 BOOST_CHECK_EQUAL(true, ba::none(vi, 100));
 BOOST_CHECK_EQUAL(false, ba::none(vi, 1));

 BOOST_CHECK_EQUAL(true, ba::none(vc, 'z'));
 BOOST_CHECK_EQUAL(false, ba::none(vc, 'a'));
}

int test_main( int , char* [] )
{
  test_none();
  return 0;
}

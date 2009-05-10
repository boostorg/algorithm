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
//  Note: The literal values here are tested against directly, careful if you change them:
    int some_numbers[] = { 1, 5, 0, 18, 1 };
    std::vector<int> vi(some_numbers, some_numbers + 5);
    
    int some_letters[] = { 'a', 'q', 'n', 'y', 'n' };
    std::vector<char> vc(some_letters, some_letters + 5);
    
    BOOST_CHECK_EQUAL ( true,  ba::none_of ( vi,                         100 ));
    BOOST_CHECK_EQUAL ( true,  ba::none_of ( vi.begin(),      vi.end (), 100 ));
    BOOST_CHECK_EQUAL ( false, ba::none_of ( vi,                           1 ));
    BOOST_CHECK_EQUAL ( false, ba::none_of ( vi.begin (),     vi.end(),    1 ));

    BOOST_CHECK_EQUAL ( true,  ba::none_of  ( vi.end (),      vi.end (),   0 ));

//   5 is not in { 0, 18, 1 }, but 1 is
    BOOST_CHECK_EQUAL ( true,  ba::none_of ( vi.begin () + 2, vi.end(),    5 ));
    BOOST_CHECK_EQUAL ( false, ba::none_of ( vi.begin () + 2, vi.end(),    1 ));
//  18 is not in { 1, 5, 0 }, but 5 is
    BOOST_CHECK_EQUAL ( true,  ba::none_of ( vi.begin (), vi.begin() + 3, 18 ));
    BOOST_CHECK_EQUAL ( false, ba::none_of ( vi.begin (), vi.begin() + 3,  5 ));
    
    BOOST_CHECK_EQUAL ( true,  ba::none_of ( vc, 'z' ));
    BOOST_CHECK_EQUAL ( false, ba::none_of ( vc, 'a' ));
//  BOOST_CHECK_EQUAL ( true,  ba::none_of ( vc, 'n' ));        // Better fail!
}

void test_any ()
{
//  Note: The literal values here are tested against directly, careful if you change them:
    int some_numbers[] = { 1, 5, 0, 18, 10 };
    std::vector<int> vi(some_numbers, some_numbers + 5);
    
    int some_letters[] = { 'a', 'q', 'n', 'y', 'n' };
    std::vector<char> vc(some_letters, some_letters + 5);
    
    BOOST_CHECK_EQUAL ( true,  ba::any_of ( vi,                           1 ));
    BOOST_CHECK_EQUAL ( true,  ba::any_of ( vi.begin(),      vi.end (),   1 ));
    BOOST_CHECK_EQUAL ( false, ba::any_of ( vi,                           9 ));
    BOOST_CHECK_EQUAL ( false, ba::any_of ( vi.begin (),     vi.end(),    9 ));
    BOOST_CHECK_EQUAL ( true,  ba::any_of ( vi,                          10 ));
    BOOST_CHECK_EQUAL ( false, ba::any_of ( vi,                           4 ));
    
    BOOST_CHECK_EQUAL ( false,  ba::any_of ( vi.end (),      vi.end (),   0 ));

//   5 is not in { 0, 18, 10 }, but 10 is
    BOOST_CHECK_EQUAL ( true,  ba::any_of ( vi.begin () + 2, vi.end(),   10 ));
    BOOST_CHECK_EQUAL ( false, ba::any_of ( vi.begin () + 2, vi.end(),    5 ));
//  18 is not in { 1, 5, 0 }, but 5 is
    BOOST_CHECK_EQUAL ( true,  ba::any_of ( vi.begin (), vi.begin() + 3,  5 ));
    BOOST_CHECK_EQUAL ( false, ba::any_of ( vi.begin (), vi.begin() + 3, 18 ));

    BOOST_CHECK_EQUAL ( true,  ba::any_of (vc, 'q' ));
    BOOST_CHECK_EQUAL ( false, ba::any_of (vc, '!' ));
//  BOOST_CHECK_EQUAL ( false, ba::any_of (vc, 'n' ));      // Better fail!
}


void test_all ()
{
//  Note: The literal values here are tested against directly, careful if you change them:
    int some_numbers[] = { 1, 1, 1, 18, 10 };
    std::vector<int> vi(some_numbers, some_numbers + 5);
    
    int some_letters[] = { 'a', 'q', 'n', 'y', 'n' };
    std::vector<char> vc(some_letters, some_letters + 5);
    
    BOOST_CHECK_EQUAL ( false, ba::all_of ( vi,                           1 ));
    BOOST_CHECK_EQUAL ( false, ba::all_of ( vi.begin(),      vi.end (),   1 ));
    BOOST_CHECK_EQUAL ( false, ba::all_of ( vi,                           0 ));
    BOOST_CHECK_EQUAL ( false, ba::all_of ( vi.begin(),      vi.end (),   0 ));

    BOOST_CHECK_EQUAL ( true,  ba::all_of ( vi.end (),       vi.end (),   0));

    BOOST_CHECK_EQUAL ( true,  ba::all_of ( vi.begin (), vi.begin () + 3,   1));
    
    BOOST_CHECK_EQUAL ( true,  ba::all_of ( vc.begin () + 1, vc.begin () + 2, 'q' ));
    BOOST_CHECK_EQUAL ( false, ba::all_of(vc, '!' ));
//  BOOST_CHECK_EQUAL ( true,  ba::all_of(vc, 'n' ));       // Better fail!
}



int test_main( int , char* [] )
{
  test_none();
  test_any();
  test_all();
  return 0;
}

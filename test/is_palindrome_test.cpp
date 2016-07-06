/*
  Copyright (c) Alexander Zaitsev <zamazan4ik@gmail.com>, 2016

  Distributed under the Boost Software License, Version 1.0. (See
  accompanying file LICENSE_1_0.txt or copy at
  http://www.boost.org/LICENSE_1_0.txt)

  See http://www.boost.org/ for latest version.
*/

#include <algorithm>
#include <iostream>
#include <list>
#include <vector>

#include <boost/algorithm/is_palindrome.hpp>
#include <boost/test/included/test_exec_monitor.hpp>

namespace ba = boost::algorithm;


template <typename T>
bool funcComparator(const T& v1, const T& v2)
{
    return v1 == v2;
}

struct functorComparator
{
    template <typename T>
    bool operator()(const T& v1, const T& v2) const
    {
        return v1 == v2;
    }
};


static void test_is_palindrome()
{
    const std::list<int> empty;
    const std::vector<char> singleElement{'z'};
    int oddNonPalindrome[] = {3,2,2};
    const int evenPalindrome[] = {1,2,2,1};

    // Test a default operator==
    BOOST_CHECK ( ba::is_palindrome(empty));
    BOOST_CHECK ( ba::is_palindrome(singleElement));
    BOOST_CHECK (!ba::is_palindrome(std::begin(oddNonPalindrome), std::end(oddNonPalindrome)));
    BOOST_CHECK ( ba::is_palindrome(std::begin(evenPalindrome), std::end(evenPalindrome)));

    //Test the custom comparators
    BOOST_CHECK ( ba::is_palindrome(empty.begin(), empty.end(), functorComparator()));
    BOOST_CHECK (!ba::is_palindrome(std::begin(oddNonPalindrome), std::end(oddNonPalindrome), funcComparator<int>));
    BOOST_CHECK ( ba::is_palindrome(evenPalindrome, std::equal_to<int>()));

    //Only C++14 or newer
    //auto lambdaComparator = [](const auto& v1, const auto& v2){ return v1 == v2; };
    //BOOST_CHECK ( ba::is_palindrome(singleElement, lambdaComparator));
}

int test_main( int, char * [] )
{
    test_is_palindrome();

    return 0;
}

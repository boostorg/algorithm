//  Boost string_algo library intersperse_test.cpp file  ---------------------------//

//  Copyright Denis Mikhailov 2022. Use, modification and
//  distribution is subject to the Boost Software License, Version
//  1.0. (See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)

//  See http://www.boost.org for updates, documentation, and revision history.

#include <boost/algorithm/string/intersperse.hpp>

// Include unit test framework
#define BOOST_TEST_MAIN
#include <boost/test/unit_test.hpp>

#include <string>
#include <iostream>
#include <boost/test/test_tools.hpp>

using namespace std;
using namespace boost;

struct CharGenerator {
    typedef char result_type;
    char counter;
    CharGenerator() { counter = '1'; }
    char operator() () {
        return counter++;
    }
};

void intersperse_test()
{
    string test = "";
    intersperse(test, ',');
    BOOST_CHECK(test == "");

    test = "a";
    intersperse(test, ',');
    BOOST_CHECK(test == "a");

    test = "ab";
    intersperse(test, ',');
    BOOST_CHECK(test == "a,b");
    intersperse(test, ' ');
    BOOST_CHECK(test == "a , b");
    intersperse(test, ' ');
    BOOST_CHECK(test == "a   ,   b");

    const string input1 = "abcde";
    const string input2 = "";
    const string input3 = "a";
    const string input4 = "ab";

    BOOST_CHECK(intersperse_copy(input1, ',') == "a,b,c,d,e");
    BOOST_CHECK(intersperse_copy(input2, ',') == "");
    BOOST_CHECK(intersperse_copy(input3, ',') == "a");
    BOOST_CHECK(intersperse_copy(input4, ',') == "a,b");
}

void intersperse_fill_test()
{
    string test = "";
    intersperse_fill(test, ", ");
    BOOST_CHECK(test == "");

    test = "a";
    intersperse_fill(test, ", ");
    BOOST_CHECK(test == "a");

    test = "ab";
    intersperse_fill(test, ", ");
    BOOST_CHECK(test == "a, b");
    intersperse_fill(test, "  ");
    BOOST_CHECK(test == "a  ,     b");

    const string input1 = "abcde";
    const string input2 = "";
    const string input3 = "a";
    const string input4 = "ab";

    BOOST_CHECK(intersperse_fill_copy(input1, ", ") == "a, b, c, d, e");
    BOOST_CHECK(intersperse_fill_copy(input2, ", ") == "");
    BOOST_CHECK(intersperse_fill_copy(input3, ", ") == "a");
    BOOST_CHECK(intersperse_fill_copy(input4, ", ") == "a, b");
}

void intersperse_generate_test()
{
    const string immutable_input = "test";
    CharGenerator g1, g2;
    BOOST_CHECK(intersperse_generate_copy(immutable_input, g1)
            == "t1e2s3t");

    string mutable_input = "test";
    intersperse_generate(mutable_input, g2);
    BOOST_CHECK(mutable_input == "t1e2s3t");
}

BOOST_AUTO_TEST_CASE( test_main )
{
    intersperse_test();
    intersperse_fill_test();
    intersperse_generate_test();
}

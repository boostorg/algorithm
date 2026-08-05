#define BOOST_TEST_MODULE TestGroup1 
#include <boost/test/included/unit_test.hpp> 
#include <boost/algorithm/string/classification.hpp>
#include <boost/algorithm/string/predicate.hpp>
#include <boost/algorithm/string/trim.hpp>
#include <iostream>
#include <boost/algorithm/string.hpp>
#include <string>
#include <vector>
using namespace std;
using namespace boost::algorithm;

#include <boost/test/test_tools.hpp>

const string foo    = "foo";
const string bar    = "bar";
const string foobar = "foobar";
const string FOO    = "FOO";
const string BAR    = "BAR";
const string FOOBAR = "FOOBAR";

const wstring CharU20000 = L"\xD840\xDC00";
const wstring CharU20001 = L"\xD840\xDC01";

const wstring CharUSuppCharHigh = L"\xDC00";
const wstring CharUSuppCharLow= L"\xD840";

//test1 verify if the prefix of a string matches a specified prefix.
BOOST_AUTO_TEST_CASE(testStartsWith) {

    BOOST_CHECK(starts_with("",""));
    BOOST_CHECK(starts_with(FOOBAR,""));
    BOOST_CHECK(!starts_with("",FOO));

    BOOST_CHECK(starts_with(foobar,foo));
    BOOST_CHECK(starts_with(FOOBAR,FOO));
    BOOST_CHECK(!starts_with(foobar,FOO));
    BOOST_CHECK(!starts_with(FOOBAR,foo));

    BOOST_CHECK(!starts_with(foo,foobar));
    BOOST_CHECK(!starts_with(bar,foobar));

    BOOST_CHECK(!starts_with(foobar,bar));
    BOOST_CHECK(!starts_with(FOOBAR,BAR));
    BOOST_CHECK(!starts_with(foobar,BAR));
    BOOST_CHECK(!starts_with(FOOBAR,bar));
}

//test2 verify if it can remove leading and trailing whitespace from a string.
BOOST_AUTO_TEST_CASE(testTrim) {
    BOOST_CHECK_EQUAL(FOO, trim_copy(FOO + "  "));
    BOOST_CHECK_EQUAL(FOO, trim_copy(" " + FOO + "  "));
    BOOST_CHECK_EQUAL(FOO, trim_copy(" " + FOO));
    BOOST_CHECK_EQUAL(FOO, trim_copy(FOO + ""));
    // BOOST_CHECK_EQUAL("", trim_copy(string(" \t\r\n ")));
    // \b
    BOOST_CHECK_EQUAL("", trim_copy(string(" \t\r\n\b ")));
}


//test3 verify if a string is entirely composed of whitespace characters.
BOOST_AUTO_TEST_CASE(testIsBlank)
{
    BOOST_CHECK(!all_of("", is_space()));
    BOOST_CHECK(all_of(string(""), is_space()));
    BOOST_CHECK(!all_of("foo", is_space()));
    BOOST_CHECK(!all_of(" foo ", is_space()));
}

// test4 verify if a string is not entirely composed of whitespace characters.
BOOST_AUTO_TEST_CASE(testIsNotBlank){
    BOOST_CHECK(!all_of("a", is_space()));
    BOOST_CHECK(!all_of("foo", is_space()));
    BOOST_CHECK(!all_of(" foo ", is_space()));
}

//test5 verify if one string contains another string.
BOOST_AUTO_TEST_CASE(testContains_String) {
    BOOST_CHECK(contains("", ""));
    BOOST_CHECK(!contains("", "a"));
    BOOST_CHECK(contains("abc", "a"));
    BOOST_CHECK(contains("abc", "b"));
    BOOST_CHECK(contains("abc", "c"));
    BOOST_CHECK(contains("abc", "abc"));
    BOOST_CHECK(!contains("abc", "z"));
}


// test6 verify if a character is present in a string.
BOOST_AUTO_TEST_CASE(testContains_Char) {
    BOOST_CHECK(!contains(string(""), string(1, ' ')));
    BOOST_CHECK(contains(string("abc"), string(1, 'a')));
    BOOST_CHECK(contains(string("abc"), string(1, 'b')));
    BOOST_CHECK(contains(string("abc"), string(1, 'c')));
    BOOST_CHECK(!contains(string("abc"), string(1, 'z')));
}


//test7  verify if a string contains Unicode supplementary characters
BOOST_AUTO_TEST_CASE(testContains_StringWithSupplementaryChars) {
    BOOST_CHECK(contains(CharU20000 + CharU20001, CharU20000));
    BOOST_CHECK(contains(CharU20000 + CharU20001, CharU20001));
    BOOST_CHECK(contains(CharU20000, CharU20000));
    BOOST_CHECK(!contains(CharU20000, CharU20001));
}

//test8 verify if a string contains invalid Unicode supplementary characters.
BOOST_AUTO_TEST_CASE(testContains_StringWithBadSupplementaryChars) {
    BOOST_CHECK(!contains(CharUSuppCharHigh, CharU20001));
    BOOST_CHECK(!contains(CharUSuppCharLow, CharU20001));
    BOOST_CHECK(!contains(CharU20001, CharUSuppCharHigh));
    // 
    BOOST_CHECK(contains(CharU20001, CharUSuppCharLow));

    BOOST_CHECK(contains(CharU20001 + CharUSuppCharLow + L"a", L"a"));
    BOOST_CHECK(contains(CharU20001 + CharUSuppCharHigh + L"a", L"a"));
}


//test9 verify if two strings are equal.
BOOST_AUTO_TEST_CASE(testEquals)
{
    string fooCs = FOO;
    string barCs = BAR;
    string foobarCs = FOOBAR;

    BOOST_CHECK(equals(fooCs, fooCs));
    BOOST_CHECK(!equals(fooCs, string("foo")));

    BOOST_CHECK(iequals(fooCs, string("fOo")));
    BOOST_CHECK(!equals(fooCs, barCs));
    BOOST_CHECK(!equals(fooCs, foobarCs));
    BOOST_CHECK(!equals(foobarCs, fooCs));
}

//test10 verify if two strings are equal.
BOOST_AUTO_TEST_CASE(testEqualsOnStrings) {
    BOOST_CHECK(equals(FOO, FOO));
    BOOST_CHECK(!equals(FOO, string("foo")));
    BOOST_CHECK(!equals(FOO, string("fOo")));
    BOOST_CHECK(!equals(FOO, BAR));
    BOOST_CHECK(!equals(FOO, FOOBAR));
    BOOST_CHECK(!equals(FOOBAR, FOO));
}


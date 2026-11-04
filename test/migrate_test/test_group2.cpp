#define BOOST_TEST_MODULE TestGroup2 
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

const string TEXT_LIST_NOSEP = "foobarbaz";
const string SEPARATOR_CHAR = ";";
const string COMMA_SEPARATOR_CHAR = ",";
const string TEXT_LIST_CHAR = "foo;bar;baz";

const vector<string> ARRAY_LIST = {"foo", "bar", "baz"};
const vector<string> EMPTY_ARRAY_LIST = {};
const vector<string> MIXED_ARRAY_LIST = {"", "foo"};
const vector<string> MIXED_TYPE_LIST = {"foo", to_string(2L)};
const vector<string> PRIM_LIST = {"1", "2"};
const vector<string> STRING_LIST = {"foo", "bar", "baz"};
const vector<string> EMPTY_STRING_LIST = {};
const vector<string> ARRAY_FALSE_FALSE = {"false", "false"};
const vector<string> ARRAY_FALSE_TRUE = {"false", "true"};
const vector<string> ARRAY_FALSE_TRUE_FALSE = {"false", "true", "false"};


//test11 verify if a string is equal to any of a set of other strings.
BOOST_AUTO_TEST_CASE(testEqualsAny) {
    BOOST_CHECK(!equals(FOO, ""));
    BOOST_CHECK(!equals(FOO, string("")));

    BOOST_CHECK(equals(FOO, FOO));
    BOOST_CHECK(!equals(FOO, "foo"));
    BOOST_CHECK(!equals(FOO, "fOo"));
    BOOST_CHECK(!equals(FOO, "bar"));
}

// test12 verify if a string contains only alphabetic characters.
BOOST_AUTO_TEST_CASE(testIsAlpha) {
    BOOST_CHECK(all(string(""), is_alpha()));
   
    BOOST_CHECK(!all(string(" "), is_alpha()));
    BOOST_CHECK(all(string("a"), is_alpha()));
    BOOST_CHECK(all(string("A"), is_alpha()));
    BOOST_CHECK(all(string("kgKgKgKgkgkGkjkjlJlOKLgHdGdHgl"), is_alpha()));
    BOOST_CHECK(!all(string("ham kso"), is_alpha()));
    BOOST_CHECK(!all(string("1"), is_alpha()));
    // 6，7，87
    BOOST_CHECK(!all(string("hkHKHik6iUGHKJgU7tUJgKJGI87GIkug"), is_alpha()));
    BOOST_CHECK(!all(string("_"), is_alpha()));
    // *
    BOOST_CHECK(!all(string("hkHKHik*khbkuh"), is_alpha()));
}

//test13 verify if it can join a vector of strings using a specified delimiter.
BOOST_AUTO_TEST_CASE(testJoin_Objects) {
    BOOST_CHECK_EQUAL("abc", join(vector<string>({"a", "b", "c"}), ""));
    BOOST_CHECK_EQUAL("a", join(vector<string>({"", "", "a"}), ""));
    BOOST_CHECK(join(vector<string>({}), "").empty());
}

//test14 verify if it can join an array of strings using a specified delimiter.
BOOST_AUTO_TEST_CASE(testJoin_Objectarray) {
    BOOST_CHECK(join(vector<string>({}), "").empty());
    
    BOOST_CHECK_EQUAL("", join(vector<string>({}), ""));
    BOOST_CHECK_EQUAL("", join(vector<string>({""}), ""));
    
    BOOST_CHECK_EQUAL("", join(EMPTY_ARRAY_LIST, ""));
    BOOST_CHECK_EQUAL("abc", join(vector<string>({"a", "b", "c"}), ""));
    BOOST_CHECK_EQUAL("a", join(vector<string>({ "", "a", "" }), ""));
    BOOST_CHECK_EQUAL("foo", join(MIXED_ARRAY_LIST, ""));
    BOOST_CHECK_EQUAL("foo2", join(MIXED_TYPE_LIST, ""));
}

// test15 verify if it can join an array of strings without using a delimiter.
BOOST_AUTO_TEST_CASE(testJoin_ArrayString_EmptyDelimiter) {
    BOOST_CHECK(join(vector<string>({}), "").empty());
    
    BOOST_CHECK_EQUAL(TEXT_LIST_NOSEP, join(ARRAY_LIST, ""));
    BOOST_CHECK_EQUAL(TEXT_LIST_NOSEP, join(ARRAY_LIST, ""));
    
    BOOST_CHECK_EQUAL("", join(EMPTY_ARRAY_LIST, ""));
    BOOST_CHECK_EQUAL("", join(EMPTY_ARRAY_LIST, ""));
    
    BOOST_CHECK_EQUAL("", join(vector<string>(MIXED_ARRAY_LIST.begin(), MIXED_ARRAY_LIST.end() - 1), ""));
}

//test16 verify if it can join an array of doubles using a specified delimiter.
BOOST_AUTO_TEST_CASE(testJoin_ArrayOfDoubles) {
    BOOST_CHECK(join(vector<string>({}), "").empty());
    
    BOOST_CHECK_EQUAL("1;2", join(PRIM_LIST, SEPARATOR_CHAR));
    BOOST_CHECK("2" != join(PRIM_LIST, SEPARATOR_CHAR));
    BOOST_CHECK("" != join(PRIM_LIST, SEPARATOR_CHAR));
}

//test17 verify if it can join an array of Number using a specified delimiter.
BOOST_AUTO_TEST_CASE(testJoin_ArrayOfFloats) {
    BOOST_CHECK(join(vector<string>({}), "").empty());

    BOOST_CHECK_EQUAL("1;2", join(PRIM_LIST, SEPARATOR_CHAR));
    BOOST_CHECK("2" != join(PRIM_LIST, SEPARATOR_CHAR));
    BOOST_CHECK("" != join(PRIM_LIST, SEPARATOR_CHAR));
}

//test18 verify if it can join an array of booleans using a specified delimiter.
BOOST_AUTO_TEST_CASE(testJoin_ArrayOfBooleans) {
    BOOST_CHECK_EQUAL("false;false", join(ARRAY_FALSE_FALSE, SEPARATOR_CHAR));
    // BOOST_CHECK_EQUAL("", join("", SEPARATOR_CHAR));
    BOOST_CHECK_EQUAL("false,true,false", join(ARRAY_FALSE_TRUE_FALSE, COMMA_SEPARATOR_CHAR));
    BOOST_CHECK_EQUAL("false;true", join(ARRAY_FALSE_TRUE, SEPARATOR_CHAR));
    BOOST_CHECK("" != join(ARRAY_FALSE_FALSE, SEPARATOR_CHAR));
    BOOST_CHECK("" != join(ARRAY_FALSE_TRUE_FALSE, SEPARATOR_CHAR));
}

//test19  verify if it can join an array of shorts using a specified delimiter.
BOOST_AUTO_TEST_CASE(testJoin_ArrayOfShorts) {
    BOOST_CHECK_EQUAL("1;2", join(PRIM_LIST, SEPARATOR_CHAR));
    BOOST_CHECK("2" != join(PRIM_LIST, SEPARATOR_CHAR));
}

//test20 verify if it can join an array of strings using a specified character delimiter.
BOOST_AUTO_TEST_CASE(testJoin_ArrayCharSeparator) {
    BOOST_CHECK_EQUAL( TEXT_LIST_CHAR  ,join( ARRAY_LIST, SEPARATOR_CHAR));
    BOOST_CHECK_EQUAL("", join( EMPTY_ARRAY_LIST, SEPARATOR_CHAR));
    BOOST_CHECK_EQUAL(";foo", join( MIXED_ARRAY_LIST, SEPARATOR_CHAR));
    BOOST_CHECK_EQUAL("foo;2", join( MIXED_TYPE_LIST, SEPARATOR_CHAR));
}

//test21 verify if it can join an array of ints using a specified delimiter.
BOOST_AUTO_TEST_CASE(testJoin_ArrayOfInts) {
    BOOST_CHECK_EQUAL("1;2", join(PRIM_LIST, SEPARATOR_CHAR));
    BOOST_CHECK("2" != join(PRIM_LIST, SEPARATOR_CHAR));
}

//test22 verify if it can join a list of strings without using a delimiter. 
BOOST_AUTO_TEST_CASE(testJoin_List_EmptyDelimiter) {
    BOOST_CHECK_EQUAL(TEXT_LIST_NOSEP, join(STRING_LIST, ""));
    BOOST_CHECK_EQUAL("", join(EMPTY_STRING_LIST, ""));
 } 

//test23 verify if it can replace all occurrences of a substring in a string with a specified replacement string.
BOOST_AUTO_TEST_CASE(testreplace_all_copy_StringStringString){
    string emptyStr ="";
    string FOO ="FOO";
    string foofoofoo= "foofoofoo";
    BOOST_CHECK_EQUAL("", replace_all_copy (emptyStr, "any", "any"));
    BOOST_CHECK_EQUAL ("FOO",replace_all_copy (FOO,"","any"));
    BOOST_CHECK_EQUAL ("", replace_all_copy (foofoofoo,"foo",""));
    BOOST_CHECK_EQUAL ("barbarbar", replace_all_copy (foofoofoo,"foo","bar"));
    BOOST_CHECK_EQUAL ("farfarfar", replace_all_copy (foofoofoo,"oo","ar"));
} 

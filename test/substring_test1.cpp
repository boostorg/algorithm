/* 
   Copyright (c) Marek Kurdej 2014.

   Distributed under the Boost Software License, Version 1.0. (See accompanying
   file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

    For more information, see http://www.boost.org
*/

#include <boost/algorithm/searching/longest_increasing_substring.hpp>

#define BOOST_TEST_MAIN
#include <boost/test/unit_test.hpp>
#include <boost/test/test_tools.hpp>

#include <fstream>
#include <iostream>
#include <string>
#include <vector>

#include <boost/assign/list_of.hpp> // for 'list_of()', tuple_list_of
using namespace boost::assign; // bring 'list_of()' into scope

namespace ba = boost::algorithm;

namespace {
    template<typename Container, typename Compare>
    void check_lis ( const Container &sequence, std::size_t expected_length, Compare cmp ) {
    }

    template<typename Container, typename Compare>
    void check_length ( const Container &sequence, std::size_t expected_length, Compare cmp ) {
        typedef typename Container::const_iterator const_iterator;
        typedef typename ba::longest_increasing_substring<const_iterator>::result_type result_type;

        ba::longest_increasing_substring<const_iterator> ls(cmp);
        result_type res = ls(sequence.begin(), sequence.end());

        BOOST_CHECK_EQUAL(std::distance(res.first, res.second), expected_length);

        // check_one_iter ( haystack, needle, expected );
        // check_one_pointer ( haystack, needle, expected );
        // check_one_object ( haystack, needle, expected );
    }
    
    // template<typename Container>
    // void check_length ( const Container &sequence, std::size_t expected_length ) {
        // typedef typename Container::value_type value_type;
        // check_length ( sequence, expected_length, std::less<value_type>() );
    // }

    // template<typename Container, typename Compare>
    // void check_lis ( const Container &sequence, const Container &expected_lis, Compare cmp ) {
        // typedef typename Container::const_iterator const_iterator;
        // typedef std::vector<const_iterator> ResultContainer;

        // ba::longest_increasing_substring<const_iterator> ls(cmp);
        // ResultContainer res = ls(sequence.begin(), sequence.end());
        // Container lis;
        // lis.resize(res.size());
        // for (std::size_t i = 0; i < res.size(); ++i) {
            // lis[i] = *res[i];
        // }
        
        // BOOST_CHECK_EQUAL_COLLECTIONS(lis.begin(), lis.end(), expected_lis.begin(), expected_lis.end());
    // }

    // template<typename Container, typename Compare, typename ContainerContainer>
    // void check_lis ( const Container &sequence, const ContainerContainer &expected_lises, Compare cmp ) {
        // typedef typename Container::const_iterator const_iterator;
        // typedef std::vector<const_iterator> ResultContainer;

        // ba::longest_increasing_substring<const_iterator> ls(cmp);
        // ResultContainer res = ls(sequence.begin(), sequence.end());
        // Container lis;
        // lis.resize(res.size());
        // for (std::size_t i = 0; i < res.size(); ++i) {
            // lis[i] = *res[i];
        // }
        
        // bool any_equal = false;
        // for (std::size_t i = 0; i < expected_lises.size(); ++i) {
            // const Container &expected_lis = expected_lises[i];
            // BOOST_REQUIRE_EQUAL(lis.size(), expected_lis.size());
            // any_equal |= std::equal(lis.begin(), lis.end(), expected_lis.begin()/*, expected_lis.end()*/);
            // // try {
                // // BOOST_CHECK_EQUAL_COLLECTIONS(lis.begin(), lis.end(), expected_lis.begin(), expected_lis.end());
            // // } catch (...) {
                // // any_equal = false;
            // // }
        // }
        // BOOST_CHECK(any_equal);
    // }

    template<typename Container, typename T, typename Compare>
    void check_one ( const std::string& test_name, const Container &sequence, std::size_t expected_length, const T &expected_lis, Compare cmp )
    {
        BOOST_TEST_CHECKPOINT ( test_name );
        std::cout << "(Iterators) Sequence is " << sequence.size() << " elements long; " << std::endl;
        check_length ( sequence, expected_length, cmp );
        // BOOST_REQUIRE_EQUAL( expected_lis.size(), expected_length );
        // check_lis ( sequence, expected_lis, cmp );
    }

    template<typename Container, typename T>
    void check_one ( const std::string& test_name, const Container &sequence, std::size_t expected_length, const T &expected_lis )
    {
        typedef typename Container::value_type value_type;
        check_one ( test_name, sequence, expected_length, expected_lis, std::less<value_type>() );
    }

    // template<typename Container, typename Compare, typename ContainerContainer>
    // void check_one ( const std::string& test_name, const Container &sequence, std::size_t expected_length, const ContainerContainer &expected_lises, Compare cmp ) {
        // BOOST_TEST_CHECKPOINT ( test_name );
        // check_length ( sequence, expected_length, cmp );
        // for (std::size_t i = 0; i <  expected_lises.size(); ++i) {
            // BOOST_REQUIRE_EQUAL ( expected_lises[i].size(), expected_length );
        // }
        // check_lis ( sequence, expected_lises, cmp );
    // }

    // template<typename Container, typename ContainerContainer>
    // void check_one ( const std::string& test_name, const Container &sequence, std::size_t expected_length, const ContainerContainer &expected_lises ) {
        // typedef typename Container::value_type value_type;
        // check_one ( test_name, sequence, expected_length, expected_lises, std::less<value_type>() );
    // }

}

template <typename Container>
void ReadFromFile ( const char *name, std::back_insert_iterator<Container> inserter )
{
    std::ifstream in ( name, std::ios_base::binary | std::ios_base::in );
    std::istream_iterator<char, char> begin(in);
    std::istream_iterator<char, char> end;
    
    std::copy ( begin, end, inserter );
}

template <typename Iter>
std::string make_str ( Iter first, std::size_t len ) {
    std::string retVal ( len + 2, '\'' );
    std::copy ( first, first + len, retVal.begin() + 1);
    return retVal;
}

BOOST_AUTO_TEST_CASE( test_main )
{
    typedef std::vector<int> vec;
    typedef typename vec::const_iterator vec_const_iterator;
    typedef std::pair<vec_const_iterator, vec_const_iterator> res;
    typedef std::vector<res> resvec;
    using std::make_pair;
    using std::string;
    using std::wstring;

    vec seq0; // empty
    res exp0 = make_pair(seq0.begin(), seq0.end());
    vec seq1 = list_of(5);
    res exp1 = make_pair(seq1.begin(), seq1.end());
    vec seq2 = list_of(5) (6) (7);
    res exp2 = make_pair(seq2.begin(), seq2.end());
    vec seq3 = list_of(5) (7) (6);
    res exp3 = make_pair(seq3.begin(), seq3.begin() + 2);
    vec seq4 = list_of(7) (2) (8) (1) (3) (4) (10) (6) (9) (5);
    res exp4 = make_pair(seq4.begin() + 3, seq4.begin() + 3 + 4);
    vec seq5 = list_of(3) (2) (6) (4) (5) (1);
    res exp5a = make_pair(seq5.begin() + 1, seq5.begin() + 1 + 2);
    res exp5b = make_pair(seq5.begin() + 3, seq5.begin() + 3 + 2);
    resvec exp5 = list_of(exp5a) (exp5b);
    vec seq6 = list_of(0) (8) (4) (12) (2) (10) (6) (14) (1) (9) (5) (13) (3) (11) (7) (15);
    res exp6a = make_pair(seq6.begin(), seq6.begin() + 2);
    res exp6b = make_pair(seq6.begin() + 2, seq6.begin() + 2 + 2);
    res exp6c = make_pair(seq6.begin() + 4, seq6.begin() + 4 + 2);
    res exp6d = make_pair(seq6.begin() + 6, seq6.begin() + 6 + 2);
    res exp6e = make_pair(seq6.begin() + 8, seq6.begin() + 8 + 2);
    res exp6f = make_pair(seq6.begin() + 10, seq6.begin() + 10 + 2);
    res exp6g = make_pair(seq6.begin() + 12, seq6.begin() + 12 + 2);
    res exp6h = make_pair(seq6.begin() + 14, seq6.begin() + 14 + 2);
    resvec exp6 = list_of(exp6a) (exp6b) (exp6b) (exp6c) (exp6d) (exp6e) (exp6f) (exp6g) (exp6h);

    string str1 = "abcdef";
    string estr1 = str1;
    string str2 = "All human beings are born free and equal in dignity and rights.";
    string estr2 = " bein";
    wstring wstr1 = L"abcdef";
    wstring ewstr1 = wstr1;
    check_one("seq0", seq0, 0, exp0);
    check_one("seq1", seq1, 1, exp1);
    check_one("seq2", seq2, 3, exp2);
    check_one("seq3", seq3, 2, exp3);
    check_one("seq4", seq4, 4, exp4);
    check_one("seq5", seq5, 2, exp5);
    check_one("seq6", seq6, 2, exp6);

    check_one("str1", str1, 6, estr1);
    check_one("str2", str2, 5, estr2);
    check_one("wstr1", wstr1, 6, ewstr1);


    // string c1; ReadFromFile ( "search_test_data/0001.corpus", std::back_inserter(c1) );
    // string ec1 = "+/0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz";

    // string p1b; ReadFromFile ( "search_test_data/0001b.pat", std::back_inserter(p1b) );
    // string ep1b = "0AKPScdehlnpwxz";

    // string p1e; ReadFromFile ( "search_test_data/0001e.pat", std::back_inserter(p1e) );
    // string ep1e = "BCGHIJVdg";

    // string p1f; ReadFromFile ( "search_test_data/0001f.pat", std::back_inserter(p1f) );
    // string ep1f = "/3568BENOPQUgxz";

    // string p1n; ReadFromFile ( "search_test_data/0001n.pat", std::back_inserter(p1n) );
    // string ep1n = "013DEIMNTUY";

    // check_one("c1", c1, 64, ec1);
    // check_one("p1b", p1b, 15, ep1b);
    // check_one("p1e", p1e, 9, ep1e);
    // check_one("p1f", p1f, 15, ep1f);
    // check_one("p1n", p1n, 11, ep1n);
}

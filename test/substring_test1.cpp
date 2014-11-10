/*
   Copyright (c) Marek Kurdej 2014.

   Distributed under the Boost Software License, Version 1.0. (See accompanying
   file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

    For more information, see http://www.boost.org
*/

#include <boost/algorithm/searching/longest_increasing_substring.hpp>

#define BOOST_TEST_MAIN
#include <boost/test/unit_test.hpp>

#include <fstream>
#include <iostream>
#include <string>
#include <vector>

#include <boost/assign/list_of.hpp>  // for 'list_of()', tuple_list_of
using namespace boost::assign;       // bring 'list_of()' into scope

namespace ba = boost::algorithm;

namespace {
// Result length checking
template <typename Container, typename Compare>
void
check_length_iter ( const Container &sequence, std::size_t expected_length,
                    Compare cmp )
{
    std::size_t size = ba::longest_increasing_substring_length (
        sequence.begin (), sequence.end (), cmp );
    BOOST_CHECK_EQUAL ( size, expected_length );
}

template <typename Container, typename Compare>
void
check_length_range ( const Container &sequence, std::size_t expected_length,
                     Compare cmp )
{
    typedef typename Container::const_iterator const_iterator;
    boost::iterator_range<const_iterator> range ( sequence.begin (),
                                                  sequence.end () );
    std::size_t size = ba::longest_increasing_substring_length ( range, cmp );
    BOOST_CHECK_EQUAL ( size, expected_length );
}

template <typename Container, typename Compare>
void
check_length_pointer ( const Container &sequence, std::size_t expected_length,
                       Compare cmp )
{
    typedef const typename Container::value_type *ptr_type;
    ptr_type sBeg = sequence.size () == 0 ? NULL : &*sequence.begin ();
    ptr_type sEnd = sBeg + sequence.size ();

    std::size_t size =
        ba::longest_increasing_substring_length ( sBeg, sEnd, cmp );
    BOOST_CHECK_EQUAL ( size, expected_length );
}

template <typename Container, typename Compare>
void
check_length_object ( const Container &sequence, std::size_t expected_length,
                      Compare cmp )
{
    typedef typename Container::const_iterator const_iterator;

    ba::longest_increasing_substring<const_iterator, Compare> ls ( cmp );
    std::size_t resSize =
        ls.compute_length ( sequence.begin (), sequence.end () );
    BOOST_CHECK_EQUAL ( resSize, expected_length );
}

// Result content checking
template <typename Container, typename Compare>
void
check_lis_iter ( const Container &sequence, const Container &expected_lis,
                 Compare cmp )
{
    typedef typename Container::const_iterator const_iterator;
    typedef typename ba::longest_increasing_substring<
        const_iterator, Compare>::result_type result_type;

    result_type res = ba::longest_increasing_substring_search (
        sequence.begin (), sequence.end (), cmp );
    BOOST_CHECK_EQUAL_COLLECTIONS (
        res.first, res.second, expected_lis.begin (), expected_lis.end () );
}

template <typename Container, typename Compare>
void
check_lis_range ( const Container &sequence, const Container &expected_lis,
                  Compare cmp )
{
    typedef typename Container::const_iterator const_iterator;
    typedef boost::iterator_range<const_iterator> range_type;
    range_type range ( sequence.begin (), sequence.end () );

    range_type resRange1 =
        ba::longest_increasing_substring_search ( range, cmp );
    BOOST_CHECK_EQUAL_COLLECTIONS ( resRange1.begin (), resRange1.end (),
                                    expected_lis.begin (),
                                    expected_lis.end () );

    ba::longest_increasing_substring<const_iterator, Compare> ls2 =
        ba::make_longest_increasing_substring ( range, cmp );
    range_type resRange2 = ls2 ( range );
    BOOST_CHECK_EQUAL_COLLECTIONS ( resRange2.begin (), resRange2.end (),
                                    expected_lis.begin (),
                                    expected_lis.end () );
}

template <typename Container, typename Compare>
void
check_lis_pointer ( const Container &sequence, const Container &expected_lis,
                    Compare cmp )
{
    typedef const typename Container::value_type *ptr_type;
    typedef typename ba::longest_increasing_substring<
        ptr_type, Compare>::result_type result_type;

    ptr_type sBeg = sequence.size () == 0 ? NULL : &*sequence.begin ();
    ptr_type sEnd = sBeg + sequence.size ();
    result_type res =
        ba::longest_increasing_substring_search ( sBeg, sEnd, cmp );
    BOOST_CHECK_EQUAL_COLLECTIONS (
        res.first, res.second, expected_lis.begin (), expected_lis.end () );
}

template <typename Container, typename Compare>
void
check_lis_object ( const Container &sequence, const Container &expected_lis,
                   Compare cmp )
{
    typedef typename Container::const_iterator const_iterator;
    typedef typename ba::longest_increasing_substring<
        const_iterator, Compare>::result_type result_type;

    ba::longest_increasing_substring<const_iterator, Compare> ls ( cmp );
    result_type res = ls ( sequence.begin (), sequence.end () );
    BOOST_CHECK_EQUAL_COLLECTIONS (
        res.first, res.second, expected_lis.begin (), expected_lis.end () );
}

// Multiple possible results
template <typename Container, typename Compare>
void
check_lis ( const Container &sequence,
            const std::vector<Container> &expected_lises, Compare cmp )
{
    typedef typename Container::const_iterator const_iterator;
    typedef typename ba::longest_increasing_substring<
        const_iterator, Compare>::result_type result_type;

    ba::longest_increasing_substring<const_iterator, Compare> ls ( cmp );
    result_type res = ls ( sequence.begin (), sequence.end () );
    std::size_t res_size = std::distance ( res.first, res.second );

    bool any_equal = false;
    for ( std::size_t i = 0; i < expected_lises.size (); ++i ) {
        const Container &expected_lis = expected_lises[i];
        BOOST_CHECK_EQUAL ( res_size, expected_lis.size () );
        any_equal |=
            std::equal ( res.first, res.second,
                         expected_lis.begin () /*, expected_lis.end()*/ );
    }
    BOOST_CHECK ( any_equal );
}

// Check using iterators
template <typename Container, typename Compare>
void
check_one_iter ( const std::string &test_name, const Container &sequence,
                 std::size_t expected_length, const Container &expected_lis,
                 Compare cmp )
{
    std::cout << "(Iterators) Sequence " << test_name << " is "
              << sequence.size () << " elements long; " << std::endl;
    check_length_iter ( sequence, expected_length, cmp );
    check_lis_iter ( sequence, expected_lis, cmp );
}

// Check using ranges
template <typename Container, typename Compare>
void
check_one_range ( const std::string &test_name, const Container &sequence,
                  std::size_t expected_length, const Container &expected_lis,
                  Compare cmp )
{
    std::cout << "(Ranges) Sequence " << test_name << " is " << sequence.size ()
              << " elements long; " << std::endl;
    check_length_range ( sequence, expected_length, cmp );
    check_lis_range ( sequence, expected_lis, cmp );
}

// Check using pointers
template <typename Container, typename Compare>
void
check_one_pointer ( const std::string &test_name, const Container &sequence,
                    std::size_t expected_length, const Container &expected_lis,
                    Compare cmp )
{
    std::cout << "(Pointers) Sequence " << test_name << " is "
              << sequence.size () << " elements long; " << std::endl;
    check_length_pointer ( sequence, expected_length, cmp );
    check_lis_pointer ( sequence, expected_lis, cmp );
}

// Check using object operator()
template <typename Container, typename Compare>
void
check_one_object ( const std::string &test_name, const Container &sequence,
                   std::size_t expected_length, const Container &expected_lis,
                   Compare cmp )
{
    std::cout << "(Objects) Sequence " << test_name << " is "
              << sequence.size () << " elements long; " << std::endl;
    check_length_object ( sequence, expected_length, cmp );
    check_lis_object ( sequence, expected_lis, cmp );
}

// Check
template <typename Container, typename Compare>
void
check_one ( const std::string &test_name, const Container &sequence,
            std::size_t expected_length, const Container &expected_lis,
            Compare cmp )
{
    check_one_iter ( test_name, sequence, expected_length, expected_lis, cmp );
    check_one_range ( test_name, sequence, expected_length, expected_lis, cmp );
    check_one_pointer ( test_name, sequence, expected_length, expected_lis,
                        cmp );
    check_one_object ( test_name, sequence, expected_length, expected_lis,
                       cmp );
}

// Check without compare
// TODO: check defaults (withous giving std::less argument)
template <typename Container>
void
check_one ( const std::string &test_name, const Container &sequence,
            std::size_t expected_length, const Container &expected_lis )
{
    typedef typename Container::value_type value_type;
    check_one ( test_name, sequence, expected_length, expected_lis,
                std::less<value_type> () );
}

// Multiple possible results check
template <typename Container, typename Compare>
void
check_one ( const std::string &test_name, const Container &sequence,
            std::size_t expected_length,
            const std::vector<Container> &expected_lises, Compare cmp )
{
    check_length_iter ( sequence, expected_length, cmp );
    check_lis ( sequence, expected_lises, cmp );
}

template <typename Container>
void
check_one ( const std::string &test_name, const Container &sequence,
            std::size_t expected_length,
            const std::vector<Container> &expected_lises )
{
    typedef typename Container::value_type value_type;
    check_one ( test_name, sequence, expected_length, expected_lises,
                std::less<value_type> () );
}
}

template <typename Container>
void
ReadFromFile ( const char *name, std::back_insert_iterator<Container> inserter )
{
    std::ifstream in ( name, std::ios_base::binary | std::ios_base::in );
    std::istream_iterator<char, char> begin ( in );
    std::istream_iterator<char, char> end;

    std::copy ( begin, end, inserter );
}

BOOST_AUTO_TEST_CASE ( test_main )
{
    using std::make_pair;

    typedef std::vector<int> vec;
    typedef std::vector<vec> vecvec;
    using std::string;
    using std::wstring;

    vec seq0;          // empty
    vec exp0;          // empty
    vec rexp0 = exp0;  // empty
    vec seq1 = list_of ( 5 );
    vec exp1 = list_of ( 5 );
    vec rexp1 = exp1;
    vec seq2 = list_of ( 5 )( 6 )( 7 )( 6 );
    vec exp2 = list_of ( 5 )( 6 )( 7 );
    vec rexp2 = list_of ( 7 )( 6 );
    vec seq3 = list_of ( 5 )( 7 )( 6 );
    vec exp3 = list_of ( 5 )( 7 );
    vec rexp3 = list_of ( 7 )( 6 );
    vec seq4 = list_of ( 7 )( 5 )( 2 )( 8 )( 1 )( 3 )( 4 )( 10 )( 6 )( 9 )( 5 );
    vec exp4 = list_of ( 1 )( 3 )( 4 )( 10 );
    vec rexp4 = list_of ( 7 )( 5 )( 2 );
    vec seq5 = list_of ( 3 )( 2 )( 6 )( 4 )( 5 )( 3 )( 1 );
    vec exp5a = list_of ( 2 )( 6 );
    vec exp5b = list_of ( 4 )( 5 );
    vecvec exp5 = list_of ( exp5a )( exp5b );
    vec rexp5 = list_of ( 5 )( 3 )( 1 );
    vec seq6 = list_of ( 0 )( 5 )( 8 )( 4 )( 12 )( 2 )( 10 )( 6 )( 14 )( 1 )(
        9 )( 5 )( 13 )( 3 )( 11 )( 7 )( 15 )( 14 )( 13 );
    vec exp6 = list_of ( 0 )( 5 )( 8 );
    vec rexp6 = list_of ( 15 )( 14 )( 13 );

    check_one ( "seq0", seq0, 0, exp0 );
    check_one ( "seq1", seq1, 1, exp1 );
    check_one ( "seq2", seq2, 3, exp2 );
    check_one ( "seq3", seq3, 2, exp3 );
    check_one ( "seq4", seq4, 4, exp4 );
    check_one ( "seq5", seq5, 2, exp5 );
    check_one ( "seq6", seq6, 3, exp6 );

    std::greater<int> intCompare;
    check_one ( "rseq0", seq0, 0, rexp0, intCompare );
    check_one ( "rseq1", seq1, 1, rexp1, intCompare );
    check_one ( "rseq2", seq2, 2, rexp2, intCompare );
    check_one ( "rseq3", seq3, 2, rexp3, intCompare );
    check_one ( "rseq4", seq4, 3, rexp4, intCompare );
    check_one ( "rseq5", seq5, 3, rexp5, intCompare );
    check_one ( "rseq6", seq6, 3, rexp6, intCompare );

    string str1 = "abcdefa";
    string estr1 = "abcdef";
    string restr1 = "fa";
    string str2 =
        "All human beings are born free and equal in dignity and rights.";
    string estr2 = " bein";
    string restr2 = "uma";
    wstring wstr1 = L"abcdefa";
    wstring ewstr1 = L"abcdef";
    wstring rewstr1 = L"fa";

    check_one ( "str1", str1, 6, estr1 );
    check_one ( "str2", str2, 5, estr2 );
    check_one ( "wstr1", wstr1, 6, ewstr1 );
    std::greater<char> charCompare;
    check_one ( "rstr1", str1, 2, restr1, charCompare );
    check_one ( "rstr2", str2, 3, restr2, charCompare );
    std::greater<wchar_t> wcharCompare;
    check_one ( "rwstr1", wstr1, 2, rewstr1, wcharCompare );

    string c1;
    ReadFromFile ( "search_test_data/0001.corpus", std::back_inserter ( c1 ) );
    string ec1 = "0FGXYqtu";
    string rec1 = "mliZXB5";

    string p1b;
    ReadFromFile ( "search_test_data/0001b.pat", std::back_inserter ( p1b ) );
    string ep1b = "0AKg";
    string rep1b = "zcXV1";

    string p1e;
    ReadFromFile ( "search_test_data/0001e.pat", std::back_inserter ( p1e ) );
    string ep1e = "BJbm";
    string rep1e = "uLCB";

    string p1f;
    ReadFromFile ( "search_test_data/0001f.pat", std::back_inserter ( p1f ) );
    string ep1f = "06Ojs";
    string rep1f = "QEBA";

    string p1n;
    ReadFromFile ( "search_test_data/0001n.pat", std::back_inserter ( p1n ) );
    string ep1n = "2NTc";
    string rep1n = "jM0";

    check_one ( "c1", c1, 8, ec1 );
    check_one ( "p1b", p1b, 4, ep1b );
    check_one ( "p1e", p1e, 4, ep1e );
    check_one ( "p1f", p1f, 5, ep1f );
    check_one ( "p1n", p1n, 4, ep1n );

    check_one ( "rc1", c1, 7, rec1, charCompare );
    check_one ( "rp1b", p1b, 5, rep1b, charCompare );
    check_one ( "rp1e", p1e, 4, rep1e, charCompare );
    check_one ( "rp1f", p1f, 4, rep1f, charCompare );
    check_one ( "rp1n", p1n, 3, rep1n, charCompare );
}

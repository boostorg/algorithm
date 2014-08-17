/*
   Copyright (c) Marek Kurdej 2014.

   Distributed under the Boost Software License, Version 1.0. (See accompanying
   file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

    For more information, see http://www.boost.org
*/

#include <boost/algorithm/searching/longest_increasing_subsequence.hpp>

#define BOOST_TEST_MAIN
#include <boost/test/unit_test.hpp>

#include <boost/range/iterator_range.hpp>

#include <fstream>
#include <iostream>
#include <string>
#include <vector>

#include <boost/assign/list_of.hpp>  // for 'list_of()', tuple_list_of
using namespace boost::assign;       // bring 'list_of()' into scope

namespace ba = boost::algorithm;

struct null_insert_iterator
    : std::iterator< std::output_iterator_tag, null_insert_iterator > {
    // Null-op assignment
    template < typename T >
    void operator=( T const & )
    {
    }

    // Null-op operators
    null_insert_iterator &operator++() { return *this; }
    null_insert_iterator operator++( int ) { return *this; }
    null_insert_iterator &operator*() { return *this; }
};

namespace {

// Length checking

template < typename Container, typename Comparator >
void
check_length_iter ( const Container &sequence, std::size_t expected_length,
                    Comparator cmp )
{
    // TODO: move to separate test
    (void)ba::longest_increasing_subsequence_length (
        sequence.begin (), sequence.end () );  // check interface, ignore result
    std::size_t size = ba::longest_increasing_subsequence_length (
        sequence.begin (), sequence.end (), cmp );
    BOOST_CHECK_EQUAL ( size, expected_length );
}

template < typename Container, typename Comparator >
void
check_length_range ( const Container &sequence, std::size_t expected_length,
                     Comparator cmp )
{
    typedef typename Container::const_iterator const_iterator;
    boost::iterator_range< const_iterator > range ( sequence.begin (),
                                                    sequence.end () );
    // TODO: move to separate test
    (void)ba::longest_increasing_subsequence_length (
        range );  // check interface, ignore result
    std::size_t size = ba::longest_increasing_subsequence_length ( range, cmp );
    BOOST_CHECK_EQUAL ( size, expected_length );
    // TODO: range creator functions make_*
}

template < typename Container, typename Comparator >
void
check_length_pointer ( const Container &sequence, std::size_t expected_length,
                       Comparator cmp )
{
    typedef const typename Container::value_type *ptr_type;
    ptr_type sBeg = sequence.size () == 0 ? NULL : &*sequence.begin ();
    ptr_type sEnd = sBeg + sequence.size ();

    // TODO: move to separate test
    (void)ba::longest_increasing_subsequence_length (
        sBeg, sEnd );  // check interface, ignore result
    std::size_t size =
        ba::longest_increasing_subsequence_length ( sBeg, sEnd, cmp );
    BOOST_CHECK_EQUAL ( size, expected_length );
}

template < typename Container, typename Comparator >
void
check_length_object ( const Container &sequence, std::size_t expected_length,
                      Comparator cmp )
{
    typedef typename Container::const_iterator const_iterator;

    // TODO: move to separate test
    ba::longest_increasing_subsequence< const_iterator >
        ls_no_comparator;  // check interface, ignore result
    (void)ls_no_comparator;
    ba::longest_increasing_subsequence< const_iterator, Comparator > ls ( cmp );
    std::size_t size = ls.compute_length ( sequence.begin (), sequence.end () );
    BOOST_CHECK_EQUAL ( size, expected_length );
    // TODO: range-based make_*
}

// Content checking

template < typename Container, typename Comparator >
void
check_lis_iter ( const Container &sequence, const Container &expected_lis,
                 Comparator cmp )
{
    typedef typename Container::const_iterator const_iterator;
    typedef typename ba::longest_increasing_subsequence<
        const_iterator >::value_vector value_vector;
    typedef typename ba::longest_increasing_subsequence<
        const_iterator >::iterator_vector iterator_vector;

    // value output
    (void)ba::longest_increasing_subsequence_search (
        sequence.begin (), sequence.end (),
        ba::value_output_tag () );  // check interface
    value_vector lis_value = ba::longest_increasing_subsequence_search (
        sequence.begin (), sequence.end (), ba::value_output_tag (), cmp );
    BOOST_CHECK_EQUAL_COLLECTIONS ( lis_value.begin (), lis_value.end (),
                                    expected_lis.begin (),
                                    expected_lis.end () );

    // iterator output
    (void)ba::longest_increasing_subsequence_search (
        sequence.begin (), sequence.end (),
        ba::iterator_output_tag () );  // check interface
    iterator_vector iter = ba::longest_increasing_subsequence_search (
        sequence.begin (), sequence.end (), ba::iterator_output_tag (), cmp );
    value_vector lis_iter ( iter.size () );
    for ( std::size_t i = 0; i < iter.size (); ++i ) {
        lis_iter[i] = *iter[i];
    }
    BOOST_CHECK_EQUAL_COLLECTIONS ( lis_iter.begin (), lis_iter.end (),
                                    expected_lis.begin (),
                                    expected_lis.end () );

    // output iterator
    (void)ba::longest_increasing_subsequence_search (
        sequence.begin (), sequence.end (),
        null_insert_iterator () );  // check interface
    value_vector lis_out_iter;
    std::back_insert_iterator< value_vector > end_out_iter =
        ba::longest_increasing_subsequence_search (
            sequence.begin (), sequence.end (),
            std::back_inserter ( lis_out_iter ), cmp );
    (void)end_out_iter;  // ignore result
    BOOST_CHECK_EQUAL_COLLECTIONS ( lis_out_iter.begin (), lis_out_iter.end (),
                                    expected_lis.begin (),
                                    expected_lis.end () );
}

template < typename Container, typename Comparator >
void
check_lis_range ( const Container &sequence, const Container &expected_lis,
                  Comparator cmp )
{
    typedef typename Container::const_iterator const_iterator;
    typedef typename ba::longest_increasing_subsequence<
        const_iterator >::value_vector value_vector;
    boost::iterator_range< const_iterator > range ( sequence.begin (),
                                                    sequence.end () );
    (void)ba::longest_increasing_subsequence_search (
        range, ba::value_output_tag () );  // check interface
    value_vector lis = ba::longest_increasing_subsequence_search (
        range, ba::value_output_tag (), cmp );
    BOOST_CHECK_EQUAL_COLLECTIONS (
        lis.begin (), lis.end (), expected_lis.begin (), expected_lis.end () );

    (void)ba::longest_increasing_subsequence_search (
        range, null_insert_iterator () );  // check interface
    value_vector lis2;
    ba::longest_increasing_subsequence_search (
        range, std::back_inserter ( lis2 ), cmp );
    BOOST_CHECK_EQUAL_COLLECTIONS ( lis2.begin (), lis2.end (),
                                    expected_lis.begin (),
                                    expected_lis.end () );
}

template < typename Container, typename Comparator >
void
check_lis_pointer ( const Container &sequence, const Container &expected_lis,
                    Comparator cmp )

{
    typedef const typename Container::value_type *ptr_type;
    typedef typename ba::longest_increasing_subsequence<
        ptr_type >::value_vector value_vector;
    ptr_type sBeg = sequence.size () == 0 ? NULL : &*sequence.begin ();
    ptr_type sEnd = sBeg + sequence.size ();

    // TODO: move to separate test
    (void)ba::longest_increasing_subsequence_search (
        sBeg, sEnd,
        ba::value_output_tag () );  // check interface, ignore result
    value_vector lis = ba::longest_increasing_subsequence_search (
        sBeg, sEnd, ba::value_output_tag (), cmp );
    BOOST_CHECK_EQUAL_COLLECTIONS (
        lis.begin (), lis.end (), expected_lis.begin (), expected_lis.end () );

    (void)ba::longest_increasing_subsequence_search (
        sBeg, sEnd,
        null_insert_iterator () );  // check interface, ignore result
    value_vector lis2;
    (void)ba::longest_increasing_subsequence_search (
        sBeg, sEnd, std::back_inserter ( lis2 ), cmp );  // ignore result
    BOOST_CHECK_EQUAL_COLLECTIONS ( lis2.begin (), lis2.end (),
                                    expected_lis.begin (),
                                    expected_lis.end () );
}

template < typename Container, typename Comparator >
void
check_lis_object ( const Container &sequence, const Container &expected_lis,
                   Comparator cmp )
{
    typedef typename Container::const_iterator const_iterator;
    typedef typename ba::longest_increasing_subsequence<
        const_iterator >::value_vector value_vector;

    ba::longest_increasing_subsequence< const_iterator > ls ( cmp );
    value_vector lis =
        ls ( sequence.begin (), sequence.end (), ba::value_output_tag () );
    BOOST_CHECK_EQUAL_COLLECTIONS (
        lis.begin (), lis.end (), expected_lis.begin (), expected_lis.end () );

    value_vector lis2;
    ls ( sequence.begin (), sequence.end (), std::back_inserter ( lis2 ) );
    BOOST_CHECK_EQUAL_COLLECTIONS ( lis2.begin (), lis2.end (),
                                    expected_lis.begin (),
                                    expected_lis.end () );
    // TODO: range-based make_*
}

template < typename Container, typename Comparator >
void
check_one_iter ( const std::string &test_name, const Container &sequence,
                 std::size_t expected_length, const Container &expected_lis,
                 Comparator cmp )
{
    std::cout << "(Iterators) Sequence " << test_name << " is "
              << sequence.size () << " elements long; " << std::endl;
    check_length_iter ( sequence, expected_length, cmp );
    BOOST_REQUIRE_EQUAL ( expected_lis.size (), expected_length );
    check_lis_iter ( sequence, expected_lis, cmp );
}

template < typename Container, typename Comparator >
void
check_one_range ( const std::string &test_name, const Container &sequence,
                  std::size_t expected_length, const Container &expected_lis,
                  Comparator cmp )
{
    std::cout << "(Ranges) Sequence " << test_name << " is " << sequence.size ()
              << " elements long; " << std::endl;
    check_length_range ( sequence, expected_length, cmp );
    BOOST_REQUIRE_EQUAL ( expected_lis.size (), expected_length );
    check_lis_range ( sequence, expected_lis, cmp );
}

template < typename Container, typename Comparator >
void
check_one_pointer ( const std::string &test_name, const Container &sequence,
                    std::size_t expected_length, const Container &expected_lis,
                    Comparator cmp )
{
    std::cout << "(Pointers) Sequence " << test_name << " is "
              << sequence.size () << " elements long; " << std::endl;
    check_length_pointer ( sequence, expected_length, cmp );
    BOOST_REQUIRE_EQUAL ( expected_lis.size (), expected_length );
    check_lis_pointer ( sequence, expected_lis, cmp );
}

template < typename Container, typename Comparator >
void
check_one_object ( const std::string &test_name, const Container &sequence,
                   std::size_t expected_length, const Container &expected_lis,
                   Comparator cmp )
{
    std::cout << "(Objects) Sequence " << test_name << " is "
              << sequence.size () << " elements long; " << std::endl;
    check_length_object ( sequence, expected_length, cmp );
    BOOST_REQUIRE_EQUAL ( expected_lis.size (), expected_length );
    check_lis_object ( sequence, expected_lis, cmp );
}

template < typename Container, typename Comparator >
void
check_one ( const std::string &test_name, const Container &sequence,
            std::size_t expected_length, const Container &expected_lis,
            Comparator cmp )
{
    check_one_iter ( test_name, sequence, expected_length, expected_lis, cmp );
    check_one_range ( test_name, sequence, expected_length, expected_lis, cmp );
    check_one_pointer ( test_name, sequence, expected_length, expected_lis,
                        cmp );
    check_one_object ( test_name, sequence, expected_length, expected_lis,
                       cmp );
}

template < typename Container >
void
check_one ( const std::string &test_name, const Container &sequence,
            std::size_t expected_length, const Container &expected_lis )
{
    typedef typename Container::value_type value_type;
    // TODO: check again defaults
    // check_one ( test_name, sequence, expected_length, expected_lis );
    check_one ( test_name, sequence, expected_length, expected_lis,
                std::less< value_type > () );
}

// Multiple possible results

template < typename Container, typename Comparator,
           typename ContainerContainer >
void
check_lis ( const Container &sequence, const ContainerContainer &expected_lises,
            Comparator cmp )
{
    typedef typename Container::const_iterator const_iterator;

    ba::longest_increasing_subsequence< const_iterator > ls ( cmp );
    Container lis =
        ls ( sequence.begin (), sequence.end (), ba::value_output_tag () );

    bool any_equal = false;
    for ( std::size_t i = 0; i < expected_lises.size (); ++i ) {
        const Container &expected_lis = expected_lises[i];
        BOOST_REQUIRE_EQUAL ( lis.size (), expected_lis.size () );
        any_equal |=
            std::equal ( lis.begin (), lis.end (),
                         expected_lis.begin () /*, expected_lis.end()*/ );
        // try {
        // BOOST_CHECK_EQUAL_COLLECTIONS(lis.begin(), lis.end(),
        // expected_lis.begin(), expected_lis.end());
        // } catch (...) {
        // any_equal = false;
        // }
    }
    BOOST_CHECK ( any_equal );
}

template < typename Container, typename Comparator,
           typename ContainerContainer >
void
check_one ( const std::string &test_name, const Container &sequence,
            std::size_t expected_length,
            const ContainerContainer &expected_lises, Comparator cmp )
{
    BOOST_TEST_CHECKPOINT ( test_name );
    check_length_iter ( sequence, expected_length, cmp );
    for ( std::size_t i = 0; i < expected_lises.size (); ++i ) {
        BOOST_REQUIRE_EQUAL ( expected_lises[i].size (), expected_length );
    }
    check_lis ( sequence, expected_lises, cmp );
}

template < typename Container, typename ContainerContainer >
void
check_one ( const std::string &test_name, const Container &sequence,
            std::size_t expected_length,
            const ContainerContainer &expected_lises )
{
    typedef typename Container::value_type value_type;
    check_one ( test_name, sequence, expected_length, expected_lises,
                std::less< value_type > () );
}
}

template < typename Container >
void
ReadFromFile ( const char *name,
               std::back_insert_iterator< Container > inserter )
{
    std::ifstream in ( name, std::ios_base::binary | std::ios_base::in );
    std::istream_iterator< char, char > begin ( in );
    std::istream_iterator< char, char > end;

    std::copy ( begin, end, inserter );
}

BOOST_AUTO_TEST_CASE ( test_main )
{
    typedef std::vector< int > vec;
    typedef vec res;
    typedef std::vector< res > resvec;
    using std::string;
    using std::wstring;

    vec seq1 = list_of ( 5 );
    res exp1 = list_of ( 5 );
    vec seq2 = list_of ( 5 )( 6 )( 7 );
    res exp2 = list_of ( 5 )( 6 )( 7 );
    vec seq3 = list_of ( 5 )( 7 )( 6 );
    res exp3a = list_of ( 5 )( 6 );
    res exp3b = list_of ( 5 )( 7 );
    resvec exp3 = list_of ( exp3a )( exp3b );
    vec seq4 = list_of ( 7 )( 2 )( 8 )( 1 )( 3 )( 4 )( 10 )( 6 )( 9 )( 5 );
    res exp4 = list_of ( 1 )( 3 )( 4 )( 6 )( 9 );
    vec seq5 = list_of ( 3 )( 2 )( 6 )( 4 )( 5 )( 1 );
    res exp5a = list_of ( 2 )( 4 )( 5 );
    res exp5b = list_of ( 3 )( 4 )( 5 );
    resvec exp5 = list_of ( exp5a )( exp5b );
    vec seq6 = list_of ( 0 )( 8 )( 4 )( 12 )( 2 )( 10 )( 6 )( 14 )( 1 )( 9 )(
        5 )( 13 )( 3 )( 11 )( 7 )( 15 );
    res exp6a = list_of ( 0 )( 2 )( 6 )( 9 )( 11 )( 15 );
    res exp6b = list_of ( 0 )( 4 )( 6 )( 9 )( 13 )( 15 );
    resvec exp6 = list_of ( exp6a )( exp6b );
    string str1 = "abcdef";
    string estr1 = str1;
    string str2 =
        "All human beings are born free and equal in dignity and rights.";
    string estr2 = " abdeginrs";
    wstring wstr1 = L"abcdef";
    wstring ewstr1 = wstr1;
    check_one ( "seq1", seq1, 1, exp1 );
    check_one ( "seq2", seq2, 3, exp2 );
    check_one ( "seq3", seq3, 2, exp3 );
    check_one ( "seq5", seq4, 5, exp4 );
    check_one ( "seq5", seq5, 3, exp5 );
    check_one ( "seq6", seq6, 6, exp6 );

    check_one ( "str1", str1, 6, estr1 );
    check_one ( "str2", str2, 10, estr2 );
    check_one ( "wstr1", wstr1, 6, ewstr1 );

    string c1;
    ReadFromFile ( "search_test_data/0001.corpus", std::back_inserter ( c1 ) );
    string ec1 =
        "+/0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz";

    string p1b;
    ReadFromFile ( "search_test_data/0001b.pat", std::back_inserter ( p1b ) );
    string ep1b = "0AKPScdehlnpwxz";

    string p1e;
    ReadFromFile ( "search_test_data/0001e.pat", std::back_inserter ( p1e ) );
    string ep1e = "BCGHIJVdg";

    string p1f;
    ReadFromFile ( "search_test_data/0001f.pat", std::back_inserter ( p1f ) );
    string ep1f = "/3568BENOPQUgxz";

    string p1n;
    ReadFromFile ( "search_test_data/0001n.pat", std::back_inserter ( p1n ) );
    string ep1n = "013DEIMNTUY";

    check_one ( "c1", c1, 64, ec1 );
    check_one ( "p1b", p1b, 15, ep1b );
    check_one ( "p1e", p1e, 9, ep1e );
    check_one ( "p1f", p1f, 15, ep1f );
    check_one ( "p1n", p1n, 11, ep1n );
}

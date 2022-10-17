/* 
   Copyright (c) Denis Mikhailov 2022.

   Distributed under the Boost Software License, Version 1.0. (See accompanying
   file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

    For more information, see http://www.boost.org
*/

#include <boost/config.hpp>
#include <boost/algorithm/cxx11/move_if.hpp>

#include "iterator_test.hpp"

#define BOOST_TEST_MAIN
#include <boost/test/unit_test.hpp>

#include <algorithm>
#include <string>
#include <iostream>
#include <vector>
#include <list>
#include <functional>

#include <boost/algorithm/cxx11/all_of.hpp>
#include <boost/algorithm/cxx14/equal.hpp>
#include <boost/algorithm/cxx11/none_of.hpp>

namespace ba = boost::algorithm;
// namespace ba = boost;

struct move_test
{
    int a = -1;
    bool moved_from = false;

    move_test() = default;
    move_test(int a): a(a) {}
    move_test(const move_test&) = delete;
    move_test& operator=(const move_test&) = delete;

    move_test(move_test&& other)
    {
        if (this != &other)
        {
            *this = std::move(other);
        }
    }

    move_test& operator=(move_test&& other)
    {
        a = other.a;
        other.moved_from = true;
        moved_from = false;
        return *this;
    }
};

bool operator== (const move_test& l, const move_test& r) { return l.a == r.a; }

struct is_true { template<class T> BOOST_CXX14_CONSTEXPR bool operator() (const T&) { return true; } };
struct is_false { template<class T> BOOST_CXX14_CONSTEXPR bool operator() (const T&) { return false; } };
struct is_even {
    BOOST_CXX14_CONSTEXPR bool operator()  ( int v ) { return v % 2 == 0; }
    BOOST_CXX14_CONSTEXPR bool operator()  ( const move_test& v ) { return is_even()(v.a); }
};
struct is_odd {
    BOOST_CXX14_CONSTEXPR bool operator()   ( int v ) { return v % 2 == 1; }
    BOOST_CXX14_CONSTEXPR bool operator()   ( const move_test& v ) { return is_odd()(v.a); }
};
struct is_zero {
    BOOST_CXX14_CONSTEXPR bool operator()  ( int v ) { return v == 0; }
    BOOST_CXX14_CONSTEXPR bool operator()  ( const move_test& v ) { return is_zero()(v.a); }
};
struct is_moved {
    BOOST_CXX14_CONSTEXPR bool operator() ( int v ) { return false; }
    BOOST_CXX14_CONSTEXPR bool operator() ( const move_test& v ) { return v.moved_from != false; }
};





template <typename Container>
void test_move_while (  const std::function<Container()> make_container ) {

    typedef typename Container::value_type value_type;
    
    const Container e = make_container();
    
    
//  None of the elements
    {
        std::vector<value_type> v;
        Container c = make_container();
        ba::move_while ( c.begin (), c.end (), back_inserter ( v ), is_false());
        BOOST_CHECK( ba::none_of(c.begin (), c.end (), is_moved()) );
        BOOST_CHECK ( v.size () == 0 );
    }
    
    {
        std::vector<value_type> v;
        Container c = make_container();
        ba::move_while ( c, back_inserter ( v ), is_false());
        BOOST_CHECK( ba::none_of(c.begin (), c.end (), is_moved()) );
        BOOST_CHECK ( v.size () == 0 );
    }

//  All the elements
    {
        std::vector<value_type> v;
        Container c = make_container();
        ba::move_while ( c.begin (), c.end (), back_inserter ( v ), is_true());
        BOOST_CHECK( ba::all_of(c.begin (), c.end (), is_moved()) );
        BOOST_CHECK ( v.size () == e.size ());
        BOOST_CHECK ( std::equal ( v.begin (), v.end (), e.begin ()));
    }

    {
        std::vector<value_type> v;
        Container c = make_container();
        ba::move_while ( c, back_inserter ( v ), is_true());
        BOOST_CHECK( ba::all_of(c.begin (), c.end (), is_moved()) );
        BOOST_CHECK ( v.size () == e.size ());
        BOOST_CHECK ( std::equal ( v.begin (), v.end (), e.begin ()));
    }

//  Some of the elements
    {
        std::vector<value_type> v;
        Container c = make_container();
        auto it = ba::move_while ( c.begin (), c.end (), back_inserter ( v ), is_even() ).first;
        const std::size_t mcount = std::count_if(c.begin (), c.end (), is_moved());
        
        std::cout << "moved: " << mcount << std::endl;
        
        BOOST_CHECK ( is_even()(e.front()) ? mcount != 0 : mcount == 0 );
        BOOST_CHECK ( v.size () == (size_t) std::distance ( c.begin (), it ));
        BOOST_CHECK ( it == c.end () || !is_even() ( *it ));
        BOOST_CHECK ( ba::all_of ( v.begin (), v.end (), is_even() ));
        BOOST_CHECK ( std::equal ( v.begin (), v.end (), e.begin ()));
    }

    {
        std::vector<value_type> v;
        Container c = make_container();
        auto it = ba::move_while ( c, back_inserter ( v ), is_even() ).first;
        const std::size_t mcount = std::count_if(c.begin (), c.end (), is_moved());
        
        std::cout << "moved: " << mcount << std::endl;
        
        BOOST_CHECK ( is_even()(e.front()) ? mcount != 0 : mcount == 0 );
        BOOST_CHECK ( v.size () == (size_t) std::distance ( c.begin (), it ));
        BOOST_CHECK ( it == c.end () || !is_even() ( *it ));
        BOOST_CHECK ( ba::all_of ( v.begin (), v.end (), is_even() ));
        BOOST_CHECK ( std::equal ( v.begin (), v.end (), e.begin ()));
    }
}

template <typename Container>
void test_move_until ( const std::function<Container()> make_container ) {

    typedef typename Container::value_type value_type;

    const Container e = make_container();
    
    
//  None of the elements
    {
        std::vector<value_type> v;
        Container c = make_container();
        ba::move_until ( c.begin (), c.end (), back_inserter ( v ), is_true());
        BOOST_CHECK( ba::none_of(c.begin (), c.end (), is_moved()) );
        BOOST_CHECK ( v.size () == 0 );
    }

    {
        std::vector<value_type> v;
        Container c = make_container();
        ba::move_until ( c, back_inserter ( v ), is_true());
        BOOST_CHECK( ba::none_of(c.begin (), c.end (), is_moved()) );
        BOOST_CHECK ( v.size () == 0 );
    }

//  All the elements
    {
        std::vector<value_type> v;
        Container c = make_container();
        ba::move_until ( c.begin (), c.end (), back_inserter ( v ), is_false());
        BOOST_CHECK( ba::all_of(c.begin (), c.end (), is_moved()) );
        BOOST_CHECK ( v.size () == e.size ());
        BOOST_CHECK ( std::equal ( v.begin (), v.end (), e.begin ()));
    }

    {
        std::vector<value_type> v;
        Container c = make_container();
        ba::move_until ( c, back_inserter ( v ), is_false());
        BOOST_CHECK( ba::all_of(c.begin (), c.end (), is_moved()) );
        BOOST_CHECK ( v.size () == e.size ());
        BOOST_CHECK ( std::equal ( v.begin (), v.end (), e.begin ()));
    }

//  Some of the elements
    {
        std::vector<value_type> v;
        Container c = make_container();
        auto it = ba::move_until ( c.begin (), c.end (), back_inserter ( v ), is_even() ).first;
        const std::size_t mcount = std::count_if(c.begin (), c.end (), is_moved());
        
        std::cout << "moved: " << mcount << std::endl;
        
        BOOST_CHECK ( mcount != 0 );
        BOOST_CHECK ( v.size () == (size_t) std::distance ( c.begin (), it ));
        BOOST_CHECK ( it == c.end () || is_even() ( *it ));
        BOOST_CHECK ( ba::none_of ( v.begin (), v.end (), is_even() ));
        BOOST_CHECK ( std::equal ( v.begin (), v.end (), e.begin ()));
    }

    {
        std::vector<value_type> v;
        Container c = make_container();
        auto it = ba::move_until ( c, back_inserter ( v ), is_even() ).first;
        const std::size_t mcount = std::count_if(c.begin (), c.end (), is_moved());
        
        std::cout << "moved: " << mcount << std::endl;
        
        BOOST_CHECK ( mcount != 0 );
        BOOST_CHECK ( v.size () == (size_t) std::distance ( c.begin (), it ));
        BOOST_CHECK ( it == c.end () || is_even() ( *it ));
        BOOST_CHECK ( ba::none_of ( v.begin (), v.end (), is_even() ));
        BOOST_CHECK ( std::equal ( v.begin (), v.end (), e.begin ()));
    }
}

void test_move_while_valid_it() {
    std::vector<int> c = {2,2,4,3,3};
    
    
//  None of the elements
    {
        std::vector<int> v;
        auto it = ba::move_while ( c.begin (), c.end (), back_inserter ( v ), is_false()).first;
        BOOST_CHECK( it == c.begin() );
    }

//  All the elements
    {
        std::vector<int> v;
        auto it = ba::move_while ( c.begin (), c.end (), back_inserter ( v ), is_true()).first;
        BOOST_CHECK( it == c.end() );
    }

//  Some of the elements
    {
        std::vector<int> v;
        auto it = ba::move_while ( c.begin (), c.end (), back_inserter ( v ), is_even() ).first;
        BOOST_CHECK( it == c.begin() + 3 );
    }
}

void test_move_until_valid_it() {
    typename std::vector<int>::const_iterator it;
    
    std::vector<int> c = {1,1,2,2,3};
    
    
//  None of the elements
    {
        std::vector<int> v;
        auto it = ba::move_until ( c.begin (), c.end (), back_inserter ( v ), is_true()).first;
        BOOST_CHECK( it == c.begin() );
    }

//  All the elements
    {
        std::vector<int> v;
        auto it = ba::move_until ( c.begin (), c.end (), back_inserter ( v ), is_false()).first;
        BOOST_CHECK( it == c.end() );
    }

//  Some of the elements
    {
        std::vector<int> v;
        auto it = ba::move_until ( c.begin (), c.end (), back_inserter ( v ), is_even() ).first;
        BOOST_CHECK( it == c.begin() + 2 );
    }
}

BOOST_CXX14_CONSTEXPR inline bool constexpr_test_move_while() {
    const int sz = 64;
    int in_data[sz] = {0};
    bool res = true;

    const int* from = in_data;
    const int* to = in_data + sz;
    
    int out_data[sz] = {0};
    int* out = out_data;
    
    int etalon_data[sz] = {0};
    int* e = etalon_data;
    
    out = ba::move_while ( from, to, out, is_false() ).second; // move none
    res = (res && out == out_data && ba::all_of(out, out + sz, is_zero()));
    
    out = ba::move_while ( from, to, out, is_true() ).second; // move all
    res = (res && out == out_data + sz
           && ba::equal( input_iterator<const int *>(out_data),  input_iterator<const int *>(out_data + sz), 
                         input_iterator<const int *>(e), input_iterator<const int *>(e + sz)));
    
    return res;
    }

BOOST_CXX14_CONSTEXPR inline bool constexpr_test_move_until() {
    const int sz = 64;
    int in_data[sz] = {0};
    bool res = true;

    const int* from = in_data;
    const int* to = in_data + sz;
    
    int out_data[sz] = {0};
    int* out = out_data;
    
    int etalon_data[sz] = {0};
    int* e = etalon_data;
    
    out = ba::move_until ( from, to, out, is_true() ).second; // move none
    res = (res && out == out_data && ba::all_of(out, out + sz, is_zero()));
    
    out = ba::move_until ( from, to, out, is_false() ).second; // move all
    res = (res && out == out_data + sz
           && ba::equal( input_iterator<const int *>(out_data),  input_iterator<const int *>(out_data + sz), 
                         input_iterator<const int *>(e), input_iterator<const int *>(e + sz)));
    
    return res;
    }

std::vector<move_test> make_vector_container() {
    std::vector<move_test> v;
    for ( int i = 5; i < 15; ++i )
        v.push_back ( move_test(i) );
    return v;
}

std::list<move_test> make_list_container() {
    std::list<move_test> l;
    for ( int i = 25; i > 15; --i )
        l.push_back ( move_test(i) );
    return l;
}

void test_sequence1 () {
    test_move_while<std::vector<move_test>> ( make_vector_container );
    test_move_until<std::vector<move_test>> ( make_vector_container );

    BOOST_CXX14_CONSTEXPR bool constexpr_res_while = constexpr_test_move_while();
    BOOST_CHECK ( constexpr_res_while );
    BOOST_CXX14_CONSTEXPR bool constexpr_res_until = constexpr_test_move_until();
    BOOST_CHECK ( constexpr_res_until );

    test_move_while<std::list<move_test>> ( make_list_container );
    test_move_until<std::list<move_test>> ( make_list_container );
    
    test_move_while_valid_it();
    test_move_until_valid_it();
    }


BOOST_AUTO_TEST_CASE( test_main )
{
  test_sequence1 ();
}

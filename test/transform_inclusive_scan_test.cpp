/*
   Copyright (c) Marshall Clow 2017.

   Distributed under the Boost Software License, Version 1.0. (See accompanying
   file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

    For more information, see http://www.boost.org
*/

#include <vector>
#include <functional>
#include <numeric>
#include <iterator>
#include <cstddef>

#include <boost/config.hpp>
#include <boost/algorithm/cxx11/iota.hpp>
#include <boost/algorithm/cxx17/transform_inclusive_scan.hpp>

#include "iterator_test.hpp"

#define BOOST_TEST_MAIN
#include <boost/test/unit_test.hpp>

namespace ba = boost::algorithm;

int triangle(int n) { return n*(n+1)/2; }

template <class _Tp>
struct identity
{
    BOOST_CXX14_CONSTEXPR const _Tp& operator()(const _Tp& __x) const { return __x;}
};


template <class Iter1, class BOp, class UOp, class Iter2>
void
transform_inclusive_scan_test(Iter1 first, Iter1 last, BOp bop, UOp uop, Iter2 rFirst, Iter2 rLast)
{
    std::vector<typename std::iterator_traits<Iter1>::value_type> v;
//  Test not in-place
    ba::transform_inclusive_scan(first, last, std::back_inserter(v), bop, uop);
    BOOST_CHECK(std::distance(first, last) == v.size());
    BOOST_CHECK(std::equal(v.begin(), v.end(), rFirst));

//  Test in-place
    v.clear();
    v.assign(first, last);
    ba::transform_inclusive_scan(v.begin(), v.end(), v.begin(), bop, uop);
    BOOST_CHECK(std::distance(first, last) == v.size());
    BOOST_CHECK(std::equal(v.begin(), v.end(), rFirst));
}


template <class Iter>
void
transform_inclusive_scan_test()
{
          int ia[]     = {  1,  3,   5,   7,    9};
    const int pResI0[] = {  1,  4,   9,  16,   25};        // with identity
    const int mResI0[] = {  1,  3,  15, 105,  945};
    const int pResN0[] = { -1, -4,  -9, -16,  -25};        // with negate
    const int mResN0[] = { -1,  3, -15, 105, -945};
    const unsigned sa = sizeof(ia) / sizeof(ia[0]);
    BOOST_CHECK(sa == sizeof(pResI0) / sizeof(pResI0[0]));       // just to be sure
    BOOST_CHECK(sa == sizeof(mResI0) / sizeof(mResI0[0]));       // just to be sure
    BOOST_CHECK(sa == sizeof(pResN0) / sizeof(pResN0[0]));       // just to be sure
    BOOST_CHECK(sa == sizeof(mResN0) / sizeof(mResN0[0]));       // just to be sure

    for (unsigned int i = 0; i < sa; ++i ) {
        transform_inclusive_scan_test(Iter(ia), Iter(ia + i), std::plus<int>(),       identity<int>(),    pResI0, pResI0 + i);
        transform_inclusive_scan_test(Iter(ia), Iter(ia + i), std::multiplies<int>(), identity<int>(),    mResI0, mResI0 + i);
        transform_inclusive_scan_test(Iter(ia), Iter(ia + i), std::plus<int>(),       std::negate<int>(), pResN0, pResN0 + i);
        transform_inclusive_scan_test(Iter(ia), Iter(ia + i), std::multiplies<int>(), std::negate<int>(), mResN0, mResN0 + i);
        }
}


//  Basic sanity
void basic_transform_inclusive_scan_tests()
{
    {
    std::vector<int> v(10);
    std::fill(v.begin(), v.end(), 3);
    ba::transform_inclusive_scan(v.begin(), v.end(), v.begin(), std::plus<int>(), identity<int>());
    for (size_t i = 0; i < v.size(); ++i)
        BOOST_CHECK(v[i] == (int)(i+1) * 3);
    }

    {
    std::vector<int> v(10);
    ba::iota(v.begin(), v.end(), 0);
    ba::transform_inclusive_scan(v.begin(), v.end(), v.begin(), std::plus<int>(), identity<int>());
    for (size_t i = 0; i < v.size(); ++i)
        BOOST_CHECK(v[i] == triangle(i));
    }

    {
    std::vector<int> v(10);
    ba::iota(v.begin(), v.end(), 1);
    ba::transform_inclusive_scan(v.begin(), v.end(), v.begin(), std::plus<int>(), identity<int>());
    for (size_t i = 0; i < v.size(); ++i)
        BOOST_CHECK(v[i] == triangle(i + 1));
    }

    {
    std::vector<int> v, res;
    ba::transform_inclusive_scan(v.begin(), v.end(), std::back_inserter(res), std::plus<int>(), identity<int>());
    BOOST_CHECK(res.empty());
    }
}

BOOST_CXX14_CONSTEXPR bool constexpr_transform_inclusive_scan_tests() {
    typedef random_access_iterator<int*> iterator_t;

    const int NUM_ELEMENTS = 3;

    bool status = true;
    int input[NUM_ELEMENTS] = {3, 3, 3};
    int output[NUM_ELEMENTS] = {0, 0, 0};
    ba::transform_inclusive_scan(
        iterator_t(input), iterator_t(input + NUM_ELEMENTS),
        iterator_t(output),
        std::plus<int>(), identity<int>());
    for (size_t i = 0; i < NUM_ELEMENTS; ++i)
        status &= (output[i] == input[0] + (int)(i * 3));
    return status;
}

void test_transform_inclusive_scan()
{
    basic_transform_inclusive_scan_tests();

//  All the iterator categories
    transform_inclusive_scan_test<input_iterator        <const int*> >();
    transform_inclusive_scan_test<forward_iterator      <const int*> >();
    transform_inclusive_scan_test<bidirectional_iterator<const int*> >();
    transform_inclusive_scan_test<random_access_iterator<const int*> >();
    transform_inclusive_scan_test<const int*>();
    transform_inclusive_scan_test<      int*>();

    {
        BOOST_CXX14_CONSTEXPR bool status = constexpr_transform_inclusive_scan_tests();
        BOOST_CHECK(status == true);
    }
}


template <class Iter1, class BOp, class UOp, class T, class Iter2>
void
transform_inclusive_scan_init_single_input_test(Iter1 first, Iter1 last, BOp bop, UOp uop, T init, Iter2 rFirst, Iter2 rLast)
{
    std::vector<typename std::iterator_traits<Iter1>::value_type> v;
//  Test not in-place
    ba::transform_inclusive_scan(first, last, std::back_inserter(v), bop, uop, init);
    BOOST_CHECK(std::distance(rFirst, rLast) == v.size());
    BOOST_CHECK(std::equal(v.begin(), v.end(), rFirst));

//  Test in-place
    v.clear();
    v.assign(first, last);
    ba::transform_inclusive_scan(v.begin(), v.end(), v.begin(), bop, uop, init);
    BOOST_CHECK(std::distance(rFirst, rLast) == v.size());
    BOOST_CHECK(std::equal(v.begin(), v.end(), rFirst));
}

template <class Iter1, class Iter2, class ScanOperation, class TransformOperation, class T, class Iter3>
void
transform_inclusive_scan_init_dual_input_test(Iter1 first1, Iter1 last1,
                                              Iter2 first2,
                                              ScanOperation scan_op, TransformOperation trans_op,
                                              T init, Iter3 expected_first, Iter3 expected_last)
{
    { // Test not in-place
        std::vector<typename std::iterator_traits<Iter3>::value_type> output;
        ba::transform_inclusive_scan(first1, last1, first2, std::back_inserter(output), scan_op, trans_op, init);
        const typename std::iterator_traits<Iter3>::difference_type result_size = std::distance(expected_first, expected_last);
        BOOST_CHECK(result_size >= 0);
        BOOST_CHECK(static_cast<std::size_t>(result_size) == output.size());
        BOOST_CHECK(std::equal(output.begin(), output.end(), expected_first));
    }
    { // Test in-place
        std::vector<typename std::iterator_traits<Iter3>::value_type> v(first1, last1);
        ba::transform_inclusive_scan(v.begin(), v.end(), first2,  v.begin(), scan_op, trans_op, init);
        const typename std::iterator_traits<Iter3>::difference_type result_size = std::distance(expected_first, expected_last);
        BOOST_CHECK(result_size >= 0);
        BOOST_CHECK(static_cast<std::size_t>(result_size) == v.size());
        BOOST_CHECK(std::equal(v.begin(), v.end(), expected_first));
    }
}

template <class Iter>
void
transform_inclusive_scan_init_test()
{
    int ia1[]    = { 1, 3, 5, 7,  9};
    int ia2[]    = { 2, 4, 6, 8, 10};

    const unsigned sa = sizeof(ia1) / sizeof(ia1[0]);
    BOOST_CHECK(sa == sizeof(ia2) / sizeof(ia2[0]));  // just to be sure

    // single input results
    const int pResI0[] = {  1,  4,   9,   16,    25};        // with identity
    const int mResI0[] = {  0,  0,   0,    0,     0};
    const int pResN0[] = { -1, -4,  -9,  -16,   -25};        // with negate
    const int mResN0[] = {  0,  0,   0,    0,     0};
    const int pResI2[] = {  3,  6,  11,   18,    27};        // with identity
    const int mResI2[] = {  2,  6,  30,  210,  1890};
    const int pResN2[] = {  1, -2,  -7,  -14,   -23};        // with negate
    const int mResN2[] = { -2,  6, -30,  210, -1890};

    BOOST_CHECK(sa == sizeof(pResI0) / sizeof(pResI0[0]));       // just to be sure
    BOOST_CHECK(sa == sizeof(mResI0) / sizeof(mResI0[0]));       // just to be sure
    BOOST_CHECK(sa == sizeof(pResN0) / sizeof(pResN0[0]));       // just to be sure
    BOOST_CHECK(sa == sizeof(mResN0) / sizeof(mResN0[0]));       // just to be sure
    BOOST_CHECK(sa == sizeof(pResI2) / sizeof(pResI2[0]));       // just to be sure
    BOOST_CHECK(sa == sizeof(mResI2) / sizeof(mResI2[0]));       // just to be sure
    BOOST_CHECK(sa == sizeof(pResN2) / sizeof(pResN2[0]));       // just to be sure
    BOOST_CHECK(sa == sizeof(mResN2) / sizeof(mResN2[0]));       // just to be sure

    // dual input results
    const int pResP0[] = {  3,  10,  21,  36,  55};
    const int pResP2[] = {  5,  12,  23,  38,  57};
    const int pResM0[] = { -1,  -2,  -3,  -4,  -5};
    const int pResM2[] = {  1,   0,  -1,  -2,  -3};
    const int mResP0[] = { -3, -10, -21, -36, -55};
    const int mResP2[] = { -1,  -8, -19, -34, -53};
    const int mResM0[] = {  1,   2,   3,   4,   5};
    const int mResM2[] = {  3,   4,   5,   6,   7};

    BOOST_CHECK(sa == sizeof(pResP0) / sizeof(pResP0[0]));  // just to be sure
    BOOST_CHECK(sa == sizeof(pResP2) / sizeof(pResP2[0]));  // just to be sure
    BOOST_CHECK(sa == sizeof(pResM0) / sizeof(pResM0[0]));  // just to be sure
    BOOST_CHECK(sa == sizeof(pResM2) / sizeof(pResM2[0]));  // just to be sure
    BOOST_CHECK(sa == sizeof(mResP0) / sizeof(mResP0[0]));  // just to be sure
    BOOST_CHECK(sa == sizeof(mResP2) / sizeof(mResP2[0]));  // just to be sure
    BOOST_CHECK(sa == sizeof(mResM0) / sizeof(mResM0[0]));  // just to be sure
    BOOST_CHECK(sa == sizeof(mResM2) / sizeof(mResM2[0]));  // just to be sure

    for (unsigned int i = 0; i < sa; ++i ) {
        transform_inclusive_scan_init_single_input_test(Iter(ia1), Iter(ia1 + i), std::plus<int>(),       identity<int>(),    0, pResI0, pResI0 + i);
        transform_inclusive_scan_init_single_input_test(Iter(ia1), Iter(ia1 + i), std::multiplies<int>(), identity<int>(),    0, mResI0, mResI0 + i);
        transform_inclusive_scan_init_single_input_test(Iter(ia1), Iter(ia1 + i), std::plus<int>(),       std::negate<int>(), 0, pResN0, pResN0 + i);
        transform_inclusive_scan_init_single_input_test(Iter(ia1), Iter(ia1 + i), std::multiplies<int>(), std::negate<int>(), 0, mResN0, mResN0 + i);
        transform_inclusive_scan_init_single_input_test(Iter(ia1), Iter(ia1 + i), std::plus<int>(),       identity<int>(),    2, pResI2, pResI2 + i);
        transform_inclusive_scan_init_single_input_test(Iter(ia1), Iter(ia1 + i), std::multiplies<int>(), identity<int>(),    2, mResI2, mResI2 + i);
        transform_inclusive_scan_init_single_input_test(Iter(ia1), Iter(ia1 + i), std::plus<int>(),       std::negate<int>(), 2, pResN2, pResN2 + i);
        transform_inclusive_scan_init_single_input_test(Iter(ia1), Iter(ia1 + i), std::multiplies<int>(), std::negate<int>(), 2, mResN2, mResN2 + i);

        transform_inclusive_scan_init_dual_input_test(Iter(ia1), Iter(ia1 + i),
                                                      Iter(ia2),
                                                      std::plus<int>(), std::plus<int>(), 0,
                                                      pResP0, pResP0 + i);
        transform_inclusive_scan_init_dual_input_test(Iter(ia1), Iter(ia1 + i),
                                                      Iter(ia2),
                                                      std::plus<int>(), std::plus<int>(), 2,
                                                      pResP2, pResP2 + i);
        transform_inclusive_scan_init_dual_input_test(Iter(ia1), Iter(ia1 + i),
                                                      Iter(ia2),
                                                      std::plus<int>(), std::minus<int>(), 0,
                                                      pResM0, pResM0 + i);
        transform_inclusive_scan_init_dual_input_test(Iter(ia1), Iter(ia1 + i),
                                                      Iter(ia2),
                                                      std::plus<int>(), std::minus<int>(), 2,
                                                      pResM2, pResM2 + i);
        transform_inclusive_scan_init_dual_input_test(Iter(ia1), Iter(ia1 + i),
                                                      Iter(ia2),
                                                      std::minus<int>(), std::plus<int>(), 0,
                                                      mResP0, mResP0 + i);
        transform_inclusive_scan_init_dual_input_test(Iter(ia1), Iter(ia1 + i),
                                                      Iter(ia2),
                                                      std::minus<int>(), std::plus<int>(), 2,
                                                      mResP2, mResP2 + i);
        transform_inclusive_scan_init_dual_input_test(Iter(ia1), Iter(ia1 + i),
                                                      Iter(ia2),
                                                      std::minus<int>(), std::minus<int>(), 0,
                                                      mResM0, mResM0 + i);
        transform_inclusive_scan_init_dual_input_test(Iter(ia1), Iter(ia1 + i),
                                                      Iter(ia2),
                                                      std::minus<int>(), std::minus<int>(), 2,
                                                      mResM2, mResM2 + i);
        }
}


//  Basic sanity
void basic_transform_inclusive_scan_init_tests()
{
    {
    std::vector<int> v(10);
    std::fill(v.begin(), v.end(), 3);
    ba::transform_inclusive_scan(v.begin(), v.end(), v.begin(), std::plus<int>(), identity<int>(), 50);
    for (size_t i = 0; i < v.size(); ++i)
        BOOST_CHECK(v[i] == 50 + (int) (i + 1) * 3);
    }

    {
    std::vector<int> v(10);
    ba::iota(v.begin(), v.end(), 0);
    ba::transform_inclusive_scan(v.begin(), v.end(), v.begin(), std::plus<int>(), identity<int>(), 30);
    for (size_t i = 0; i < v.size(); ++i)
        BOOST_CHECK(v[i] == 30 + triangle(i));
    }

    {
    std::vector<int> v(10);
    ba::iota(v.begin(), v.end(), 1);
    ba::transform_inclusive_scan(v.begin(), v.end(), v.begin(), std::plus<int>(), identity<int>(), 40);
    for (size_t i = 0; i < v.size(); ++i)
        BOOST_CHECK(v[i] == 40 + triangle(i + 1));
    }

    {
    std::vector<int> v, res;
    ba::transform_inclusive_scan(v.begin(), v.end(), std::back_inserter(res), std::plus<int>(), identity<int>(), 1);
    BOOST_CHECK(res.empty());
    }
}

BOOST_CXX14_CONSTEXPR bool constexpr_transform_inclusive_scan_init_tests() {
    typedef random_access_iterator<int*> iterator_t;

    const int NUM_ELEMENTS = 3;

    bool status = true;
    { // Single input range
        int input[NUM_ELEMENTS] = {3, 3, 3};
        int output[NUM_ELEMENTS] = {0, 0, 0};
        ba::transform_inclusive_scan(
            iterator_t(input), iterator_t(input + NUM_ELEMENTS),
            iterator_t(output),
            std::plus<int>(), identity<int>(),
            30);
        for (size_t i = 0; i < NUM_ELEMENTS; ++i)
            status &= (output[i] == 30 + (int)((i + 1) * 3));
    }
    { // Dual input ranges
        int input1[NUM_ELEMENTS] = {3, 3, 3};
        int input2[NUM_ELEMENTS] = {1, 1, 1};
        int output[NUM_ELEMENTS] = {0, 0, 0};
        ba::transform_inclusive_scan(
            iterator_t(input1), iterator_t(input1 + NUM_ELEMENTS),
            iterator_t(input2),
            iterator_t(output),
            std::plus<int>(), std::minus<int>(),
            30);
        for (size_t i = 0; i < NUM_ELEMENTS; ++i)
            status &= (output[i] == 30 + (int)((i + 1) * 2));
    }
    return status;
}

void test_transform_inclusive_scan_init()
{
	basic_transform_inclusive_scan_init_tests();

//  All the iterator categories
    transform_inclusive_scan_init_test<input_iterator        <const int*> >();
    transform_inclusive_scan_init_test<forward_iterator      <const int*> >();
    transform_inclusive_scan_init_test<bidirectional_iterator<const int*> >();
    transform_inclusive_scan_init_test<random_access_iterator<const int*> >();
    transform_inclusive_scan_init_test<const int*>();
    transform_inclusive_scan_init_test<      int*>();

    {
        BOOST_CXX14_CONSTEXPR bool status = constexpr_transform_inclusive_scan_init_tests();
        BOOST_CHECK(status == true);
    }
}


BOOST_AUTO_TEST_CASE( test_main )
{
  test_transform_inclusive_scan();
  test_transform_inclusive_scan_init();
}

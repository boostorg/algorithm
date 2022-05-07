/*
   Copyright (c) T. Zachary Laine 2022.

   Distributed under the Boost Software License, Version 1.0. (See accompanying
   file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

    For more information, see http://www.boost.org
*/
#include <iostream>

#include <boost/algorithm/cxx20/shift.hpp>

#define BOOST_TEST_MAIN
#include <boost/test/unit_test.hpp>

#include <vector>
#include <list>
#include <forward_list>


namespace ba = boost::algorithm;

template<typename Container>
struct dist_t
{
    dist_t(Container & cont) : cont_(cont) {}
    template<typename Range>
    std::ptrdiff_t operator()(Range const & r) const
    {
        return std::distance(cont_.begin(), r.begin());
    }

    Container & cont_;
};

BOOST_CXX14_CONSTEXPR bool check_constexpr_shift_left()
{
    bool res = true;

    {
        int data[] = {1, 2, 3, 4, 5};
        auto result = ba::shift_left(boost::begin(data), boost::end(data), 1);
        res = (res && result.begin() == boost::begin(data));
        res = (res && result.end() == boost::end(data) - 1);
        res = (res && data[0] == 2);
    }

    {
        int data[] = {1, 2, 3, 4, 5};
        auto result = ba::shift_left(boost::begin(data), boost::end(data), 5);
        res = (res && result.begin() == boost::begin(data));
        res = (res && result.end() == boost::end(data));
        res = (res && data[0] == 1);
    }

    {
        int data[] = {1, 2, 3, 4, 5};
        auto result = ba::shift_left(boost::begin(data), boost::end(data), 4);
        res = (res && result.begin() == boost::begin(data));
        res = (res && result.end() == boost::begin(data) + 1);
        res = (res && data[0] == 5);
    }

    return res;
}

template<typename Iter>
struct sentinel
{
    friend bool operator==(Iter it, sentinel last) { return it == last.last_; }
    friend bool operator!=(Iter it, sentinel last) { return it != last.last_; }

    Iter last_;
};

void test_shift_left()
{
    {
        const std::vector<int> v = {1, 2, 3, 4, 5};

        {
            auto v1 = v;
            const auto result = ba::shift_left(v1, 0);
            const auto expected = v;
            BOOST_CHECK(result.begin() == v1.begin());
            BOOST_CHECK(result.end() == v1.end());
            BOOST_CHECK_EQUAL_COLLECTIONS(
                result.begin(), result.end(), expected.begin(), expected.end());
        }
        {
            auto v1 = v;
            const auto result = ba::shift_left(v1, 5);
            const auto expected = v;
            BOOST_CHECK(result.begin() == v1.begin());
            BOOST_CHECK(result.end() == v1.end());
            BOOST_CHECK_EQUAL_COLLECTIONS(
                result.begin(), result.end(), expected.begin(), expected.end());
        }
        {
            auto v1 = v;
            const auto result = ba::shift_left(v1, 1);
            const std::vector<int> expected = {2, 3, 4, 5};
            BOOST_CHECK(result.begin() == v1.begin());
            BOOST_CHECK(result.end() == v1.end() - 1);
            BOOST_CHECK_EQUAL_COLLECTIONS(
                result.begin(), result.end(), expected.begin(), expected.end());
        }
        {
            auto v1 = v;
            const auto result = ba::shift_left(v1, 3);
            const std::vector<int> expected = {4, 5};
            BOOST_CHECK(result.begin() == v1.begin());
            BOOST_CHECK(result.end() == v1.end() - 3);
            BOOST_CHECK_EQUAL_COLLECTIONS(
                result.begin(), result.end(), expected.begin(), expected.end());
        }
    }
    // With sentinels.
    {
        const std::vector<int> v = {1, 2, 3, 4, 5};

        {
            auto v1 = v;
            sentinel<std::vector<int>::iterator> v1_end{v1.end()};
            const auto result = ba::shift_left(v1.begin(), v1_end, 0);
            const auto expected = v;
            BOOST_CHECK(result.begin() == v1.begin());
            BOOST_CHECK(result.end() == v1.end());
            BOOST_CHECK_EQUAL_COLLECTIONS(
                result.begin(), result.end(), expected.begin(), expected.end());
        }
        {
            auto v1 = v;
            sentinel<std::vector<int>::iterator> v1_end{v1.end()};
            const auto result = ba::shift_left(v1.begin(), v1_end, 5);
            const auto expected = v;
            BOOST_CHECK(result.begin() == v1.begin());
            BOOST_CHECK(result.end() == v1.end());
            BOOST_CHECK_EQUAL_COLLECTIONS(
                result.begin(), result.end(), expected.begin(), expected.end());
        }
        {
            auto v1 = v;
            sentinel<std::vector<int>::iterator> v1_end{v1.end()};
            const auto result = ba::shift_left(v1.begin(), v1_end, 1);
            const std::vector<int> expected = {2, 3, 4, 5};
            BOOST_CHECK(result.begin() == v1.begin());
            BOOST_CHECK(result.end() == v1.end() - 1);
            BOOST_CHECK_EQUAL_COLLECTIONS(
                result.begin(), result.end(), expected.begin(), expected.end());
        }
        {
            auto v1 = v;
            sentinel<std::vector<int>::iterator> v1_end{v1.end()};
            const auto result = ba::shift_left(v1.begin(), v1_end, 3);
            const std::vector<int> expected = {4, 5};
            BOOST_CHECK(result.begin() == v1.begin());
            BOOST_CHECK(result.end() == v1.end() - 3);
            BOOST_CHECK_EQUAL_COLLECTIONS(
                result.begin(), result.end(), expected.begin(), expected.end());
        }
    }

    //  With bidirectional iterators.
    {
        const std::list<int> l = {1, 2, 3, 4, 5};

        {
            auto l1 = l;
            const auto result = ba::shift_left(l1, 0);
            const auto expected = l;
            BOOST_CHECK(result.begin() == l1.begin());
            BOOST_CHECK(result.end() == l1.end());
            BOOST_CHECK_EQUAL_COLLECTIONS(
                result.begin(), result.end(), expected.begin(), expected.end());
        }
        {
            auto l1 = l;
            const auto result = ba::shift_left(l1, 5);
            const auto expected = l;
            BOOST_CHECK(result.begin() == l1.begin());
            BOOST_CHECK(result.end() == l1.end());
            BOOST_CHECK_EQUAL_COLLECTIONS(
                result.begin(), result.end(), expected.begin(), expected.end());
        }
        {
            auto l1 = l;
            const auto result = ba::shift_left(l1, 1);
            const std::list<int> expected = {2, 3, 4, 5};
            BOOST_CHECK(result.begin() == l1.begin());
            BOOST_CHECK(result.end() == std::prev(l1.end(), 1));
            BOOST_CHECK_EQUAL_COLLECTIONS(
                result.begin(), result.end(), expected.begin(), expected.end());
        }
        {
            auto l1 = l;
            const auto result = ba::shift_left(l1, 3);
            const std::list<int> expected = {4, 5};
            BOOST_CHECK(result.begin() == l1.begin());
            BOOST_CHECK(result.end() == std::prev(l1.end(), 3));
            BOOST_CHECK_EQUAL_COLLECTIONS(
                result.begin(), result.end(), expected.begin(), expected.end());
        }
    }
    //  With bidirectional iterator/sentinel.
    {
        const std::list<int> l = {1, 2, 3, 4, 5};

        {
            auto l1 = l;
            sentinel<std::list<int>::iterator> l1_end{l1.end()};
            const auto result = ba::shift_left(l1.begin(), l1_end, 0);
            const auto expected = l;
            BOOST_CHECK(result.begin() == l1.begin());
            BOOST_CHECK(result.end() == l1.end());
            BOOST_CHECK_EQUAL_COLLECTIONS(
                result.begin(), result.end(), expected.begin(), expected.end());
        }
        {
            auto l1 = l;
            sentinel<std::list<int>::iterator> l1_end{l1.end()};
            const auto result = ba::shift_left(l1.begin(), l1_end, 5);
            const auto expected = l;
            BOOST_CHECK(result.begin() == l1.begin());
            BOOST_CHECK(result.end() == l1.end());
            BOOST_CHECK_EQUAL_COLLECTIONS(
                result.begin(), result.end(), expected.begin(), expected.end());
        }
        {
            auto l1 = l;
            sentinel<std::list<int>::iterator> l1_end{l1.end()};
            const auto result = ba::shift_left(l1.begin(), l1_end, 1);
            const std::list<int> expected = {2, 3, 4, 5};
            BOOST_CHECK(result.begin() == l1.begin());
            BOOST_CHECK(result.end() == std::prev(l1.end(), 1));
            BOOST_CHECK_EQUAL_COLLECTIONS(
                result.begin(), result.end(), expected.begin(), expected.end());
        }
        {
            auto l1 = l;
            sentinel<std::list<int>::iterator> l1_end{l1.end()};
            const auto result = ba::shift_left(l1.begin(), l1_end, 3);
            const std::list<int> expected = {4, 5};
            BOOST_CHECK(result.begin() == l1.begin());
            BOOST_CHECK(result.end() == std::prev(l1.end(), 3));
            BOOST_CHECK_EQUAL_COLLECTIONS(
                result.begin(), result.end(), expected.begin(), expected.end());
        }
    }

    //  With forward iterators.
    {
        const std::forward_list<int> l = {1, 2, 3, 4, 5};

        {
            auto l1 = l;
            const auto result = ba::shift_left(l1, 0);
            const auto expected = l;
            BOOST_CHECK(result.begin() == l1.begin());
            BOOST_CHECK(result.end() == l1.end());
            BOOST_CHECK_EQUAL_COLLECTIONS(
                result.begin(), result.end(), expected.begin(), expected.end());
        }
        {
            auto l1 = l;
            const auto result = ba::shift_left(l1, 5);
            const auto expected = l;
            BOOST_CHECK(result.begin() == l1.begin());
            BOOST_CHECK(result.end() == l1.end());
            BOOST_CHECK_EQUAL_COLLECTIONS(
                result.begin(), result.end(), expected.begin(), expected.end());
        }
        {
            auto l1 = l;
            const auto result = ba::shift_left(l1, 1);
            const std::forward_list<int> expected = {2, 3, 4, 5};
            BOOST_CHECK(result.begin() == l1.begin());
            BOOST_CHECK(result.end() == std::next(l1.begin(), 4));
            BOOST_CHECK_EQUAL_COLLECTIONS(
                result.begin(), result.end(), expected.begin(), expected.end());
        }
        {
            auto l1 = l;
            const auto result = ba::shift_left(l1, 3);
            const std::forward_list<int> expected = {4, 5};
            BOOST_CHECK(result.begin() == l1.begin());
            BOOST_CHECK(result.end() == std::next(l1.begin(), 2));
            BOOST_CHECK_EQUAL_COLLECTIONS(
                result.begin(), result.end(), expected.begin(), expected.end());
        }
    }
    //  With forward iterator/sentinel.
    {
        const std::forward_list<int> l = {1, 2, 3, 4, 5};

        {
            auto l1 = l;
            sentinel<std::forward_list<int>::iterator> l1_end{l1.end()};
            const auto result = ba::shift_left(l1.begin(), l1_end, 0);
            const auto expected = l;
            BOOST_CHECK(result.begin() == l1.begin());
            BOOST_CHECK(result.end() == l1.end());
            BOOST_CHECK_EQUAL_COLLECTIONS(
                result.begin(), result.end(), expected.begin(), expected.end());
        }
        {
            auto l1 = l;
            sentinel<std::forward_list<int>::iterator> l1_end{l1.end()};
            const auto result = ba::shift_left(l1.begin(), l1_end, 5);
            const auto expected = l;
            BOOST_CHECK(result.begin() == l1.begin());
            BOOST_CHECK(result.end() == l1.end());
            BOOST_CHECK_EQUAL_COLLECTIONS(
                result.begin(), result.end(), expected.begin(), expected.end());
        }
        {
            auto l1 = l;
            sentinel<std::forward_list<int>::iterator> l1_end{l1.end()};
            const auto result = ba::shift_left(l1.begin(), l1_end, 1);
            const std::forward_list<int> expected = {2, 3, 4, 5};
            BOOST_CHECK(result.begin() == l1.begin());
            BOOST_CHECK(result.end() == std::next(l1.begin(), 4));
            BOOST_CHECK_EQUAL_COLLECTIONS(
                result.begin(), result.end(), expected.begin(), expected.end());
        }
        {
            auto l1 = l;
            sentinel<std::forward_list<int>::iterator> l1_end{l1.end()};
            const auto result = ba::shift_left(l1.begin(), l1_end, 3);
            const std::forward_list<int> expected = {4, 5};
            BOOST_CHECK(result.begin() == l1.begin());
            BOOST_CHECK(result.end() == std::next(l1.begin(), 2));
            BOOST_CHECK_EQUAL_COLLECTIONS(
                result.begin(), result.end(), expected.begin(), expected.end());
        }
    }

    BOOST_CXX14_CONSTEXPR bool ce_result = check_constexpr_shift_left();
    BOOST_CHECK(ce_result);
}

void test_shift_right()
{
    // TODO
}

BOOST_AUTO_TEST_CASE(test_main)
{
    test_shift_left();
    test_shift_right();
}

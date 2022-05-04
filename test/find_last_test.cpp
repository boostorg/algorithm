/*
   Copyright (c) T. Zachary Laine 2022.

   Distributed under the Boost Software License, Version 1.0. (See accompanying
   file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

    For more information, see http://www.boost.org
*/
#include <iostream>

#include <boost/algorithm/find_last.hpp>

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

BOOST_CXX14_CONSTEXPR bool check_constexpr_last()
{
    int in_data[] = {1, 2, 3, 4, 5};
    bool res = true;

    const int * from = in_data;
    const int * to = in_data + 5;

    auto result = ba::find_last(from, to, 1); // stops on first
    res = (res && result.begin() == from);
    res = (res && result.end() == to);

    result = ba::find_last(in_data, 1); // stops on first
    res = (res && result.begin() == from);
    res = (res && result.end() == to);

    result = ba::find_last(from, to, 6); // stops on the end
    res = (res && result.begin() == to);
    res = (res && result.end() == to);

    result = ba::find_last(in_data, 6); // stops on the end
    res = (res && result.begin() == to);
    res = (res && result.end() == to);

    result = ba::find_last(from, to, 3); // stops on third element
    res = (res && result.begin() == in_data + 2);
    res = (res && result.end() == to);

    result = ba::find_last(in_data, 3); // stops on third element
    res = (res && result.begin() == in_data + 2);
    res = (res && result.end() == to);

    return res;
}

template<typename Iter>
struct sentinel
{
    friend bool operator==(Iter it, sentinel last) { return it == last.last_; }
    friend bool operator!=(Iter it, sentinel last) { return it != last.last_; }

    Iter last_;
};

void test_find_last()
{
    {
        std::vector<int> v1;
        const dist_t<std::vector<int>> dist(v1);

        for (int i = 5; i < 15; ++i)
            v1.push_back(i);
        BOOST_CHECK_EQUAL(
            dist(ba::find_last(v1.begin(), v1.end(), 0)), v1.size());
        BOOST_CHECK_EQUAL(
            dist(ba::find_last(v1.begin(), v1.end(), 100)), v1.size());
        BOOST_CHECK_EQUAL(
            dist(ba::find_last(v1.begin(), v1.end(), v1.back())),
            v1.size() - 1);
        BOOST_CHECK_EQUAL(
            dist(ba::find_last(v1.begin(), v1.end(), v1.front())), 0);

        BOOST_CHECK_EQUAL(dist(ba::find_last(v1, 0)), v1.size());
        BOOST_CHECK_EQUAL(dist(ba::find_last(v1, 100)), v1.size());
        BOOST_CHECK_EQUAL(dist(ba::find_last(v1, v1.back())), v1.size() - 1);
        BOOST_CHECK_EQUAL(dist(ba::find_last(v1, v1.front())), 0);
    }
    // With sentinels.
    {
        std::vector<int> v1;
        const dist_t<std::vector<int>> dist(v1);
        sentinel<std::vector<int>::iterator> v1_end;

        for (int i = 5; i < 15; ++i)
            v1.push_back(i);
        v1_end = sentinel<std::vector<int>::iterator>{v1.end()};

        BOOST_CHECK_EQUAL(
            dist(ba::find_last(v1.begin(), v1_end, 0)), v1.size());
        BOOST_CHECK_EQUAL(
            dist(ba::find_last(v1.begin(), v1_end, 100)), v1.size());
        BOOST_CHECK_EQUAL(
            dist(ba::find_last(v1.begin(), v1_end, v1.back())), v1.size() - 1);
        BOOST_CHECK_EQUAL(
            dist(ba::find_last(v1.begin(), v1_end, v1.front())), 0);

        BOOST_CHECK_EQUAL(dist(ba::find_last(v1, 0)), v1.size());
        BOOST_CHECK_EQUAL(dist(ba::find_last(v1, 100)), v1.size());
        BOOST_CHECK_EQUAL(dist(ba::find_last(v1, v1.back())), v1.size() - 1);
        BOOST_CHECK_EQUAL(dist(ba::find_last(v1, v1.front())), 0);
    }

    //  With bidirectional iterators.
    {
        std::list<int> l1;
        const dist_t<std::list<int>> dist(l1);

        for (int i = 5; i < 15; ++i)
            l1.push_back(i);
        BOOST_CHECK_EQUAL(
            dist(ba::find_last(l1.begin(), l1.end(), 0)), l1.size());
        BOOST_CHECK_EQUAL(
            dist(ba::find_last(l1.begin(), l1.end(), 100)), l1.size());
        BOOST_CHECK_EQUAL(
            dist(ba::find_last(l1.begin(), l1.end(), l1.back())),
            l1.size() - 1);
        BOOST_CHECK_EQUAL(
            dist(ba::find_last(l1.begin(), l1.end(), l1.front())), 0);

        BOOST_CHECK_EQUAL(dist(ba::find_last(l1, 0)), l1.size());
        BOOST_CHECK_EQUAL(dist(ba::find_last(l1, 100)), l1.size());
        BOOST_CHECK_EQUAL(dist(ba::find_last(l1, l1.back())), l1.size() - 1);
        BOOST_CHECK_EQUAL(dist(ba::find_last(l1, l1.front())), 0);
    }
    //  With bidirectional iterator/sentinel.
    {
        std::list<int> l1;
        const dist_t<std::list<int>> dist(l1);
        sentinel<std::list<int>::iterator> l1_end;

        for (int i = 5; i < 15; ++i)
            l1.push_back(i);
        l1_end = sentinel<std::list<int>::iterator>{l1.end()};

        BOOST_CHECK_EQUAL(
            dist(ba::find_last(l1.begin(), l1.end(), 0)), l1.size());
        BOOST_CHECK_EQUAL(
            dist(ba::find_last(l1.begin(), l1.end(), 100)), l1.size());
        BOOST_CHECK_EQUAL(
            dist(ba::find_last(l1.begin(), l1.end(), l1.back())),
            l1.size() - 1);
        BOOST_CHECK_EQUAL(
            dist(ba::find_last(l1.begin(), l1.end(), l1.front())), 0);

        BOOST_CHECK_EQUAL(dist(ba::find_last(l1, 0)), l1.size());
        BOOST_CHECK_EQUAL(dist(ba::find_last(l1, 100)), l1.size());
        BOOST_CHECK_EQUAL(dist(ba::find_last(l1, l1.back())), l1.size() - 1);
        BOOST_CHECK_EQUAL(dist(ba::find_last(l1, l1.front())), 0);
    }

    //  With forward iterators.
    {
        std::forward_list<int> l1;
        const dist_t<std::forward_list<int>> dist(l1);

        for (int i = 14; 5 <= i; --i)
            l1.push_front(i);
        const auto back = *std::next(l1.begin(), 9);

        BOOST_CHECK_EQUAL(dist(ba::find_last(l1.begin(), l1.end(), 0)), 10);
        BOOST_CHECK_EQUAL(dist(ba::find_last(l1.begin(), l1.end(), 100)), 10);
        BOOST_CHECK_EQUAL(dist(ba::find_last(l1.begin(), l1.end(), back)), 9);
        BOOST_CHECK_EQUAL(
            dist(ba::find_last(l1.begin(), l1.end(), l1.front())), 0);

        BOOST_CHECK_EQUAL(dist(ba::find_last(l1, 0)), 10);
        BOOST_CHECK_EQUAL(dist(ba::find_last(l1, 100)), 10);
        BOOST_CHECK_EQUAL(dist(ba::find_last(l1, back)), 9);
        BOOST_CHECK_EQUAL(dist(ba::find_last(l1, l1.front())), 0);
    }
    //  With forward iterator/sentinel.
    {
        std::forward_list<int> l1;
        const dist_t<std::forward_list<int>> dist(l1);
        sentinel<std::forward_list<int>::iterator> l1_end;

        for (int i = 14; 5 <= i; --i)
            l1.push_front(i);
        l1_end = sentinel<std::forward_list<int>::iterator>{l1.end()};
        const auto back = *std::next(l1.begin(), 9);

        BOOST_CHECK_EQUAL(dist(ba::find_last(l1.begin(), l1.end(), 0)), 10);
        BOOST_CHECK_EQUAL(dist(ba::find_last(l1.begin(), l1.end(), 100)), 10);
        BOOST_CHECK_EQUAL(dist(ba::find_last(l1.begin(), l1.end(), back)), 9);
        BOOST_CHECK_EQUAL(
            dist(ba::find_last(l1.begin(), l1.end(), l1.front())), 0);

        BOOST_CHECK_EQUAL(dist(ba::find_last(l1, 0)), 10);
        BOOST_CHECK_EQUAL(dist(ba::find_last(l1, 100)), 10);
        BOOST_CHECK_EQUAL(dist(ba::find_last(l1, back)), 9);
        BOOST_CHECK_EQUAL(dist(ba::find_last(l1, l1.front())), 0);
    }

    BOOST_CXX14_CONSTEXPR bool ce_result = check_constexpr_last();
    BOOST_CHECK(ce_result);
}

struct equals
{
    BOOST_CXX14_CONSTEXPR equals(int n) : n_(n) {}
    BOOST_CXX14_CONSTEXPR bool operator()(int i) const { return i == n_; }
    int n_;
};

BOOST_CXX14_CONSTEXPR bool check_constexpr_if_last()
{
    int in_data[] = {1, 2, 3, 4, 5};
    bool res = true;

    const int * from = in_data;
    const int * to = in_data + 5;

    auto result = ba::find_last_if(from, to, equals(1)); // stops on first
    res = (res && result.begin() == from);
    res = (res && result.end() == to);

    result = ba::find_last_if(in_data, equals(1)); // stops on first
    res = (res && result.begin() == from);
    res = (res && result.end() == to);

    result = ba::find_last_if(from, to, equals(6)); // stops on the end
    res = (res && result.begin() == to);
    res = (res && result.end() == to);

    result = ba::find_last_if(in_data, equals(6)); // stops on the end
    res = (res && result.begin() == to);
    res = (res && result.end() == to);

    result = ba::find_last_if(from, to, equals(3)); // stops on third element
    res = (res && result.begin() == in_data + 2);
    res = (res && result.end() == to);

    result = ba::find_last_if(in_data, equals(3)); // stops on third element
    res = (res && result.begin() == in_data + 2);
    res = (res && result.end() == to);

    return res;
}

void test_find_last_if()
{
    {
        std::vector<int> v1;
        const dist_t<std::vector<int>> dist(v1);

        for (int i = 5; i < 15; ++i)
            v1.push_back(i);
        BOOST_CHECK_EQUAL(
            dist(ba::find_last_if(v1.begin(), v1.end(), equals(0))), v1.size());
        BOOST_CHECK_EQUAL(
            dist(ba::find_last_if(v1.begin(), v1.end(), equals(100))),
            v1.size());
        BOOST_CHECK_EQUAL(
            dist(ba::find_last_if(v1.begin(), v1.end(), equals(v1.back()))),
            v1.size() - 1);
        BOOST_CHECK_EQUAL(
            dist(ba::find_last_if(v1.begin(), v1.end(), equals(v1.front()))),
            0);

        BOOST_CHECK_EQUAL(dist(ba::find_last_if(v1, equals(0))), v1.size());
        BOOST_CHECK_EQUAL(dist(ba::find_last_if(v1, equals(100))), v1.size());
        BOOST_CHECK_EQUAL(
            dist(ba::find_last_if(v1, equals(v1.back()))), v1.size() - 1);
        BOOST_CHECK_EQUAL(dist(ba::find_last_if(v1, equals(v1.front()))), 0);
    }

    //  With bidirectional iterators.
    {
        std::list<int> l1;
        const dist_t<std::list<int>> dist(l1);

        for (int i = 5; i < 15; ++i)
            l1.push_back(i);
        BOOST_CHECK_EQUAL(
            dist(ba::find_last_if(l1.begin(), l1.end(), equals(0))), l1.size());
        BOOST_CHECK_EQUAL(
            dist(ba::find_last_if(l1.begin(), l1.end(), equals(100))),
            l1.size());
        BOOST_CHECK_EQUAL(
            dist(ba::find_last_if(l1.begin(), l1.end(), equals(l1.back()))),
            l1.size() - 1);
        BOOST_CHECK_EQUAL(
            dist(ba::find_last_if(l1.begin(), l1.end(), equals(l1.front()))),
            0);

        BOOST_CHECK_EQUAL(dist(ba::find_last_if(l1, equals(0))), l1.size());
        BOOST_CHECK_EQUAL(dist(ba::find_last_if(l1, equals(100))), l1.size());
        BOOST_CHECK_EQUAL(
            dist(ba::find_last_if(l1, equals(l1.back()))), l1.size() - 1);
        BOOST_CHECK_EQUAL(dist(ba::find_last_if(l1, equals(l1.front()))), 0);
    }

    //  With forward iterators.
    {
        std::forward_list<int> l1;
        const dist_t<std::forward_list<int>> dist(l1);

        for (int i = 14; 5 <= i; --i)
            l1.push_front(i);
        const auto back = *std::next(l1.begin(), 9);

        BOOST_CHECK_EQUAL(
            dist(ba::find_last_if(l1.begin(), l1.end(), equals(0))), 10);
        BOOST_CHECK_EQUAL(
            dist(ba::find_last_if(l1.begin(), l1.end(), equals(100))), 10);
        BOOST_CHECK_EQUAL(
            dist(ba::find_last_if(l1.begin(), l1.end(), equals(back))), 9);
        BOOST_CHECK_EQUAL(
            dist(ba::find_last_if(l1.begin(), l1.end(), equals(l1.front()))),
            0);

        BOOST_CHECK_EQUAL(dist(ba::find_last_if(l1, equals(0))), 10);
        BOOST_CHECK_EQUAL(dist(ba::find_last_if(l1, equals(100))), 10);
        BOOST_CHECK_EQUAL(dist(ba::find_last_if(l1, equals(back))), 9);
        BOOST_CHECK_EQUAL(dist(ba::find_last_if(l1, equals(l1.front()))), 0);
    }

    BOOST_CXX14_CONSTEXPR bool ce_result = check_constexpr_if_last();
    BOOST_CHECK(ce_result);
}

struct not_equals
{
    BOOST_CXX14_CONSTEXPR not_equals(int n) : n_(n) {}
    BOOST_CXX14_CONSTEXPR bool operator()(int i) const { return i != n_; }
    int n_;
};

BOOST_CXX14_CONSTEXPR bool check_constexpr_if_not_last()
{
    int in_data[] = {1, 2, 3, 4, 5};
    bool res = true;

    const int * from = in_data;
    const int * to = in_data + 5;

    auto result =
        ba::find_last_if_not(from, to, not_equals(1)); // stops on first
    res = (res && result.begin() == from);
    res = (res && result.end() == to);

    result = ba::find_last_if_not(in_data, not_equals(1)); // stops on first
    res = (res && result.begin() == from);
    res = (res && result.end() == to);

    result = ba::find_last_if_not(from, to, not_equals(6)); // stops on the end
    res = (res && result.begin() == to);
    res = (res && result.end() == to);

    result = ba::find_last_if_not(in_data, not_equals(6)); // stops on the end
    res = (res && result.begin() == to);
    res = (res && result.end() == to);

    result =
        ba::find_last_if_not(from, to, not_equals(3)); // stops on third element
    res = (res && result.begin() == in_data + 2);
    res = (res && result.end() == to);

    result =
        ba::find_last_if_not(in_data, not_equals(3)); // stops on third element
    res = (res && result.begin() == in_data + 2);
    res = (res && result.end() == to);

    return res;
}

void test_find_last_if_not()
{
    {
        std::vector<int> v1;
        const dist_t<std::vector<int>> dist(v1);

        for (int i = 5; i < 15; ++i)
            v1.push_back(i);
        BOOST_CHECK_EQUAL(
            dist(ba::find_last_if_not(v1.begin(), v1.end(), not_equals(0))),
            v1.size());
        BOOST_CHECK_EQUAL(
            dist(ba::find_last_if_not(v1.begin(), v1.end(), not_equals(100))),
            v1.size());
        BOOST_CHECK_EQUAL(
            dist(ba::find_last_if_not(
                v1.begin(), v1.end(), not_equals(v1.back()))),
            v1.size() - 1);
        BOOST_CHECK_EQUAL(
            dist(ba::find_last_if_not(
                v1.begin(), v1.end(), not_equals(v1.front()))),
            0);

        BOOST_CHECK_EQUAL(
            dist(ba::find_last_if_not(v1, not_equals(0))), v1.size());
        BOOST_CHECK_EQUAL(
            dist(ba::find_last_if_not(v1, not_equals(100))), v1.size());
        BOOST_CHECK_EQUAL(
            dist(ba::find_last_if_not(v1, not_equals(v1.back()))),
            v1.size() - 1);
        BOOST_CHECK_EQUAL(
            dist(ba::find_last_if_not(v1, not_equals(v1.front()))), 0);
    }

    //  With bidirectional iterators.
    {
        std::list<int> l1;
        const dist_t<std::list<int>> dist(l1);

        for (int i = 5; i < 15; ++i)
            l1.push_back(i);
        BOOST_CHECK_EQUAL(
            dist(ba::find_last_if_not(l1.begin(), l1.end(), not_equals(0))),
            l1.size());
        BOOST_CHECK_EQUAL(
            dist(ba::find_last_if_not(l1.begin(), l1.end(), not_equals(100))),
            l1.size());
        BOOST_CHECK_EQUAL(
            dist(ba::find_last_if_not(
                l1.begin(), l1.end(), not_equals(l1.back()))),
            l1.size() - 1);
        BOOST_CHECK_EQUAL(
            dist(ba::find_last_if_not(
                l1.begin(), l1.end(), not_equals(l1.front()))),
            0);

        BOOST_CHECK_EQUAL(
            dist(ba::find_last_if_not(l1, not_equals(0))), l1.size());
        BOOST_CHECK_EQUAL(
            dist(ba::find_last_if_not(l1, not_equals(100))), l1.size());
        BOOST_CHECK_EQUAL(
            dist(ba::find_last_if_not(l1, not_equals(l1.back()))),
            l1.size() - 1);
        BOOST_CHECK_EQUAL(
            dist(ba::find_last_if_not(l1, not_equals(l1.front()))), 0);
    }

    //  With forward iterators.
    {
        std::forward_list<int> l1;
        const dist_t<std::forward_list<int>> dist(l1);

        for (int i = 14; 5 <= i; --i)
            l1.push_front(i);
        const auto back = *std::next(l1.begin(), 9);

        BOOST_CHECK_EQUAL(
            dist(ba::find_last_if_not(l1.begin(), l1.end(), not_equals(0))),
            10);
        BOOST_CHECK_EQUAL(
            dist(ba::find_last_if_not(l1.begin(), l1.end(), not_equals(100))),
            10);
        BOOST_CHECK_EQUAL(
            dist(ba::find_last_if_not(l1.begin(), l1.end(), not_equals(back))),
            9);
        BOOST_CHECK_EQUAL(
            dist(ba::find_last_if_not(
                l1.begin(), l1.end(), not_equals(l1.front()))),
            0);

        BOOST_CHECK_EQUAL(dist(ba::find_last_if_not(l1, not_equals(0))), 10);
        BOOST_CHECK_EQUAL(dist(ba::find_last_if_not(l1, not_equals(100))), 10);
        BOOST_CHECK_EQUAL(dist(ba::find_last_if_not(l1, not_equals(back))), 9);
        BOOST_CHECK_EQUAL(
            dist(ba::find_last_if_not(l1, not_equals(l1.front()))), 0);
    }

    BOOST_CXX14_CONSTEXPR bool ce_result = check_constexpr_if_not_last();
    BOOST_CHECK(ce_result);
}

BOOST_AUTO_TEST_CASE(test_main)
{
    test_find_last();
    test_find_last_if();
    test_find_last_if_not();
}

/*
   Copyright (c) T. Zachary Laine 2022.

   Distributed under the Boost Software License, Version 1.0. (See accompanying
   file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

    For more information, see http://www.boost.org
*/
#include <iostream>

#include <boost/algorithm/cxx23/contains.hpp>

#define BOOST_TEST_MAIN
#include <boost/test/unit_test.hpp>

#include <vector>
#include <list>
#include <forward_list>


namespace ba = boost::algorithm;

BOOST_CXX14_CONSTEXPR bool check_constexpr_contains()
{
    int data[] = {1, 2, 3, 4, 5};
    int contained = 2;
    int uncontained = 6;
    bool res = true;

    res &= ba::contains(std::begin(data), std::end(data), contained);
    res &= !ba::contains(std::begin(data), std::end(data), uncontained);
    res &= ba::contains(data, contained);
    res &= !ba::contains(data, uncontained);

    return res;
}

template<typename Iter>
struct sentinel
{
    friend bool operator==(Iter it, sentinel last) { return it == last.last_; }
    friend bool operator!=(Iter it, sentinel last) { return it != last.last_; }

    Iter last_;
};

void test_contains()
{
    {
        std::vector<int> v1 = {0, 1, 3};

        BOOST_CHECK(ba::contains(v1, 0));
        BOOST_CHECK(!ba::contains(v1, 2));
    }
    // With sentinels.
    {
        std::vector<int> v1 = {0, 1, 3};
        sentinel<std::vector<int>::iterator> v1_end{v1.end()};

        BOOST_CHECK(ba::contains(v1.begin(), v1_end, 0));
        BOOST_CHECK(!ba::contains(v1.begin(), v1_end, 2));
    }

    //  With bidirectional iterators.
    {
        std::list<int> l1 = {0, 1, 3};

        BOOST_CHECK(ba::contains(l1, 0));
        BOOST_CHECK(!ba::contains(l1, 2));

    }
    //  With bidirectional iterator/sentinel.
    {
        std::list<int> l1 = {0, 1, 3};
        sentinel<std::list<int>::iterator> l1_end{l1.end()};

        BOOST_CHECK(ba::contains(l1.begin(), l1_end, 0));
        BOOST_CHECK(!ba::contains(l1.begin(), l1_end, 2));
    }

    //  With forward iterators.
    {
        std::forward_list<int> l1 = {0, 1, 3};

        BOOST_CHECK(ba::contains(l1, 0));
        BOOST_CHECK(!ba::contains(l1, 2));
    }
    //  With forward iterator/sentinel.
    {
        std::forward_list<int> l1 = {0, 1, 3};
        sentinel<std::forward_list<int>::iterator> l1_end{l1.end()};

        BOOST_CHECK(ba::contains(l1.begin(), l1_end, 0));
        BOOST_CHECK(!ba::contains(l1.begin(), l1_end, 2));
    }

    BOOST_CXX14_CONSTEXPR bool ce_result = check_constexpr_contains();
    BOOST_CHECK(ce_result);
}

BOOST_CXX14_CONSTEXPR bool check_constexpr_contains_subrange()
{
    int data[] = {1, 2, 3, 4, 5};
    int end_elements[] = {4, 5};
    int begin_elements[] = {1, 2};
    int other_elements[] = {2, 3};
    int * empty_begin = nullptr;
    int * empty_end = nullptr;
    int uncontained_elements[] = {5, 4};
    bool res = true;

    res &= ba::contains_subrange(data, end_elements);
    res &= ba::contains_subrange(data, begin_elements);
    res &= ba::contains_subrange(data, other_elements);
    res &= ba::contains_subrange(
        boost::begin(data), boost::end(data), empty_begin, empty_end);
    res &= !ba::contains_subrange(data, uncontained_elements);

    return res;
}

struct equals
{
    BOOST_CXX14_CONSTEXPR equals(int n) : n_(n) {}
    BOOST_CXX14_CONSTEXPR bool operator()(int i) const { return i == n_; }
    int n_;
};

void test_contains_subrange()
{
    {
        std::vector<int> data = {1, 2, 2, 3, 4, 5};
        std::vector<int> end_elements = {4, 5};
        std::vector<int> begin_elements = {1, 2};
        std::vector<int> other_elements = {2, 3};
        std::vector<int> empty;
        std::vector<int> uncontained_elements = {5, 4};

        BOOST_CHECK(ba::contains_subrange(data, end_elements));
        BOOST_CHECK(ba::contains_subrange(data, begin_elements));
        BOOST_CHECK(ba::contains_subrange(data, other_elements));
        BOOST_CHECK(ba::contains_subrange(
            boost::begin(data),
            boost::end(data),
            boost::begin(empty),
            boost::begin(empty)));
        BOOST_CHECK(!ba::contains_subrange(data, uncontained_elements));
    }
    // With sentinels.
    {
        std::vector<int> data = {1, 2, 2, 3, 4, 5};
        sentinel<std::vector<int>::iterator> data_end{data.end()};
        std::vector<int> end_elements = {4, 5};
        sentinel<std::vector<int>::iterator> end_elements_end{
            end_elements.end()};
        std::vector<int> begin_elements = {1, 2};
        sentinel<std::vector<int>::iterator> begin_elements_end{
            begin_elements.end()};
        std::vector<int> other_elements = {2, 3};
        sentinel<std::vector<int>::iterator> other_elements_end{
            other_elements.end()};
        std::vector<int> empty;
        sentinel<std::vector<int>::iterator> empty_end{empty.end()};
        std::vector<int> uncontained_elements = {5, 4};
        sentinel<std::vector<int>::iterator> uncontained_elements_end{
            uncontained_elements.end()};

        BOOST_CHECK(ba::contains_subrange(
            boost::begin(data),
            data_end,
            boost::begin(end_elements),
            end_elements_end));
        BOOST_CHECK(ba::contains_subrange(
            boost::begin(data),
            data_end,
            boost::begin(begin_elements),
            begin_elements_end));
        BOOST_CHECK(ba::contains_subrange(
            boost::begin(data),
            data_end,
            boost::begin(other_elements),
            other_elements_end));
        BOOST_CHECK(ba::contains_subrange(
            boost::begin(data),
            data_end,
            boost::begin(empty),
            empty_end));
        BOOST_CHECK(!ba::contains_subrange(
            boost::begin(data),
            data_end,
            boost::begin(uncontained_elements),
            uncontained_elements_end));
    }

    //  With bidirectional iterators.
    {
        std::list<int> data = {1, 2, 2, 3, 4, 5};
        std::list<int> end_elements = {4, 5};
        std::list<int> begin_elements = {1, 2};
        std::list<int> other_elements = {2, 3};
        std::list<int> empty;
        std::list<int> uncontained_elements = {5, 4};

        BOOST_CHECK(ba::contains_subrange(data, end_elements));
        BOOST_CHECK(ba::contains_subrange(data, begin_elements));
        BOOST_CHECK(ba::contains_subrange(data, other_elements));
        BOOST_CHECK(ba::contains_subrange(
            boost::begin(data),
            boost::end(data),
            boost::begin(empty),
            boost::begin(empty)));
        BOOST_CHECK(!ba::contains_subrange(data, uncontained_elements));
    }
    //  With bidirectional iterator/sentinel.
    {
        std::list<int> data = {1, 2, 2, 3, 4, 5};
        sentinel<std::list<int>::iterator> data_end{data.end()};
        std::list<int> end_elements = {4, 5};
        sentinel<std::list<int>::iterator> end_elements_end{
            end_elements.end()};
        std::list<int> begin_elements = {1, 2};
        sentinel<std::list<int>::iterator> begin_elements_end{
            begin_elements.end()};
        std::list<int> other_elements = {2, 3};
        sentinel<std::list<int>::iterator> other_elements_end{
            other_elements.end()};
        std::list<int> empty;
        sentinel<std::list<int>::iterator> empty_end{empty.end()};
        std::list<int> uncontained_elements = {5, 4};
        sentinel<std::list<int>::iterator> uncontained_elements_end{
            uncontained_elements.end()};

        BOOST_CHECK(ba::contains_subrange(
            boost::begin(data),
            data_end,
            boost::begin(end_elements),
            end_elements_end));
        BOOST_CHECK(ba::contains_subrange(
            boost::begin(data),
            data_end,
            boost::begin(begin_elements),
            begin_elements_end));
        BOOST_CHECK(ba::contains_subrange(
            boost::begin(data),
            data_end,
            boost::begin(other_elements),
            other_elements_end));
        BOOST_CHECK(ba::contains_subrange(
            boost::begin(data),
            data_end,
            boost::begin(empty),
            empty_end));
        BOOST_CHECK(!ba::contains_subrange(
            boost::begin(data),
            data_end,
            boost::begin(uncontained_elements),
            uncontained_elements_end));
    }

    //  With forward iterators.
    {
        std::forward_list<int> data = {1, 2, 2, 3, 4, 5};
        std::forward_list<int> end_elements = {4, 5};
        std::forward_list<int> begin_elements = {1, 2};
        std::forward_list<int> other_elements = {2, 3};
        std::forward_list<int> empty;
        std::forward_list<int> uncontained_elements = {5, 4};

        BOOST_CHECK(ba::contains_subrange(data, end_elements));
        BOOST_CHECK(ba::contains_subrange(data, begin_elements));
        BOOST_CHECK(ba::contains_subrange(data, other_elements));
        BOOST_CHECK(ba::contains_subrange(
            boost::begin(data),
            boost::end(data),
            boost::begin(empty),
            boost::begin(empty)));
        BOOST_CHECK(!ba::contains_subrange(data, uncontained_elements));
    }
    //  With forward iterator/sentinel.
    {
        std::forward_list<int> data = {1, 2, 2, 3, 4, 5};
        sentinel<std::forward_list<int>::iterator> data_end{data.end()};
        std::forward_list<int> end_elements = {4, 5};
        sentinel<std::forward_list<int>::iterator> end_elements_end{
            end_elements.end()};
        std::forward_list<int> begin_elements = {1, 2};
        sentinel<std::forward_list<int>::iterator> begin_elements_end{
            begin_elements.end()};
        std::forward_list<int> other_elements = {2, 3};
        sentinel<std::forward_list<int>::iterator> other_elements_end{
            other_elements.end()};
        std::forward_list<int> empty;
        sentinel<std::forward_list<int>::iterator> empty_end{empty.end()};
        std::forward_list<int> uncontained_elements = {5, 4};
        sentinel<std::forward_list<int>::iterator> uncontained_elements_end{
            uncontained_elements.end()};

        BOOST_CHECK(ba::contains_subrange(
            boost::begin(data),
            data_end,
            boost::begin(end_elements),
            end_elements_end));
        BOOST_CHECK(ba::contains_subrange(
            boost::begin(data),
            data_end,
            boost::begin(begin_elements),
            begin_elements_end));
        BOOST_CHECK(ba::contains_subrange(
            boost::begin(data),
            data_end,
            boost::begin(other_elements),
            other_elements_end));
        BOOST_CHECK(ba::contains_subrange(
            boost::begin(data),
            data_end,
            boost::begin(empty),
            empty_end));
        BOOST_CHECK(!ba::contains_subrange(
            boost::begin(data),
            data_end,
            boost::begin(uncontained_elements),
            uncontained_elements_end));
    }

    BOOST_CXX14_CONSTEXPR bool ce_result = check_constexpr_contains_subrange();
    BOOST_CHECK(ce_result);
}

BOOST_AUTO_TEST_CASE(test_main)
{
    test_contains();
    test_contains_subrange();
}

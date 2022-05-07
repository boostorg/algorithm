/*
   Copyright (c) T. Zachary Laine 2022.

   Distributed under the Boost Software License, Version 1.0. (See accompanying
   file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

    For more information, see http://www.boost.org
*/
#ifndef BOOST_ALGORITHM_STARTS_CONTAINS_HPP
#define BOOST_ALGORITHM_STARTS_CONTAINS_HPP

#include <boost/algorithm/cxx20_helper.hpp>
#include <boost/type_traits/is_detected.hpp>


namespace boost { namespace algorithm {

    namespace detail {
        template<
            typename I1,
            typename S1,
            typename I2,
            typename S2,
            typename Pred = std::equal_to<>,
            typename Proj1 = identity,
            typename Proj2 = identity>
        BOOST_CXX14_CONSTEXPR bool contains_subrange_impl(
            I1 first1,
            S1 last1,
            I2 first2,
            S2 last2,
            Pred pred,
            Proj1 proj1,
            Proj2 proj2,
            std::forward_iterator_tag,
            std::forward_iterator_tag)
        {
            while (true) {
                while (true) {
                    if (first1 == last1)
                        return false;
                    if (pred(proj1(*first1), proj2(*first2)))
                        break;
                    ++first1;
                }

                auto it1 = first1;
                auto it2 = first2;
                while (true) {
                    if (++it2 == last2)
                        return true;
                    if (++it1 == last1)
                        return false;
                    if (!pred(proj1(*it1), proj2(*it2))) {
                        ++first1;
                        break;
                    }
                }
            }
        }

        template<
            typename I1,
            typename I2,
            typename Pred = std::equal_to<>,
            typename Proj1 = identity,
            typename Proj2 = identity>
        BOOST_CXX14_CONSTEXPR bool contains_subrange_impl(
            I1 first1,
            I1 last1,
            I2 first2,
            I2 last2,
            Pred pred,
            Proj1 proj1,
            Proj2 proj2,
            std::random_access_iterator_tag,
            std::random_access_iterator_tag)
        {
            const auto size1 = last1 - first1;
            const auto size2 = last2 - first2;

            if (size1 < size2)
                return true;

            const I1 last_start = last1 - (size2 - 1);

            while (true) {
                while (true) {
                    if (first1 == last_start)
                        return false;
                    if (pred(proj1(*first1), proj2(*first2)))
                        break;
                    ++first1;
                }

                auto it1 = first1;
                auto it2 = first2;
                while (true) {
                    ++it1;
                    if (++it2 == last2)
                        return true;
                    if (!pred(proj1(*it1), proj2(*it2))) {
                        ++first1;
                        break;
                    }
                }
            }
        }

        template<typename I, typename Proj>
        using indirectly_invocable_1 =
            decltype(std::declval<Proj>()(*std::declval<I>()));

        template<
            typename Pred,
            typename I1,
            typename Proj1,
            typename I2,
            typename Proj2>
        using indirectly_invocable_2 = decltype(std::declval<Pred>()(
            std::declval<Proj1>()(*std::declval<I1>()),
            std::declval<Proj2>()(*std::declval<I2>())));
    }

#if defined(__cpp_lib_concepts)
    template<
        std::input_iterator I,
        std::sentinel_for<I> S,
        typename T,
        typename Proj = identity>
    requires std::indirect_binary_predicate < std::equal_to<>,
        std::projected<I, Proj>,
    const T * >
#else
    template<
        typename I,
        typename S,
        typename T,
        typename Proj = identity,
        typename Enable = std::enable_if_t<
            !is_detected<detail::indirectly_invocable_1, I, const T &>::value>>
#endif
    BOOST_CXX14_CONSTEXPR bool
    contains(I first, S last, const T & value, Proj proj = {})
    {
        while (first != last) {
            if (proj(*first) == value)
                return true;
            ++first;
        }
        return false;
    }

#if defined(__cpp_lib_concepts)
    template<std::input_range R, typename T, typename Proj = identity>
    requires std::indirect_binary_predicate < std::equal_to<>,
        std::projected<std::iterator_t<R>, Proj>,
    const T * >
#else
    template<
        typename R,
        typename T,
        typename Proj = identity,
        typename Enable = std::enable_if_t<
            is_detected<detail::indirectly_invocable_1, iterator_t<R>, Proj>::
                value>>
#endif
    BOOST_CXX14_CONSTEXPR bool
    contains(R && r, const T & value, Proj proj = {})
    {
        return algorithm::contains(boost::begin(r), boost::end(r), value, proj);
    }

#if defined(__cpp_lib_concepts)
    template<
        std::forward_iterator I1,
        std::sentinel_for<I1> S1,
        std::forward_iterator I2,
        std::sentinel_for<I2> S2,
        typename Pred = std::equal_to<>,
        typename Proj1 = identity,
        typename Proj2 = identity>
    requires std::indirectly_comparable<I1, I2, Pred, Proj1, Proj2>
#else
    template<
        typename I1,
        typename S1,
        typename I2,
        typename S2,
        typename Pred = std::equal_to<>,
        typename Proj1 = identity,
        typename Proj2 = identity,
        typename Enable = std::enable_if_t<!is_detected<
            detail::indirectly_invocable_2,
            I2,
            I1,
            Proj1,
            I2,
            Proj2>::value>>
#endif
    BOOST_CXX14_CONSTEXPR bool contains_subrange(
        I1 first1,
        S1 last1,
        I2 first2,
        S2 last2,
        Pred pred = {},
        Proj1 proj1 = {},
        Proj2 proj2 = {})
    {
        if (first2 == last2)
            return true;

        return detail::contains_subrange_impl(
            first1,
            last1,
            first2,
            last2,
            pred,
            proj1,
            proj2,
            typename std::iterator_traits<I1>::iterator_category(),
            typename std::iterator_traits<I2>::iterator_category());
    }

#if defined(__cpp_lib_concepts)
    template<
        std::forward_range R1,
        std::forward_range R2,
        typename Pred = std::equal_to<>,
        typename Proj1 = identity,
        typename Proj2 = identity>
    requires std::indirectly_comparable<
        std::iterator_t<R1>,
        std::iterator_t<R2>,
        Pred,
        Proj1,
        Proj2>
#else
    template<
        typename R1,
        typename R2,
        typename Pred = std::equal_to<>,
        typename Proj1 = identity,
        typename Proj2 = identity,
        typename Enable = std::enable_if_t<is_detected<
            detail::indirectly_invocable_2,
            Pred,
            iterator_t<R1>,
            Proj1,
            iterator_t<R2>,
            Proj2>::value>>
#endif
    BOOST_CXX14_CONSTEXPR bool contains_subrange(
        R1 && r1,
        R2 && r2,
        Pred pred = {},
        Proj1 proj1 = {},
        Proj2 proj2 = {})
    {
        return algorithm::contains_subrange(
            boost::begin(r1),
            boost::end(r1),
            boost::begin(r2),
            boost::end(r2),
            pred,
            proj1,
            proj2);
    }

}} // namespace boost and algorithm

#endif // BOOST_ALGORITHM_STARTS_CONTAINS_HPP

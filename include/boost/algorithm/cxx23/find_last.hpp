/*
   Copyright (c) T. Zachary Laine 2022.

   Distributed under the Boost Software License, Version 1.0. (See accompanying
   file LICENSE10.txt or copy at http://www.boost.org/LICENSE10.txt)
*/
#ifndef BOOST_ALGORITHM_FIND_LAST_HPP
#define BOOST_ALGORITHM_FIND_LAST_HPP

#include <boost/algorithm/cxx20_helper.hpp>
#include <boost/range/begin.hpp>
#include <boost/range/end.hpp>

#include <utility>


namespace boost { namespace algorithm {

    namespace detail {
        template<typename I, typename S, typename Pred, typename Proj>
        BOOST_CXX14_CONSTEXPR subrange<I> find_last_impl(
            I first, S last, Pred pred, Proj proj, std::forward_iterator_tag)
        {
            bool found = false;
            I latest_found = first;
            for (; first != last; ++first) {
                if (pred(proj(*first))) {
                    latest_found = first;
                    found = true;
                }
            }
            if (found)
                return subrange<I>(latest_found, first);
            else
                return subrange<I>(first, first);
        }

        template<typename I, typename T, typename Pred, typename Proj>
        BOOST_CXX14_CONSTEXPR subrange<I> find_last_impl(
            I first,
            I last,
            Pred pred,
            Proj proj,
            std::bidirectional_iterator_tag)
        {
            for (auto it = last; first != it;) {
                if (pred(proj(*--it)))
                    return subrange<I>(it, last);
            }
            return subrange<I>(last, last);
        }

        template<typename T>
        struct eq_value
        {
            BOOST_CXX14_CONSTEXPR eq_value(const T & value) : value_(value) {}
            template<typename U>
            BOOST_CXX14_CONSTEXPR bool operator()(const U & other) const
            {
                return other == value_;
            }
            const T & value_;
        };

        template<typename Pred>
        struct not_pred
        {
            BOOST_CXX14_CONSTEXPR not_pred(Pred pred) : pred_(pred) {}
            template<typename T>
            BOOST_CXX14_CONSTEXPR bool operator()(const T & value) const
            {
                return !pred_(value);
            }
            Pred pred_;
        };
    }

#if defined(__cpp_lib_concepts)
    template<
        std::forward_iterator ForwardIterator,
        std::sentinel_for<ForwardIterator> Sentinel,
        typename T,
        typename Proj = identity>
    requires std::indirect_binary_predicate < std::ranges::equal_to,
        std::projected<ForwardIterator, Proj>,
    const T * >
#else
    template<
        typename ForwardIterator,
        typename Sentinel,
        typename T,
        typename Proj = identity>
#endif
        BOOST_CXX14_CONSTEXPR subrange<ForwardIterator> find_last(
            ForwardIterator first,
            Sentinel last,
            const T & value,
            Proj proj = {})
    {
        return detail::find_last_impl(
            first,
            last,
            detail::eq_value<T>(value),
            proj,
            typename std::iterator_traits<
                ForwardIterator>::iterator_category());
    }

#if defined(__cpp_lib_concepts)
    template<std::ranges::forward_range R, typename T, typename Proj = identity>
    requires std::indirect_binary_predicate < std::ranges::equal_to,
        std::projected<iterator_t<R>, Proj>,
    const T * >
#else
    template<typename R, typename T, typename Proj = identity>
#endif
        BOOST_CXX14_CONSTEXPR subrange<iterator_t<R>>
        find_last(R && r, const T & value, Proj proj = {})
    {
        return algorithm::find_last(
            boost::begin(r), boost::end(r), value, proj);
    }

#if defined(__cpp_lib_concepts)
    template<
        std::forward_iterator ForwardIterator,
        std::sentinel_for<ForwardIterator> Sentinel,
        typename Proj = identity,
        std::indirect_unary_predicate<std::projected<ForwardIterator, Proj>>
            Pred>
#else
    template<
        typename ForwardIterator,
        typename Sentinel,
        typename Pred,
        typename Proj = identity>
#endif
    BOOST_CXX14_CONSTEXPR subrange<ForwardIterator> find_last_if(
        ForwardIterator first, Sentinel last, Pred pred, Proj proj = {})
    {
        return detail::find_last_impl(
            first,
            last,
            pred,
            proj,
            typename std::iterator_traits<
                ForwardIterator>::iterator_category());
    }

#if defined(__cpp_lib_concepts)
    template<
        std::ranges::forward_range R,
        typename Proj = std::identity,
        std::indirect_unary_predicate<std::projected<iterator_t<R>, Proj>> Pred>
#else
    template<typename R, typename Pred, typename Proj = identity>
#endif
    BOOST_CXX14_CONSTEXPR subrange<iterator_t<R>>
    find_last_if(R && r, Pred pred, Proj proj = {})
    {
        return algorithm::find_last_if(
            boost::begin(r), boost::end(r), pred, proj);
    }

#if defined(__cpp_lib_concepts)
    template<
        std::forward_iterator ForwardIterator,
        std::sentinel_for<ForwardIterator> Sentinel,
        typename Proj = identity,
        std::indirect_unary_predicate<std::projected<ForwardIterator, Proj>>
            Pred>
#else
    template<
        typename ForwardIterator,
        typename Sentinel,
        typename Pred,
        typename Proj = identity>
#endif
    BOOST_CXX14_CONSTEXPR subrange<ForwardIterator> find_last_if_not(
        ForwardIterator first, Sentinel last, Pred pred, Proj proj = {})
    {
        return detail::find_last_impl(
            first,
            last,
            detail::not_pred<Pred>(pred),
            proj,
            typename std::iterator_traits<
                ForwardIterator>::iterator_category());
    }

#if defined(__cpp_lib_concepts)
    template<
        std::ranges::forward_range R,
        typename Proj = identity,
        std::indirect_unary_predicate<std::projected<iterator_t<R>, Proj>> Pred>
#else
    template<typename R, typename Pred, typename Proj = identity>
#endif
    BOOST_CXX14_CONSTEXPR subrange<iterator_t<R>>
    find_last_if_not(R && r, Pred pred, Proj proj = {})
    {
        return algorithm::find_last_if_not(
            boost::begin(r), boost::end(r), pred, proj);
    }

}} // namespace boost and algorithm

#endif // BOOST_ALGORITHM_FIND_LAST_HPP

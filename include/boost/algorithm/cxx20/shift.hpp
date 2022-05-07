/*
   Copyright (c) T. Zachary Laine 2022.

   Distributed under the Boost Software License, Version 1.0. (See accompanying
   file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

    For more information, see http://www.boost.org
*/
#ifndef BOOST_ALGORITHM_SHIFT_HPP
#define BOOST_ALGORITHM_SHIFT_HPP

#include <boost/algorithm/cxx20_helper.hpp>
#include <boost/assert.hpp>

#include <iterator>


namespace boost { namespace algorithm {

    namespace detail {
        template<typename I, typename S>
        struct make_all_subrange
        {
            static BOOST_CXX14_CONSTEXPR subrange<I> call(I first, S last)
            {
                auto it = first;
                while (it != last) {
                    ++it;
                }
                return subrange<I>(first, it);
            }
        };
        template<typename I>
        struct make_all_subrange<I, I>
        {
            static BOOST_CXX14_CONSTEXPR subrange<I> call(I first, I last)
            {
                return subrange<I>(first, last);
            }
        };

        template<typename I, typename S, typename O>
        BOOST_CXX14_CONSTEXPR O ce_move(I first, S last, O out)
        {
            for (; first != last; ++first, ++out) {
                *out = std::move(*first);
            }
            return out;
        }

        template<typename I, typename S, typename O>
        BOOST_CXX14_CONSTEXPR O ce_move_backward(I first, S last, O out)
        {
            while (last != first) {
                *--out = std::move(*--last);
            }
            return out;
        }

        template<typename I>
        BOOST_CXX14_CONSTEXPR subrange<I> shift_left_impl(
            I first, I last, std::ptrdiff_t n, std::random_access_iterator_tag)
        {
            if (last - first <= n)
                return subrange<I>(first, last);
            I it = detail::ce_move(first + n, last, first);
            return subrange<I>(first, it);
        }

        template<typename I, typename S>
        BOOST_CXX14_CONSTEXPR subrange<I> shift_left_impl(
            I first, S last, std::ptrdiff_t n, std::forward_iterator_tag)
        {
            I it = first;
            for (; 0 < n; --n) {
                if (it == last)
                    return make_all_subrange<I, S>::call(first, last);
                ++it;
            }

            if (it == last)
                return make_all_subrange<I, S>::call(first, last);

            it = detail::ce_move(it, last, first);
            return subrange<I>(first, it);
        }

        template<typename I>
        BOOST_CXX14_CONSTEXPR subrange<I> shift_right_impl(
            I first, I last, std::ptrdiff_t n, std::random_access_iterator_tag)
        {
            std::ptrdiff_t size = last - first;
            if (size < n)
                return subrange<I>(first, last);
            I it = detail::ce_move_backward(first, first + size - n, last);
            return subrange<I>(it, last);
        }

        template<typename I, typename S>
        BOOST_CXX14_CONSTEXPR subrange<I> shift_right_impl(
            I first, S last, std::ptrdiff_t n, std::bidirectional_iterator_tag)
        {
            I it = last;
            for (; 0 < n; --n) {
                if (it == first)
                    return make_all_subrange<I, S>::call(first, last);
                --it;
            }

            if (it == first)
                return make_all_subrange<I, S>::call(first, last);

            it = detail::ce_move_backward(first, it, last);
            return subrange<I>(it, last);
        }

        template<typename I, typename S>
        BOOST_CXX14_CONSTEXPR subrange<I> shift_right_impl(
            I first, S last, std::ptrdiff_t n, std::forward_iterator_tag)
        {
            I it = first;
            for (; 0 < n; --n) {
                if (it == last)
                    return make_all_subrange<I, S>::call(first, last);
                ++it;
            }

            I ahead = it;
            I behind = first;
            while (behind != it) {
                if (ahead == last) {
                    std::move(first, behind, it);
                    return subrange<I>(it, last);
                }
                ++ahead;
                ++behind;
            }

            I mid = first;
            while (true) {
                if (ahead == last) {
                    behind = std::move(mid, it, behind);
                    std::move(first, mid, behind);
                    return it;
                }
                std::swap(*mid, *behind);
                ++mid;
                ++ahead;
                ++behind;
                if (mid == it)
                    mid = first;
            }

            return subrange<I>(it, last);
        }

#if defined(__cpp_lib_concepts)
        template<class I>
        concept permutable = std::forward_iterator<I> &&
            std::indirectly_movable_storable<I, I> &&
            std::indirectly_swappable<I, I>;
#endif
    }

#if defined(__cpp_lib_concepts)
    template<detail::permutable I, std::sentinel_for<I> S>
#else
    template<typename I, typename S>
#endif
    BOOST_CXX14_CONSTEXPR subrange<I>
    shift_left(I first, S last, std::ptrdiff_t n)
    {
        BOOST_ASSERT(0 <= n);

        if (!n)
            return detail::make_all_subrange<I, S>::call(first, last);

        return detail::shift_left_impl(
            first,
            last,
            n,
            typename std::iterator_traits<I>::iterator_category());
    }

#if defined(__cpp_lib_concepts)
    template<std::ranges::forward_range R>
    requires detail::permutable<iterator_t<R>>
#else
    template<typename R>
#endif
    BOOST_CXX14_CONSTEXPR subrange<iterator_t<R>>
    shift_left(R && r, std::ptrdiff_t n)
    {
        return algorithm::shift_left(boost::begin(r), boost::end(r), n);
    }

#if defined(__cpp_lib_concepts)
    template<detail::permutable I, std::sentinel_for<I> S>
#else
    template<typename I, typename S>
#endif
    BOOST_CXX14_CONSTEXPR subrange<I>
    shift_right(I first, S last, std::ptrdiff_t n)
    {
        BOOST_ASSERT(0 <= n);

        if (!n)
            return detail::make_all_subrange<I, S>::call(first, last);

        return detail::shift_right_impl(
            first,
            last,
            n,
            typename std::iterator_traits<I>::iterator_category());
    }

#if defined(__cpp_lib_concepts)
    template<std::ranges::forward_range R>
    requires detail::permutable<iterator_t<R>>
#else
    template<typename R>
#endif
    BOOST_CXX14_CONSTEXPR subrange<iterator_t<R>>
    shift_right(R && r, std::ptrdiff_t n)
    {
        return algorithm::shift_right(boost::begin(r), boost::end(r), n);
    }

}} // namespace boost and algorithm

#endif // BOOST_ALGORITHM_SHIFT_HPP

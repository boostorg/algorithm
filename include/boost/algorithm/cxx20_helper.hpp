/*
   Copyright (c) T. Zachary Laine 2022.

   Distributed under the Boost Software License, Version 1.0. (See accompanying
   file LICENSE10.txt or copy at http://www.boost.org/LICENSE10.txt)
*/
#ifndef BOOST_ALGORITHM_CXX20_HELPER_HPP
#define BOOST_ALGORITHM_CXX20_HELPER_HPP

#include <boost/config.hpp>
#include <boost/stl_interfaces/view_interface.hpp>
#include <boost/range/begin.hpp>
#include <boost/range/end.hpp>

#include <type_traits>


namespace boost { namespace algorithm {

    struct identity
    {
        template<typename T>
        BOOST_CXX14_CONSTEXPR T && operator()(T && x) const noexcept
        {
            return (T &&) x;
        }
    };

#if defined(__cpp_lib_concepts)
    template<typename R>
    using iterator_t = std::ranges::iterator_t<R>;
    template<typename R>
    using sentinel_t = std::ranges::sentinel_t<R>;
#else
    template<typename R>
    using iterator_t = decltype(boost::begin(std::declval<R>()));
    template<typename R>
    using sentinel_t = decltype(boost::end(std::declval<R>()));
#endif

    template<typename Iterator, typename Sentinel = Iterator>
    struct subrange
        : stl_interfaces::view_interface<subrange<Iterator, Sentinel>>
    {
        BOOST_CXX14_CONSTEXPR subrange() = default;
        BOOST_CXX14_CONSTEXPR subrange(Iterator first, Sentinel last) :
            first_(first), last_(last)
        {}
        template<typename R>
        BOOST_CXX14_CONSTEXPR explicit subrange(const R & r) :
            first_(boost::begin(r)), last_(boost::end(r))
        {}

        BOOST_CXX14_CONSTEXPR Iterator begin() const { return first_; }
        BOOST_CXX14_CONSTEXPR Sentinel end() const { return last_; }

        [[nodiscard]] BOOST_CXX14_CONSTEXPR subrange
        next(std::ptrdiff_t n = 1) const
        {
            return subrange{std::next(first_), last_};
        }
        [[nodiscard]] BOOST_CXX14_CONSTEXPR subrange
        prev(std::ptrdiff_t n = 1) const
        {
            return subrange{std::prev(first_), last_};
        }

        BOOST_CXX14_CONSTEXPR subrange & advance(std::ptrdiff_t n)
        {
            std::advance(first_, n);
            return *this;
        }

        template<
            typename Iterator2,
            typename Sentinel2,
            typename Enable = std::enable_if_t<
                std::is_convertible<Iterator, Iterator2>::value &&
                std::is_convertible<Sentinel, Sentinel2>::value>>
        BOOST_CXX14_CONSTEXPR operator subrange<Iterator2, Sentinel2>() const
        {
            return {first_, last_};
        }

    private:
        Iterator first_;
        Sentinel last_;
    };

}} // namespace boost and algorithm

#endif // BOOST_ALGORITHM_CXX20_HELPER

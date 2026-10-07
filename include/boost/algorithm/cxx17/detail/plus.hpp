/*
   Copyright (c) fhgffy 2026.

   Distributed under the Boost Software License, Version 1.0. (See accompanying
   file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
*/

#ifndef BOOST_ALGORITHM_DETAIL_PLUS_HPP
#define BOOST_ALGORITHM_DETAIL_PLUS_HPP

#include <boost/config.hpp>
#if !defined(BOOST_NO_CXX11_RVALUE_REFERENCES)
#include <utility>
#endif

namespace boost { namespace algorithm { namespace detail {

// 2026-10-08: Preserve the input reference type and convert only the resulting sum.
template<class T>
struct plus
{
#if !defined(BOOST_NO_CXX11_RVALUE_REFERENCES)
    template<class U>
    T operator()(T& lhs, U&& rhs) const
    {
        return lhs + std::forward<U>(rhs);
    }
#else
    template<class U>
    T operator()(T& lhs, U& rhs) const
    {
        return lhs + rhs;
    }

    template<class U>
    T operator()(T& lhs, const U& rhs) const
    {
        return lhs + rhs;
    }
#endif
};

}}} // namespace boost, algorithm and detail

#endif // BOOST_ALGORITHM_DETAIL_PLUS_HPP

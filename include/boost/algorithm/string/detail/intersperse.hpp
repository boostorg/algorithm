//  Boost string_algo library intersperse.hpp header file  ---------------------------//

//  Copyright Denis Mikhailov 2022.
//
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

//  See http://www.boost.org/ for updates, documentation, and revision history.

#ifndef BOOST_STRING_INTERSPERSE_DETAIL_HPP
#define BOOST_STRING_INTERSPERSE_DETAIL_HPP

#include <boost/algorithm/string/config.hpp>
#include <iterator>

#include <boost/range/size_type.hpp>
#include <boost/range/value_type.hpp>
#include <boost/iterator/iterator_adaptor.hpp> // for iterator_adaptor

namespace boost {
    namespace algorithm {
        namespace detail {

            // just workaround for https://github.com/boostorg/iterator/issues/75
            // FIXME remove this when the issue will be resolved
            template<typename It>
            struct just_iterator_adaptor : iterator_adaptor<just_iterator_adaptor<It>, It> {
                explicit just_iterator_adaptor(It p)
                  : just_iterator_adaptor::iterator_adaptor_(p) {}
            };

            // functor to be passed into ::boost::token_finder
            template<typename SinglePassRange>
            class non_last_condition {
            public:
                typedef BOOST_STRING_TYPENAME range_size<SinglePassRange>::type size_type;
                typedef BOOST_STRING_TYPENAME range_value<SinglePassRange>::type value_type;

                explicit non_last_condition(size_type s, size_type& counter)
                    : m_Size(s), m_Counter(counter) { }
                bool operator() (const value_type&) const {
                    return ++m_Counter < m_Size;
                }
            private:
                const size_type m_Size;
                size_type& m_Counter;
            };


        } // namespace detail
    } // namespace algorithm
} // namespace boost


#endif  // BOOST_STRING_INTERSPERSE_DETAIL_HPP

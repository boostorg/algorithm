//  Boost string_algo library intersperse.hpp header file  ---------------------------//

//  Copyright Denis Mikhailov 2022.
//
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

//  See http://www.boost.org/ for updates, documentation, and revision history.

#ifndef BOOST_STRING_INTERSPERSE_HPP
#define BOOST_STRING_INTERSPERSE_HPP

#include <boost/algorithm/string/config.hpp>

#include <boost/range.hpp>
#include <boost/range/begin.hpp>
#include <boost/range/end.hpp>
#include <boost/range/size.hpp>
#include <boost/range/size_type.hpp>
#include <boost/range/iterator_range_core.hpp>
#include <boost/range/as_literal.hpp>
#include <boost/iterator/function_input_iterator.hpp>
#include <boost/core/addressof.hpp>

#include <boost/algorithm/string/detail/intersperse.hpp>
#include <boost/algorithm/string/find_format.hpp>
#include <boost/algorithm/string/formatter.hpp>
#include <boost/algorithm/string/finder.hpp>

/*! \file
    Defines intersperse algorithms.
    Intersperse algorithms are used to insert value between each contiguous elements 
    from a sequence (string). Also, variants with (\c _fill) postfix can insert a whole 
    range instead of value.

    Parametric (\c _generate) variants use a generator (functor) to obtain characters
    to be inserted..
    Functions take a nullary function object as a parameter, which is used to extract 
    a new character and then insert.
*/

namespace boost {
    namespace algorithm {

    //  intersperse fill  -----------------------------------------------//


        //! Intersperse fill
        /*!
            Insert string between each contiguous elements from the input.
            The input sequence is modified in-place.

            \param Input An input sequence
            \param Fill A string to be inserted
        */
        template<typename SequenceT, typename RangeT>
        inline void intersperse_fill(SequenceT& Input, const RangeT& Fill)
        {
            typename ::boost::range_size<SequenceT>::type counter = 0;
            ::boost::find_format_all(       
                Input,
                ::boost::token_finder(
                    detail::non_last_condition<SequenceT>(::boost::size(Input), counter)),
                ::boost::join_formatter(::boost::as_literal(Fill)));
        }

        //! Intersperse fill
        /*!
            Insert string between each contiguous elements from the input.
            The result is an interspersed copy of the input

            \param Input An input sequence
            \param Fill A string to be inserted
            \return An interspersed copy of the input
            
            \note This function provides the strong exception-safety guarantee
        */
        template<typename SequenceT, typename RangeT>
        inline SequenceT intersperse_fill_copy(
            const SequenceT& Input,
            const RangeT& Fill)
        {
            typename ::boost::range_size<SequenceT>::type counter = 0;
            return 
                ::boost::find_format_all_copy(      
                    Input,
                    ::boost::token_finder(
                        detail::non_last_condition<SequenceT>(::boost::size(Input), counter)),
                    ::boost::join_formatter(::boost::as_literal(Fill)));
        }

    //  intersperse  -----------------------------------------------//

        //! Intersperse - parametric
        /*!
            Insert generated char between each contiguous elements from the input.
            The input sequence is modified in-place.

            \param Input An input sequence
            \param G A generator(nullary function object) used to create insertable char
        */
        template<typename SequenceT, typename GeneratorT>
        inline void intersperse_generate(SequenceT& Input, GeneratorT G)
        {
            typedef ::boost::function_input_iterator<GeneratorT, int> It;
            intersperse_fill(Input, boost::make_iterator_range(
                detail::just_iterator_adaptor<It>(::boost::make_function_input_iterator(G, 0)),
                detail::just_iterator_adaptor<It>(::boost::make_function_input_iterator(G, 1))));
        }

        //! Intersperse - parametric
        /*!
            Insert generated char between each contiguous elements from the input.
            The result is an interspersed copy of the input.
            \param Input An input sequence
            \param G A generator(nullary function object) used to create insertable char
            \return An interspersed copy of the input
        */
        template<typename SequenceT, typename GeneratorT>
        inline SequenceT intersperse_generate_copy(
            const SequenceT& Input,
            GeneratorT G)
        {
            typedef ::boost::function_input_iterator<GeneratorT, int> It;
            return
                intersperse_fill_copy(Input, ::boost::make_iterator_range(
                    detail::just_iterator_adaptor<It>(::boost::make_function_input_iterator(G, 0)),
                    detail::just_iterator_adaptor<It>(::boost::make_function_input_iterator(G, 1))));
        }

        //! Intersperse
        /*!
            Insert char between each contiguous elements from the input.
            The input sequence is modified in-place.

            \param Input An input sequence
            \param Value A char to be inserted
        */
        template<typename SequenceT, typename T>
        inline void intersperse(SequenceT& Input, const T& Value)
        {
            intersperse_fill(Input, ::boost::make_iterator_range(
                ::boost::addressof(Value),
                ::boost::addressof(Value)+1));
        }

        //! Intersperse
        /*!
            Insert char between each contiguous elements from the input.
            The result is an interspersed copy of the input

            \param Input An input sequence
            \param Value A char to be inserted
            \return An interspersed copy of the input

            \note This function provides the strong exception-safety guarantee
        */
        template<typename SequenceT, typename T>
        inline SequenceT intersperse_copy(
            const SequenceT& Input,
            const T& Value)
        {
            return
                intersperse_fill_copy(Input, ::boost::make_iterator_range(
                    ::boost::addressof(Value),
                    ::boost::addressof(Value)+1));
        }

    } // namespace algorithm 

    // pull names to the boost namespace
    using algorithm::intersperse_fill;
    using algorithm::intersperse_fill_copy;
    using algorithm::intersperse_generate;
    using algorithm::intersperse_generate_copy;
    using algorithm::intersperse;
    using algorithm::intersperse_copy;

} // namespace boost

#endif  // BOOST_STRING_INTERSPERSE_HPP

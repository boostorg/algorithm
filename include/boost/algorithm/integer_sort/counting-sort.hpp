//  (C) Copyright Jeremy W. Murphy 2013.
//  Use, modification and distribution are subject to the 
//  Boost Software License, Version 1.0. (See accompanying file 
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

/** \file counting-sort.hpp
 *  \brief Stable counting sort.
 */

#ifndef COUNTING_SORT
#define COUNTING_SORT

#include <iterator>
#include <numeric>
#include <cassert>
#include <limits>
#include <vector>

#include <boost/concept_check.hpp>
#include <boost/concept/requires.hpp>
#include <boost/integer.hpp>
#include <boost/utility/result_of.hpp>
#include <boost/algorithm/minmax_element.hpp>
#include <boost/scoped_array.hpp>


namespace boost {
namespace algorithm {
    namespace detail {
        
        template <typename Value, typename Shift, typename Bitmask>
        inline Value count_index(Value const a, Shift const b, Value const c, Bitmask const d)
        {
            return ((a >> b) - c) & d;
        }
    }
    
    
    // For types that are implicitly convertible to an unsigned integral type.
    template <typename T>
    struct identity
    {
        typedef T result_type;
        
        identity() {}
        
        template <typename U>
        T operator()(U const &x) const
        {
            return x;
        }
    };
    
    
    /**
    * Requires that client allocates space for result beforehand.
    * 
    * @brief Generalized stable counting-sort.
    * 
    * \c Input Bidirectional input iterator.
    * \c Output Random access output iterator.
    * 
    * \param first Input iterator that points to the first element of the unsorted data.
    * \param last Input iterator that points past the last element of the unsorted data.
    * \param result Output iterator that points to the first element where the sorted data will go.
    * \param conv Function object that converts the input type to an unsigned integral type.
    * \param min The smallest value present in the input >> r * d.
    * \param max The largest value present in the input >> r * d.
    * \param r The radix or width of digit to consider.
    * \param d Which digit to consider.
    */
    template <typename Input, typename Output, typename Conversion>
        BOOST_CONCEPT_REQUIRES(
            ((BidirectionalIterator<Input>))
            ((Mutable_RandomAccessIterator<Output>))
            ((UnsignedInteger<typename tr1_result_of<Conversion(typename std::iterator_traits<Input>::value_type)>::type>))
            , (Output))
    stable_counting_sort(Input first, Input last, Output result, Conversion conv,
        typename tr1_result_of<Conversion(typename std::iterator_traits<Input>::value_type)>::type const min,
        typename tr1_result_of<Conversion(typename std::iterator_traits<Input>::value_type)>::type const max,
        unsigned const radix, unsigned char const digit)
    {
        typedef std::reverse_iterator<Input> ReverseIterator;

        if(first != last)
        {
            Input next(first);
            ++next;
            if(next == last)
                *result++ = *first;
            else
            {
                assert(radix != 0);
                // TODO: Maybe this next assertion should be an exception?
                assert(max - min != std::numeric_limits<uintmax_t>::max()); // Because k - min + 1 == 0.
                unsigned const shift = radix * digit;
                uintmax_t const bitmask = (1ul << radix) - 1;
                std::vector<uintmax_t> C(static_cast<uintmax_t>(max - min) + 1);
                ReverseIterator rfirst(last);
                ReverseIterator const rlast(first);

                // TODO: Could this be done faster by left-shifting _min and _bitmask once instead of right-shifting the value n times?
                for(; first != last; first++)
                    C[detail::count_index(conv(*first), shift, min, bitmask)]++;

                std::partial_sum(C.begin(), C.end(), C.begin());

                for(; rfirst != rlast; rfirst++)
                    *(result + --C[detail::count_index(conv(*rfirst), shift, min, bitmask)]) = *rfirst;
            }
        }
        return result;
    }

    
    template <typename Input, typename Output, typename Conversion>
        BOOST_CONCEPT_REQUIRES(
            ((BidirectionalIterator<Input>))
            ((Mutable_RandomAccessIterator<Output>))
            ((UnsignedInteger<typename tr1_result_of<Conversion(typename std::iterator_traits<Input>::value_type)>::type>))
        , (Output))
    stable_counting_sort(Input first, Input last, Output result, Conversion conv,
        typename tr1_result_of<Conversion(typename std::iterator_traits<Input>::value_type)>::type const min,
        typename tr1_result_of<Conversion(typename std::iterator_traits<Input>::value_type)>::type const max)
    {
        unsigned const radix(sizeof(typename tr1_result_of<Conversion(typename std::iterator_traits<Input>::value_type)>::type) * 8);
        return stable_counting_sort(first, last, result, conv, min, max, radix, 0);
    }
    
    
    template <typename Input, typename Output, typename Conversion>
        BOOST_CONCEPT_REQUIRES(
            ((BidirectionalIterator<Input>))
            ((Mutable_RandomAccessIterator<Output>))
            ((UnsignedInteger<typename tr1_result_of<Conversion(typename std::iterator_traits<Input>::value_type)>::type>))
            , (Output))
    stable_counting_sort(Input first, Input last, Output result, Conversion conv)
    {
        if(first != last)
        {
            std::pair<Input, Input> const bound(boost::minmax_element(first, last));
            return stable_counting_sort(first, last, result, conv, *bound.first, *bound.second);
        }
        else
            return result;
    }
    
    
    template <typename Input, typename Output>
        BOOST_CONCEPT_REQUIRES(
            ((BidirectionalIterator<Input>))
            ((Mutable_RandomAccessIterator<Output>))
            , (Output))
    stable_counting_sort(Input first, Input last, Output result)
    {
        return stable_counting_sort(first, last, result, identity<typename std::iterator_traits<Input>::value_type>());
    }
}
}
#endif

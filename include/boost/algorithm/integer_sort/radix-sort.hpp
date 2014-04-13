//  (C) Copyright Jeremy W. Murphy 2013.
//  Use, modification and distribution are subject to the 
//  Boost Software License, Version 1.0. (See accompanying file 
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

/// \file radix-sort.hpp
/// \brief Stable LSD radix sort.

#ifndef RADIX_SORT
#define RADIX_SORT

#include <boost/algorithm/integer_sort/counting-sort.hpp>
#include <boost/algorithm/minmax_element.hpp>

#include <algorithm>
#include <cmath>
#include <iterator>
#include <utility>
#include <vector>

/* TODO: Calculate and use effective b?
 * It's assumed that value_type is no larger than necessary.  That is, k >= 2^(b/2).
 * However, if k << 2^b, then there are a lot of unused significant bits that do not need sorting.
 * Using effective b could reduce the number of passes required to sort.
 */

namespace boost {
namespace algorithm {
    /**
     * \fn stable_radix_sort
     * \brief Stable LSD radix sort.
     * 
     * \c Input Bidirectional input iterator.
     * \c Output Random access output iterator.
     * 
     * \param first Input iterator that points to the first element of the unsorted data.
     * \param last Input iterator that points past the last element of the unsorted data.
     * \param result Output iterator that points to the first element where the sorted data will go.
     * \param conv Function object that converts the input type to an unsigned integral type.
     * \param min The smallest value present in the input.
     * \param max The largest value present in the input.
     */
    template <typename Input, typename Output, typename Conversion>
    BOOST_CONCEPT_REQUIRES(((BidirectionalIterator<Input>))
        ((Mutable_RandomAccessIterator<Output>))
        ((UnsignedInteger<typename tr1_result_of<Conversion(typename std::iterator_traits<Input>::value_type)>::type>)), 
                           (Output))
    stable_radix_sort(Input first, Input last, Output result, Conversion conv,
        typename tr1_result_of<Conversion(typename std::iterator_traits<Input>::value_type)>::type const min,
        typename tr1_result_of<Conversion(typename std::iterator_traits<Input>::value_type)>::type const max,
        unsigned const radix = 8u)
    {
        typedef typename std::iterator_traits<Input>::value_type value_type;
        typedef typename tr1_result_of<Conversion(typename std::iterator_traits<Input>::value_type)>::type uint_type;
        
        if(first != last)
        {
            assert(max >= min);
            
            Input next(first);
            ++next;
            if(next == last)
                *result++ = *first;
            else
            {
                char unsigned const bits(sizeof(uint_type) * 8), 
                                digits(ceil(static_cast<double>(bits) / static_cast<double>(radix)));
            
                assert(radix * digits >= bits);
                
                if(digits == 1)
                    stable_counting_sort(first, last, result, conv, min, max);
                else
                {
                    std::vector<value_type> tmp1(first, last);
                    uint_type const dk = (uint_type(1) << radix) - 1; // TODO: This can be improved.
                    // NOTE: Is there an easy way to utilize minimum here?
                    
                    if(digits == 2)
                    {
                        stable_counting_sort(first, last, tmp1.begin(), conv, 0, dk, radix, 0);
                        stable_counting_sort(tmp1.begin(), tmp1.end(), result, conv, 0, dk, radix, 1);
                    }
                    else
                    {
                        std::vector<value_type> tmp2(tmp1.size());
                        for(unsigned i = 0; i < digits; i++)
                        {
                            stable_counting_sort(tmp1.begin(), tmp1.end(), tmp2.begin(), conv, 0, dk, radix, i);
                            std::swap(tmp1, tmp2);
                        }
                        
                        std::copy(tmp1.begin(), tmp1.end(), result);
                    }
                }
            }
        }
        return result;
    }
    
    
    template <typename Input, typename Output, typename Conversion>
        BOOST_CONCEPT_REQUIRES(((BidirectionalIterator<Input>))
        ((Mutable_RandomAccessIterator<Output>))
        ((UnsignedInteger<typename tr1_result_of<Conversion(typename std::iterator_traits<Input>::value_type)>::type>)), 
                            (Output))
    stable_radix_sort(Input first, Input last, Output result, Conversion conv)
    {
        if(first != last)
        {
            std::pair<Input, Input> const bound(boost::minmax_element(first, last));
            return stable_radix_sort(first, last, result, conv, conv(*bound.first), conv(*bound.second));
        }
        else
            return result;
    }

    
    template <typename Input, typename Output>
        BOOST_CONCEPT_REQUIRES(((BidirectionalIterator<Input>))
        ((Mutable_RandomAccessIterator<Output>))
        ((UnsignedInteger<typename std::iterator_traits<Input>::value_type>)), 
                           (Output))
    stable_radix_sort(Input first, Input last, Output result)
    {
        return stable_radix_sort(first, last, result, identity<typename std::iterator_traits<Input>::value_type>());
    }
}
}
#endif

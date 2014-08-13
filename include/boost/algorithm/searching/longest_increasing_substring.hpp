/* 
   Copyright (c) Marek Kurdej 2014.

   Distributed under the Boost Software License, Version 1.0. (See accompanying
   file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

    For more information, see http://www.boost.org
*/

#ifndef BOOST_ALGORITHM_LONGEST_INCREASING_SUBSTRING_HPP
#define BOOST_ALGORITHM_LONGEST_INCREASING_SUBSTRING_HPP

#include <iterator>     // for std::iterator_traits

#include <boost/assert.hpp>

#include <boost/range/begin.hpp>
#include <boost/range/end.hpp>

// #include <boost/type_traits/is_same.hpp>

// #include <boost/algorithm/searching/detail/debugging.hpp>
// #define BOOST_ALGORITHM_LONGEST_INCREASING_SUBSTRING_DEBUG_HPP

namespace boost { namespace algorithm {

/*
    A templated version of the longest (increasing) substring algorithm.
    
    Requirements:
        * Forward (input) iterators
*/

    template <typename Iter, typename Compare = std::less<typename std::iterator_traits<Iter>::value_type> >
    class longest_increasing_substring
    {
    public:
        typedef std::pair<Iter, Iter> result_type;

        /// \param cmp          A predicate used for the search comparisons.
        longest_increasing_substring (Compare cmp = Compare())
            : compare(cmp) {
        }
            
        ~longest_increasing_substring () {}
        
        /// \brief Searches the longest (increasing) substring in the data
        ///
        /// The result is a pair of iterators delimiting the longest increasing substring (continuous elements).
        /// By giving a different comparison predicate to the constructor, one can find as well, e.g., the longest decreasing
        /// substring (with std::greater<T>() predicate) or the non-decreasing one (with std::not1(std::greater<T>())).
        /// 
        /// \param first        The start of the data to search (Forward Input Iterator)
        /// \param last         One past the end of the data to search
        ///
        result_type operator () ( Iter first, Iter last ) const
        {
            return this->do_search ( first, last );
        }
        
        template <typename Range>
        Range operator () ( Range &r ) const
        {
            // BOOST_STATIC_ASSERT (( boost::is_same<
            //     typename std::iterator_traits<boost::range_iterator<Range> >::value_type, 
            //     typename std::iterator_traits<Iter>::value_type>::value ));
            
            result_type result = (*this) ( boost::begin(r), boost::end(r) );
            return Range ( result.first, result.second );
        }

        std::size_t compute_length(Iter first, Iter last ) const
        {
            result_type result = (*this) (first, last);
            return std::distance(result.first, result.second);
        }

    private:
/// \cond DOXYGEN_HIDE
        Compare compare;
        
        result_type do_search ( Iter first, Iter last ) const {
            // Empty sequence has empty increasing substring
            result_type result(first, first);
            if (first == last) {
                return result;
            }

            Iter prev = first;
            Iter i = first;
            std::advance(i, 1);
            result = std::make_pair(first, i);
            std::size_t n = 1;
            assert(std::distance(result.first, result.second) == 1);

            result_type newResult = result;
            std::size_t newN = n;
            assert(std::distance(newResult.first, newResult.second) == 1);
            for (; i != last; ++i) {
                if (compare(*prev, *i)) {
                    // Current element is in the increasing substring
                    ++newN;
                } else {
                    // Current is not increasing, restart a new substring
                    if (newN > n) {
                        n = newN;
                        result = std::make_pair(newResult.first, i);
                        assert(std::distance(result.first, result.second) == n);
                    }
                    Iter j = i;
                    std::advance(j, 1);
                    newResult = std::make_pair(i, j);
                    newN = 1;
                    assert(std::distance(newResult.first, newResult.second) == 1);
                }
                prev = i;
            }
            if (newN > n) {
                // Last element was in the longest increasing substring
                n = newN;
                result = std::make_pair(newResult.first, i);
            }
            assert(std::distance(result.first, result.second) == n);
            return result;
        }
// \endcond
    };

/*  Two ranges as inputs gives us four possibilities; with 1,2 parameters
    Use a bit of TMP to disambiguate the 3-argument templates */

/// \fn longest_increasing_substring_search ( corpusIter corpus_first, corpusIter corpus_last, 
///       patIter pat_first, patIter pat_last )
/// \brief Searches the corpus for the pattern.
/// 
/// \param corpus_first The start of the data to search (Random Access Iterator)
/// \param corpus_last  One past the end of the data to search
/// \param pat_first    The start of the pattern to search for (Random Access Iterator)
/// \param pat_last     One past the end of the data to search for
///
    template <typename Iter>
    std::size_t longest_increasing_substring_length( Iter first, Iter last )
    {
        longest_increasing_substring<Iter> lis;
        return lis.compute_length(first, last);
    }
    
    template <typename Iter, typename Compare>
    std::size_t longest_increasing_substring_length( Iter first, Iter last, Compare cmp )
    {
        longest_increasing_substring<Iter, Compare> lis(cmp);
        return lis.compute_length( first, last );
    }

    template <typename Range>
    std::size_t longest_increasing_substring_length( const Range &sequence )
    {
        return longest_increasing_substring_length(boost::begin(sequence), boost::end(sequence));
    }

    template <typename Range, typename Compare>
    std::size_t longest_increasing_substring_length( const Range &sequence, Compare cmp )
    {
        return longest_increasing_substring_length(boost::begin(sequence), boost::end(sequence), cmp);
    }

    template <typename Iter>
    std::pair<Iter, Iter> longest_increasing_substring_search ( Iter first, Iter last )
    {
        longest_increasing_substring<Iter> lis;
        return lis ( first, last );
    }

    template <typename Iter, typename Compare>
    std::pair<Iter, Iter> longest_increasing_substring_search ( Iter first, Iter last, Compare cmp )
    {
        longest_increasing_substring<Iter, Compare> lis(cmp);
        return lis ( first, last );
    }

    template <typename Range>
    Range longest_increasing_substring_search ( const Range &sequence )
    {
        typedef typename boost::range_iterator<const Range>::type iterator;
        typedef longest_increasing_substring<iterator> lis_type;
        lis_type lis;
        typename lis_type::result_type result = lis ( boost::begin(sequence), boost::end(sequence) );
        return Range ( result.first, result.second );
    }

    template <typename Range, typename Compare>
    Range longest_increasing_substring_search ( const Range &sequence, Compare cmp )
    {
        typedef typename boost::range_iterator<const Range>::type iterator;
        typedef longest_increasing_substring<iterator, Compare> lis_type;
        lis_type lis(cmp);
        typename lis_type::result_type result = lis ( boost::begin(sequence), boost::end(sequence) );
        return Range ( result.first, result.second );
    }

    //  Creator functions -- take a pattern range, return an object
    template <typename Range>
    boost::algorithm::longest_increasing_substring<typename boost::range_iterator<const Range>::type>
    make_longest_increasing_substring ( const Range &r ) {
        return boost::algorithm::longest_increasing_substring
            <typename boost::range_iterator<const Range>::type> ();
        }

    template <typename Range, typename Compare>
    boost::algorithm::longest_increasing_substring<typename boost::range_iterator<const Range>::type, Compare>
    make_longest_increasing_substring ( const Range &r, Compare cmp )
    {
        return boost::algorithm::longest_increasing_substring
            <typename boost::range_iterator<const Range>::type, Compare> (cmp);
        }

    template <typename Range>
    boost::algorithm::longest_increasing_substring<typename boost::range_iterator<Range>::type>
    make_longest_increasing_substring ( Range &r )
    {
        return boost::algorithm::longest_increasing_substring
            <typename boost::range_iterator<Range>::type> ();
        }

    template <typename Range, typename Compare>
    boost::algorithm::longest_increasing_substring<typename boost::range_iterator<Range>::type, Compare>
    make_longest_increasing_substring ( Range &r, Compare cmp )
    {
        return boost::algorithm::longest_increasing_substring
            <typename boost::range_iterator<Range>::type, Compare> (cmp);
        }

}}

#endif  //  BOOST_ALGORITHM_LONGEST_INCREASING_SUBSTRING_HPP

/* 
   Copyright (c) Marek Kurdej 2014.

   Distributed under the Boost Software License, Version 1.0. (See accompanying
   file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

    For more information, see http://www.boost.org
*/

#ifndef BOOST_ALGORITHM_LONGEST_INCREASING_SUBSEQUENCE_HPP
#define BOOST_ALGORITHM_LONGEST_INCREASING_SUBSEQUENCE_HPP

#include <iomanip>
#include <iterator>     // for std::iterator_traits

#include <boost/assert.hpp>
#include <boost/static_assert.hpp>

#include <boost/range/begin.hpp>
#include <boost/range/end.hpp>

#include <boost/utility/enable_if.hpp>
#include <boost/type_traits/is_same.hpp>

#include <boost/algorithm/searching/detail/debugging.hpp>

// #define BOOST_ALGORITHM_LONGEST_INCREASING_SUBSEQUENCE_DEBUG_HPP

#include <vector>

namespace boost { namespace algorithm {

/*
    A templated version of the longest (increasing) subsequence algorithm.
    
    Requirements:
        * Random access iterators
        * The two iterator types (patIter and corpusIter) must 
            "point to" the same underlying type.
        * Additional requirements may be imposed buy the skip table, such as:
        ** Numeric type (array-based skip table)
        ** Hashable type (map-based skip table)

https://en.wikipedia.org/wiki/Longest_increasing_subsequence

*/

    template <typename Iter, typename Compare = std::less<typename std::iterator_traits<Iter>::value_type> >
    class longest_increasing_subsequence {
        typedef typename std::iterator_traits<Iter>::difference_type difference_type;
        typedef typename std::iterator_traits<Iter>::value_type value_type;
    public:
        longest_increasing_subsequence (Compare cmp = Compare())
            : compare(cmp) {
        }
            
        ~longest_increasing_subsequence () {}
        
        /// \fn operator ( corpusIter corpus_first, corpusIter corpus_last, Pred p )
        /// \brief Searches the corpus for the pattern that was passed into the constructor
        /// 
        /// \param first        The start of the data to search (Random Access Iterator)
        /// \param last         One past the end of the data to search
        /// \param cmp          A predicate used for the search comparisons.
        ///
        std::vector<Iter> operator () ( Iter first, Iter last ) const {
            return this->do_search ( first, last );
            }
            
        template <typename Range>
        typename boost::range_iterator<Range>::type operator () ( Range &r ) const {
            return (*this) (boost::begin(r), boost::end(r));
            }

    private:
/// \cond DOXYGEN_HIDE
        Compare compare;
        
        /// \fn do_search ( corpusIter corpus_first, corpusIter corpus_last )
        /// \brief Searches the corpus for the pattern that was passed into the constructor
        /// 
        /// \param corpus_first The start of the data to search (Random Access Iterator)
        /// \param corpus_last  One past the end of the data to search
        ///
        std::vector<Iter> do_search ( Iter first, Iter last ) const {
            // typedef typename node::iterator iterator;
            // typedef typename node* iterator;
            // typedef typename node::const_iterator const_iterator;
            // typedef typename node const* const_iterator;
            
            typedef std::size_t size_type;
            typedef std::vector<size_type> Container;
            typedef typename std::iterator_traits<typename Container::iterator>::difference_type difference_type;
            difference_type const n = std::distance(first, last);
            // if (n == 0)
            
            /// P[k] - stores the index of the predecessor of X[k] in the longest increasing subsequence ending at X[k]
            Container P(n);
            /// M[j] - stores the index k of the smallest value X[k] such that there is an increasing subsequence of length j ending at X[k] on the range k <= i
            /// (note we have j <= k <= i here, because j represents the length of the increasing subsequence, and k represents the index of its termination.
            /// Obviously, we can never have an increasing subsequence of length 13 ending at index 11. k <= i by definition).
            // -> j-elements: X[.] X[.] ... X[k] is an increasing subsequence
            Container M(n + 1);
            M[0] = 0;
            
            /// Length of the longest (increasing) subsequence found so far
            size_type L = 0;
            
            for (difference_type i = 0; i < n; ++i) {
                // Binary search for the largest positive j <= L, such that X[M[j]] < X[i]
                size_type lo = 1;
                size_type hi = L;
                while (lo <= hi) {
                    size_type mid = (lo + hi) / 2;
                    assert(mid <= i);
                    if (compare(*(first + M[mid]), *(first + i))) { // X[M[mid]] < X[i]
                            lo = mid + 1;
                    } else {
                            hi = mid - 1;
                    }
                }
                
                // After searching, lo is 1 greater than the length of the longest prefix of X[i]
                size_type newL = lo;
                
                // The predecessor of X[i] is the last index of the subsequence of length newL-1
                assert(newL > 0);
                P[i] = M[newL - 1];
                
                assert(newL <= n);
                assert(M[newL] < n);
                if (newL > L) {
                    // If we found a subsequence longer than any we've found yet, update M and L
                    M[newL] = i;
                    L = newL;
                } else if (compare(*(first + i), *(first + M[newL]))) { // X[i] < X[M[newL]]
                    // If we found a smaller last value for the
                    // subsequence of length newL, only update M
                    M[newL] = i;
                } // if
            } // for
            
            // Reconstruct the longest increasing subsequence
            std::vector<Iter> S(L);
            size_type k = M[L];
            for (size_type i = L - 1; i > 0; --i) {
                S[i] = first + k; // X[k];
                k = P[k];
            }
            S[0] = first + k; // X[k];
            return S;
        }
// \endcond
    };

/*  Two ranges as inputs gives us four possibilities; with 1,2 parameters
    Use a bit of TMP to disambiguate the 3-argument templates */

/// \fn longest_increasing_subsequence_search ( corpusIter corpus_first, corpusIter corpus_last, 
///       patIter pat_first, patIter pat_last )
/// \brief Searches the corpus for the pattern.
/// 
/// \param corpus_first The start of the data to search (Random Access Iterator)
/// \param corpus_last  One past the end of the data to search
/// \param pat_first    The start of the pattern to search for (Random Access Iterator)
/// \param pat_last     One past the end of the data to search for
///
    template <typename Iter>
    std::size_t longest_increasing_subsequence_length(
                  Iter first, Iter last )
    {
        longest_increasing_subsequence<Iter> lis;
        return lis.compute_length(first, last);
    }
    
    template <typename Iter, typename Compare>
    std::size_t longest_increasing_subsequence_length(
                  Iter first, Iter last, Compare cmp )
    {
        longest_increasing_subsequence<Iter> lis(cmp);
        return lis.compute_length( first, last );
    }

    template <typename Iter>
    Iter longest_increasing_subsequence_search ( 
                  Iter first, Iter last )
    {
        longest_increasing_subsequence<Iter> lis;
        return lis ( first, last );
    }

    template <typename Range, typename OutputIter>
    OutputIter longest_increasing_subsequence_search ( 
        const Range &sequence )
    {
        typedef typename boost::range_iterator<const Range>::type pattern_iterator;
        longest_increasing_subsequence<pattern_iterator> lis;
        return lis ( boost::begin(sequence), boost::end(sequence) );
    }

    //  Creator functions -- take a pattern range, return an object
    template <typename Range>
    boost::algorithm::longest_increasing_subsequence<typename boost::range_iterator<const Range>::type>
    make_longest_increasing_subsequence ( const Range &r ) {
        return boost::algorithm::longest_increasing_subsequence
            <typename boost::range_iterator<const Range>::type> (boost::begin(r), boost::end(r));
        }

    template <typename Range>
    boost::algorithm::longest_increasing_subsequence<typename boost::range_iterator<Range>::type>
    make_longest_increasing_subsequence ( Range &r ) {
        return boost::algorithm::longest_increasing_subsequence
            <typename boost::range_iterator<Range>::type> (boost::begin(r), boost::end(r));
        }

}}

#endif  //  BOOST_ALGORITHM_LONGEST_INCREASING_SUBSEQUENCE_HPP

/*
   Copyright (c) Marek Kurdej 2014.

   Distributed under the Boost Software License, Version 1.0. (See accompanying
   file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

    For more information, see http://www.boost.org
*/

#ifndef BOOST_ALGORITHM_LONGEST_INCREASING_SUBSEQUENCE_HPP
#define BOOST_ALGORITHM_LONGEST_INCREASING_SUBSEQUENCE_HPP

#include <iterator>  // for std::iterator_traits

#include <boost/assert.hpp>
#include <boost/static_assert.hpp>

#include <boost/range/begin.hpp>
#include <boost/range/end.hpp>

#include <boost/type_traits/is_same.hpp>

// #define BOOST_ALGORITHM_LONGEST_INCREASING_SUBSEQUENCE_DEBUG
#ifdef BOOST_ALGORITHM_LONGEST_INCREASING_SUBSEQUENCE_DEBUG
#include <boost/algorithm/searching/detail/debugging.hpp>
#endif

#include <vector>

namespace boost { namespace algorithm {

/*
    A templated version of the longest (increasing) subsequence algorithm.

    Requirements:
        * Random access iterators

https://en.wikipedia.org/wiki/Longest_increasing_subsequence

*/

template <typename Iter, typename Compare = std::less<
                             typename std::iterator_traits<Iter>::value_type> >
class longest_increasing_subsequence {
    typedef
        typename std::iterator_traits<Iter>::difference_type difference_type;
    typedef typename std::iterator_traits<Iter>::value_type value_type;

public:
    typedef std::vector<Iter> iterator_vector;
    typedef std::vector<value_type> value_vector;

    longest_increasing_subsequence ( Compare cmp = Compare () )
        : compare ( cmp )
    {
    }

    ~longest_increasing_subsequence () {}

    /// \brief Searches the longest (increasing) subsequence in the sequence
    ///
    /// The result is a vector of iterators defining the longest increasing
    /// subsequence (possibly non-continuous elements). By giving a different
    /// comparison predicate to the constructor, one can find as well, e.g., the
    /// longest decreasing subsequence (with std::greater<T>() predicate) or the
    /// non-decreasing one (with std::not1(std::greater<T>())).
    /// The time complexity of the algorithm is log-linear O(n logn) where n is the length
    /// of the sequence (`std::distance(first, last)`).
    /// The space complexity is linear O(n).
    ///
    /// \param first        The start of the sequence to search (Random Access
    /// Iterator)
    /// \param last         One past the end of the sequence to search
    ///
    iterator_vector operator()( Iter first, Iter last ) const
    {
        return this->do_search ( first, last );
    }

    template <typename OutputIter>
    OutputIter
    operator()( Iter first, Iter last, OutputIter d_first ) const
    {
        BOOST_STATIC_ASSERT (( boost::is_same<
            typename std::iterator_traits<Iter>::value_type, 
            typename std::iterator_traits<OutputIter>::value_type>::value ));
        return this->do_search ( first, last, d_first );
    }

    template <typename Range>
    iterator_vector operator()( const Range &r ) const
    {
        return ( *this )( boost::begin ( r ), boost::end ( r ) );
    }

    template <typename Range, typename OutputIter>
    OutputIter
    operator()( const Range &r, OutputIter d_first ) const
    {
        BOOST_STATIC_ASSERT (( boost::is_same<
            typename std::iterator_traits<Iter>::value_type, 
            typename std::iterator_traits<OutputIter>::value_type>::value ));
        return this->do_search ( boost::begin ( r ), boost::end ( r ), d_first );
    }

    std::size_t
    compute_length ( Iter first, Iter last ) const
    {
        // TODO: separate search from output creation, and use the first part here
        iterator_vector result = ( *this )( first, last );
        return result.size ();
    }

    template <typename Range>
    std::size_t
    compute_length ( const Range &r ) const
    {
        return ( *this )( boost::begin ( r ), boost::end ( r ) );
    }

private:
    /// \cond DOXYGEN_HIDE
    Compare compare;

    iterator_vector
    do_search ( Iter first, Iter last ) const
    {
        typedef std::size_t size_type;
        typedef std::vector<size_type> Container;
        typedef typename std::iterator_traits<
            typename Container::iterator>::difference_type difference_type;
        difference_type const n = std::distance ( first, last );
        // if (n == 0)

        /// P[k] - stores the index of the predecessor of X[k] in the longest
        /// increasing subsequence ending at X[k]
        Container P ( n );
        /// M[j] - stores the index k of the smallest value X[k] such that there
        /// is an increasing subsequence of length j ending at X[k] on the range
        /// k <= i
        /// (note we have j <= k <= i here, because j represents the length of
        /// the increasing subsequence, and k represents the index of its
        /// termination.
        /// Obviously, we can never have an increasing subsequence of length 13
        /// ending at index 11. k <= i by definition).
        // -> j-elements: X[.] X[.] ... X[k] is an increasing subsequence
        Container M ( n + 1 );
        M[0] = 0;

        /// Length of the longest (increasing) subsequence found so far
        size_type L = 0;

        for ( difference_type i = 0; i < n; ++i ) {
            // Binary search for the largest positive j <= L, such that X[M[j]]
            // < X[i]
            size_type lo = 1;
            size_type hi = L;
            while ( lo <= hi ) {
                size_type mid = ( lo + hi ) / 2;
                assert ( mid <= i );
                if ( compare ( *( first + M[mid] ),
                               *( first + i ) ) ) {  // X[M[mid]] < X[i]
                    lo = mid + 1;
                }
                else {
                    hi = mid - 1;
                }
            }

            // After searching, lo is 1 greater than the length of the longest
            // prefix of X[i]
            size_type newL = lo;

            // The predecessor of X[i] is the last index of the subsequence of
            // length newL-1
            assert ( newL > 0 );
            P[i] = M[newL - 1];

            assert ( newL <= n );
            assert ( M[newL] < n );
            if ( newL > L ) {
                // If we found a subsequence longer than any we've found yet,
                // update M and L
                M[newL] = i;
                L = newL;
            }
            else if ( compare ( *( first + i ),
                                *( first + M[newL] ) ) ) {  // X[i] < X[M[newL]]
                // If we found a smaller last value for the
                // subsequence of length newL, only update M
                M[newL] = i;
            }  // if
        }      // for

        // Reconstruct the longest increasing subsequence
        iterator_vector S ( L );
        size_type k = M[L];
        for ( size_type i = L - 1; i > 0; --i ) {
            S[i] = first + k;  // X[k];
            k = P[k];
        }
        S[0] = first + k;  // X[k];
        return S;
    }
    // \endcond
};

/// \fn longest_increasing_subsequence_length ( Iter first, Iter last )
/// \brief Searches the range [first, last) for the length of the longest
/// increasing subsequence.
///
/// \param first        The start of the sequence to search for (Forward Input
/// Iterator)
/// \param last         One past the end of the sequence to search for
///
template <typename Iter>
std::size_t
longest_increasing_subsequence_length ( Iter first, Iter last )
{
    longest_increasing_subsequence<Iter> lis;
    return lis.compute_length ( first, last );
}

template <typename Range>
std::size_t
longest_increasing_subsequence_length ( const Range &r )
{
    typedef typename boost::range_iterator<const Range>::type range_iterator;
    longest_increasing_subsequence<range_iterator> lis;
    return lis.compute_length ( boost::begin ( r ), boost::end ( r ) );
}
// TODO: do we need a non-const range overload?

/// \fn longest_increasing_subsequence_length ( Iter first, Iter last, Compare
/// cmp )
/// \brief Searches the range [first, last) for the length of the longest
/// subsequence ordered by custom predicate cmp.
///
/// \param first        The start of the sequence to search for (Forward Input
/// Iterator)
/// \param last         One past the end of the sequence to search for
/// \param cmp          Comparison predicate defining the ordering
///
template <typename Iter, typename Compare>
std::size_t
longest_increasing_subsequence_length ( Iter first, Iter last, Compare cmp )
{
    longest_increasing_subsequence<Iter, Compare> lis ( cmp );
    return lis.compute_length ( first, last );
}

template <typename Range, typename Compare>
std::size_t
longest_increasing_subsequence_length ( const Range &r, Compare cmp )
{
    typedef typename boost::range_iterator<const Range>::type range_iterator;
    longest_increasing_subsequence<range_iterator, Compare> lis ( cmp );
    return lis.compute_length ( boost::begin ( r ), boost::end ( r ) );
}
// TODO: do we need a non-const range overload?

/// \fn longest_increasing_subsequence_search ( Iter first, Iter last )
/// \brief Searches the range [first, last) for the longest increasing
/// subsequence.
///
/// \param first        The start of the sequence to search for (Forward Input
/// Iterator)
/// \param last         One past the end of the sequence to search for
///
template <typename Iter>
std::vector<Iter>
longest_increasing_subsequence_search ( Iter first, Iter last )
{
    longest_increasing_subsequence<Iter> lis;
    return lis ( first, last );
}
// TODO: range

/// \fn longest_increasing_subsequence_search ( Iter first, Iter last, Compare
/// cmp )
/// \brief Searches the range [first, last) for the longest subsequence ordered
/// by custom predicate cmp.
///
/// \param first        The start of the sequence to search for (Forward Input
/// Iterator)
/// \param last         One past the end of the sequence to search for
/// \param cmp          Comparison predicate defining the ordering
///
template <typename Iter, typename Compare>
std::vector<Iter>
longest_increasing_subsequence_search ( Iter first, Iter last, Compare cmp )
{
    longest_increasing_subsequence<Iter, Compare> lis ( cmp );
    return lis ( first, last );
}
// TODO: range

template <typename Range, typename OutputIter>
void
longest_increasing_subsequence_search ( const Range &sequence,
                                        OutputIter output_first )
{
    typedef typename boost::range_iterator<const Range>::type range_iterator;
    longest_increasing_subsequence<range_iterator> lis;
    return lis ( boost::begin ( sequence ), boost::end ( sequence ),
                 output_first );
}

//  Creator functions -- take a range, return an object
template <typename Range>
boost::algorithm::longest_increasing_subsequence<
    typename boost::range_iterator<const Range>::type>
make_longest_increasing_subsequence ( const Range &r )
{
    return boost::algorithm::longest_increasing_subsequence<
        typename boost::range_iterator<const Range>::type> ();
}

template <typename Range, typename Compare>
boost::algorithm::longest_increasing_subsequence<
    typename boost::range_iterator<const Range>::type, Compare>
make_longest_increasing_subsequence ( const Range &r, Compare cmp )
{
    return boost::algorithm::longest_increasing_subsequence<
        typename boost::range_iterator<const Range, Compare>::type> ( cmp );
}

template <typename Range>
boost::algorithm::longest_increasing_subsequence<
    typename boost::range_iterator<Range>::type>
make_longest_increasing_subsequence ( Range &r )
{
    return boost::algorithm::longest_increasing_subsequence<
        typename boost::range_iterator<Range>::type> ( boost::begin ( r ),
                                                       boost::end ( r ) );
}

template <typename Range, typename Compare>
boost::algorithm::longest_increasing_subsequence<
    typename boost::range_iterator<Range>::type, Compare>
make_longest_increasing_subsequence ( Range &r, Compare cmp )
{
    return boost::algorithm::longest_increasing_subsequence<
        typename boost::range_iterator<Range, Compare>::type> ( cmp );
}

}  // namespace algorithm
}  // namespace boost

#endif  //  BOOST_ALGORITHM_LONGEST_INCREASING_SUBSEQUENCE_HPP

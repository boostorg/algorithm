/*
   Copyright (c) Marek Kurdej 2014.

   Distributed under the Boost Software License, Version 1.0. (See accompanying
   file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

    For more information, see http://www.boost.org
*/

#ifndef BOOST_ALGORITHM_LONGEST_INCREASING_SUBSEQUENCE_HPP
#define BOOST_ALGORITHM_LONGEST_INCREASING_SUBSEQUENCE_HPP

#include <iterator>  // for std::iterator_traits
#include <vector>

#include <boost/assert.hpp>
#include <boost/static_assert.hpp>

#include <boost/range/begin.hpp>
#include <boost/range/end.hpp>

#include <boost/utility/enable_if.hpp>
#include <boost/type_traits/is_same.hpp>

// #define BOOST_ALGORITHM_LONGEST_INCREASING_SUBSEQUENCE_DEBUG
#ifdef BOOST_ALGORITHM_LONGEST_INCREASING_SUBSEQUENCE_DEBUG
#include <boost/algorithm/searching/detail/debugging.hpp>
#endif

namespace boost {
namespace algorithm {

/*
    A templated version of the longest (increasing) subsequence algorithm.

    Requirements:
        * Random access iterators

https://en.wikipedia.org/wiki/Longest_increasing_subsequence

*/

// TODO: doc
struct iterator_output_tag {
};
// TODO: doc
struct value_output_tag {
};

/// Longest increasing subsequence algorithm.
///
/// @time O(N logN)
/// @space O(N)
template < typename RandomIterator,
           typename Comparator = std::less<
               typename std::iterator_traits< RandomIterator >::value_type > >
class longest_increasing_subsequence {
    typedef typename std::iterator_traits< RandomIterator >::difference_type
        difference_type;
    typedef
        typename std::iterator_traits< RandomIterator >::value_type value_type;

public:
    typedef std::vector< RandomIterator > iterator_vector;
    typedef std::vector< value_type > value_vector;

    longest_increasing_subsequence ( Comparator cmp = Comparator () )
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
    /// The time complexity of the algorithm is log-linear O(n logn) where n is
    /// the length of the sequence (`std::distance(first, last)`).
    /// The space complexity is linear O(n).
    ///
    /// \param first        The start of the sequence to search (Random Access
    /// Iterator)
    /// \param last         One past the end of the sequence to search
    ///
    value_vector operator()( RandomIterator first, RandomIterator last,
                             value_output_tag tag ) const
    {
        return this->do_search_and_output ( first, last, tag );
    }

    iterator_vector operator()( RandomIterator first, RandomIterator last,
                                iterator_output_tag tag ) const
    {
        return this->do_search_and_output ( first, last, tag );
    }

    template < typename OutputIterator >
    OutputIterator operator()( RandomIterator first, RandomIterator last,
                               OutputIterator d_first ) const
    {
        return this->do_search_and_output ( first, last, d_first );
    }

    template < typename Range >
    value_vector operator()( const Range &r, value_output_tag tag ) const
    {
        return ( *this )( boost::begin ( r ), boost::end ( r ), tag );
    }

    template < typename Range >
    iterator_vector operator()( const Range &r, iterator_output_tag tag ) const
    {
        return ( *this )( boost::begin ( r ), boost::end ( r ), tag );
    }

    template < typename Range, typename OutputIterator >
    OutputIterator operator()( const Range &r, OutputIterator d_first ) const
    {
        return this->do_search_and_output ( boost::begin ( r ),
                                            boost::end ( r ), d_first );
    }

    std::size_t
    compute_length ( RandomIterator first, RandomIterator last ) const
    {
        return this->do_search ( first, last );
    }

    template < typename Range >
    std::size_t
    compute_length ( const Range &r ) const
    {
        // BOOST_STATIC_ASSERT ( ( boost::is_same<
        //     typename std::iterator_traits<RandomIterator>::value_type,
        //     typename std::iterator_traits<Range>::value_type>::value ) );
        return ( *this )( boost::begin ( r ), boost::end ( r ) );
    }

private:
    /// \cond DOXYGEN_HIDE
    Comparator compare;

    typedef std::size_t size_type;
    typedef std::vector< size_type > index_vector;

    size_type
    do_search ( RandomIterator first, RandomIterator last ) const
    {
        index_vector predecessor;
        index_vector lis_tail;
        return do_search ( first, last, predecessor, lis_tail );
    }

    size_type
    do_search ( RandomIterator first, RandomIterator last,
                /*output*/ index_vector &predecessor,
                /*output*/ index_vector &lis_tail ) const
    {
        difference_type const n = std::distance ( first, last );
        /// Length of the longest (increasing) subsequence found so far
        size_type lis_length = 0;
        /// predecessor[k, 0 <= k < n] - stores the index of the predecessor of
        /// X[k] in the longest increasing subsequence ending at X[k]
        predecessor.resize ( n );
        /// lis_tail[j, 0 <= k <= n] - stores the index k of the smallest value
        /// X[k] such that there is an increasing subsequence of length j ending
        /// at X[k] on the range k <= i (note we have j <= k <= i here, because
        /// j represents the length of the increasing subsequence, and k
        /// represents the index of its termination.
        /// Obviously, we can never have an increasing subsequence of length 13
        /// ending at index 11. k <= i by definition).
        lis_tail.resize ( n + 1 );

        for ( difference_type i = 0; i < n; ++i ) {
            // Binary search for the largest positive j <= lis_length, such that
            // X[lis_tail[j]] < X[i].
            // After searching, lo is 1 greater than the length of the longest
            // prefix of X[i].
            size_type new_lis_length;
            {
                size_type lo = 1;
                size_type hi = lis_length;
                while ( lo <= hi ) {
                    size_type mid = ( lo + hi ) / 2;
                    assert ( mid <= i );
                    if ( compare (
                             *( first + lis_tail[mid] ),
                             *( first + i ) ) ) {  // X[lis_tail[mid]] < X[i]
                        lo = mid + 1;
                    }
                    else {
                        hi = mid - 1;
                    }
                }
                new_lis_length = lo;
            }

            // The predecessor of X[i] is the last index of the subsequence of
            // length new_lis_length-1
            assert ( new_lis_length > 0 );
            predecessor[i] = lis_tail[new_lis_length - 1];

            assert ( new_lis_length <= n );
            assert ( lis_tail[new_lis_length] < n );
            if ( new_lis_length > lis_length ) {
                // If we found a subsequence longer than any we have found yet,
                // update lis_tail and lis_length
                lis_tail[new_lis_length] = i;
                lis_length = new_lis_length;
            }
            else if ( compare ( *( first + i ),
                                *( first + lis_tail[new_lis_length] ) ) ) {
                // X[i] < X[lis_tail[new_lis_length]]
                // If we found a smaller last value for the subsequence of
                // length new_lis_length, only update lis_tail
                lis_tail[new_lis_length] = i;
            }
        }
        return lis_length;
    }

    template < typename OutputIterator >
    OutputIterator
    do_output ( RandomIterator first, RandomIterator /*last*/,
                size_type lis_length, index_vector const &predecessor,
                index_vector const &lis_tail, OutputIterator d_first ) const
    {
        // Reconstruct the longest increasing subsequence
        index_vector lis ( lis_length );
        size_type k = lis_tail[lis_length];
        for ( size_type i = lis_length; i--; ) {
            lis[i] = k;  // X[k];
            k = predecessor[k];
        }
        // Output the found subsequence
        for ( size_type i = 0; i < lis_length; ++i ) {
            *d_first++ = *( first + lis[i] );
        }
        return d_first;
    }

    value_vector
    do_output ( RandomIterator first, RandomIterator /*last*/,
                size_type lis_length, index_vector const &predecessor,
                index_vector const &lis_tail, value_output_tag /*tag*/ ) const
    {
        // Reconstruct the longest increasing subsequence values
        value_vector lis ( lis_length );
        size_type k = lis_tail[lis_length];
        for ( size_type i = lis_length; i--; ) {
            lis[i] = *( first + k );  // X[k];
            k = predecessor[k];
        }
        return lis;
    }

    iterator_vector
    do_output ( RandomIterator first, RandomIterator /*last*/,
                size_type lis_length, index_vector const &predecessor,
                index_vector const &lis_tail,
                iterator_output_tag /*tag*/ ) const
    {
        // Reconstruct the longest increasing subsequence iterators
        iterator_vector lis ( lis_length );
        size_type k = lis_tail[lis_length];
        for ( size_type i = lis_length; i--; ) {
            lis[i] = first + k;  // &X[k];
            k = predecessor[k];
        }
        return lis;
    }

    template < typename OutputIterator >
    OutputIterator
    do_search_and_output ( RandomIterator first, RandomIterator last,
                           OutputIterator d_first ) const
    {
        index_vector predecessor;
        index_vector lis_tail;
        size_type lis_length = do_search ( first, last, predecessor, lis_tail );
        return do_output ( first, last, lis_length, predecessor, lis_tail,
                           d_first );
    }

    value_vector
    do_search_and_output ( RandomIterator first, RandomIterator last,
                           value_output_tag tag ) const
    {
        index_vector predecessor;
        index_vector lis_tail;
        size_type lis_length = do_search ( first, last, predecessor, lis_tail );
        return this->do_output ( first, last, lis_length, predecessor, lis_tail,
                                 tag );
    }

    iterator_vector
    do_search_and_output ( RandomIterator first, RandomIterator last,
                           iterator_output_tag tag ) const
    {
        index_vector predecessor;
        index_vector lis_tail;
        size_type lis_length = do_search ( first, last, predecessor, lis_tail );
        return this->do_output ( first, last, lis_length, predecessor, lis_tail,
                                 tag );
    }
    // \endcond
};

/// \fn longest_increasing_subsequence_length ( RandomIterator first,
/// RandomIterator last )
/// \brief Searches the range [first, last) for the length of the longest
/// increasing subsequence.
///
/// \param first        The start of the sequence to search for (Forward Input
/// Iterator)
/// \param last         One past the end of the sequence to search for
///
template < typename RandomIterator >
std::size_t
longest_increasing_subsequence_length ( RandomIterator first,
                                        RandomIterator last )
{
    longest_increasing_subsequence< RandomIterator > lis;
    return lis.compute_length ( first, last );
}

template < typename Range >
std::size_t
longest_increasing_subsequence_length ( const Range &r )
{
    typedef typename boost::range_iterator< const Range >::type range_iterator;
    longest_increasing_subsequence< range_iterator > lis;
    return lis.compute_length ( boost::begin ( r ), boost::end ( r ) );
}
// TODO: do we need a non-const range overload?

/// \fn longest_increasing_subsequence_length ( RandomIterator first,
/// RandomIterator last, Comparator
/// cmp )
/// \brief Searches the range [first, last) for the length of the longest
/// subsequence ordered by custom predicate cmp.
///
/// \param first        The start of the sequence to search for (Forward Input
/// Iterator)
/// \param last         One past the end of the sequence to search for
/// \param cmp          Comparison predicate defining the ordering
///
template < typename RandomIterator, typename Comparator >
std::size_t
longest_increasing_subsequence_length ( RandomIterator first,
                                        RandomIterator last, Comparator cmp )
{
    longest_increasing_subsequence< RandomIterator, Comparator > lis ( cmp );
    return lis.compute_length ( first, last );
}

template < typename Range, typename Comparator >
typename boost::disable_if< boost::is_same< Range, Comparator >,
                            std::size_t >::type
longest_increasing_subsequence_length ( const Range &r, Comparator cmp )
{
    typedef typename boost::range_iterator< const Range >::type range_iterator;
    longest_increasing_subsequence< range_iterator, Comparator > lis ( cmp );
    return lis.compute_length ( boost::begin ( r ), boost::end ( r ) );
}
// TODO: do we need a non-const range overload?

/// \fn longest_increasing_subsequence_search ( RandomIterator first,
/// RandomIterator last )
/// \brief Searches the range [first, last) for the longest increasing
/// subsequence.
///
/// \param first        The start of the sequence to search for (Forward Input
/// Iterator)
/// \param last         One past the end of the sequence to search for

/// \fn longest_increasing_subsequence_search ( RandomIterator first,
/// RandomIterator last, Comparator cmp )
/// \brief Searches the range [first, last) for the longest subsequence ordered
/// by custom predicate cmp.
/// \param first        The start of the sequence to search for (Forward Input
/// Iterator)
/// \param last         One past the end of the sequence to search for
/// \param cmp          Comparison predicate defining the ordering
///

// Vector of values
template < typename RandomIterator >
typename longest_increasing_subsequence< RandomIterator >::value_vector
longest_increasing_subsequence_search ( RandomIterator first,
                                        RandomIterator last,
                                        value_output_tag tag )
{
    longest_increasing_subsequence< RandomIterator > lis;
    return lis ( first, last, tag );
}

template < typename RandomIterator, typename Comparator >
typename longest_increasing_subsequence< RandomIterator,
                                         Comparator >::value_vector
longest_increasing_subsequence_search ( RandomIterator first,
                                        RandomIterator last,
                                        value_output_tag tag, Comparator cmp )
{
    longest_increasing_subsequence< RandomIterator, Comparator > lis ( cmp );
    return lis ( first, last, tag );
}

// Vector of iterators
template < typename RandomIterator >
typename longest_increasing_subsequence< RandomIterator >::iterator_vector
longest_increasing_subsequence_search ( RandomIterator first,
                                        RandomIterator last,
                                        iterator_output_tag tag )
{
    longest_increasing_subsequence< RandomIterator > lis;
    return lis ( first, last, tag );
}

template < typename RandomIterator, typename Comparator >
typename longest_increasing_subsequence< RandomIterator,
                                         Comparator >::iterator_vector
longest_increasing_subsequence_search ( RandomIterator first,
                                        RandomIterator last,
                                        iterator_output_tag tag,
                                        Comparator cmp )
{
    longest_increasing_subsequence< RandomIterator, Comparator > lis ( cmp );
    return lis ( first, last, tag );
}

// Output iterator
template < typename RandomIterator, typename OutputIterator >
OutputIterator
longest_increasing_subsequence_search ( RandomIterator first,
                                        RandomIterator last,
                                        OutputIterator d_first )
{
    longest_increasing_subsequence< RandomIterator > lis;
    return lis ( first, last, d_first );
}

template < typename RandomIterator, typename OutputIterator,
           typename Comparator >
OutputIterator
longest_increasing_subsequence_search ( RandomIterator first,
                                        RandomIterator last,
                                        OutputIterator d_first, Comparator cmp )
{
    longest_increasing_subsequence< RandomIterator, Comparator > lis ( cmp );
    return lis ( first, last, d_first );
}

// Range - Vector of values
template < typename Range >
typename longest_increasing_subsequence<
    typename boost::range_iterator< const Range >::type >::value_vector
longest_increasing_subsequence_search ( const Range &sequence,
                                        value_output_tag tag )
{
    typedef typename boost::range_iterator< const Range >::type range_iterator;
    longest_increasing_subsequence< range_iterator > lis;
    return lis ( boost::begin ( sequence ), boost::end ( sequence ), tag );
}

template < typename Range, typename Comparator >
typename longest_increasing_subsequence<
    typename boost::range_iterator< const Range >::type,
    Comparator >::value_vector
longest_increasing_subsequence_search ( const Range &sequence,
                                        value_output_tag tag, Comparator cmp )
{
    typedef typename boost::range_iterator< const Range >::type range_iterator;
    longest_increasing_subsequence< range_iterator, Comparator > lis ( cmp );
    return lis ( boost::begin ( sequence ), boost::end ( sequence ), tag );
}

// Range - Vector of iterators
template < typename Range >
typename longest_increasing_subsequence<
    typename boost::range_iterator< const Range >::type >::iterator_vector
longest_increasing_subsequence_search ( const Range &sequence,
                                        iterator_output_tag tag )
{
    typedef typename boost::range_iterator< const Range >::type range_iterator;
    longest_increasing_subsequence< range_iterator > lis;
    return lis ( boost::begin ( sequence ), boost::end ( sequence ), tag );
}

template < typename Range, typename Comparator >
typename longest_increasing_subsequence<
    typename boost::range_iterator< const Range >::type,
    Comparator >::iterator_vector
longest_increasing_subsequence_search ( const Range &sequence,
                                        iterator_output_tag tag,
                                        Comparator cmp )
{
    typedef typename boost::range_iterator< const Range >::type range_iterator;
    longest_increasing_subsequence< range_iterator, Comparator > lis ( cmp );
    return lis ( boost::begin ( sequence ), boost::end ( sequence ), tag );
}

// Range - Output Iterator
template < typename Range, typename OutputIterator >
OutputIterator
longest_increasing_subsequence_search ( const Range &sequence,
                                        OutputIterator d_first )
{
    typedef typename boost::range_iterator< const Range >::type range_iterator;
    longest_increasing_subsequence< range_iterator > lis;
    return lis ( boost::begin ( sequence ), boost::end ( sequence ), d_first );
}

// Use a bit of TMP to disambiguate the 3-argument templates
template < typename Range, typename OutputIterator, typename Comparator >
typename boost::disable_if< boost::is_same< Range, OutputIterator >,
                            OutputIterator >::type
longest_increasing_subsequence_search ( const Range &sequence,
                                        OutputIterator d_first, Comparator cmp )
{
    typedef typename boost::range_iterator< const Range >::type range_iterator;
    longest_increasing_subsequence< range_iterator, Comparator > lis ( cmp );
    return lis ( boost::begin ( sequence ), boost::end ( sequence ), d_first );
}

//  Creator functions -- take a range, return an object
template < typename Range >
boost::algorithm::longest_increasing_subsequence<
    typename boost::range_iterator< const Range >::type >
make_longest_increasing_subsequence ( const Range &r )
{
    return boost::algorithm::longest_increasing_subsequence<
        typename boost::range_iterator< const Range >::type > ();
}

template < typename Range, typename Comparator >
boost::algorithm::longest_increasing_subsequence<
    typename boost::range_iterator< const Range >::type, Comparator >
make_longest_increasing_subsequence ( const Range &r, Comparator cmp )
{
    return boost::algorithm::longest_increasing_subsequence<
        typename boost::range_iterator< const Range, Comparator >::type > (
        cmp );
}

template < typename Range >
boost::algorithm::longest_increasing_subsequence<
    typename boost::range_iterator< Range >::type >
make_longest_increasing_subsequence ( Range &r )
{
    return boost::algorithm::longest_increasing_subsequence<
        typename boost::range_iterator< Range >::type > ( boost::begin ( r ),
                                                          boost::end ( r ) );
}

template < typename Range, typename Comparator >
boost::algorithm::longest_increasing_subsequence<
    typename boost::range_iterator< Range >::type, Comparator >
make_longest_increasing_subsequence ( Range &r, Comparator cmp )
{
    return boost::algorithm::longest_increasing_subsequence<
        typename boost::range_iterator< Range, Comparator >::type > ( cmp );
}

}  // namespace algorithm
}  // namespace boost

#endif  //  BOOST_ALGORITHM_LONGEST_INCREASING_SUBSEQUENCE_HPP

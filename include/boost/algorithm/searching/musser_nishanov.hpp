/* 
   Copyright (c) Dave R. Musser & Gor V. Nishanov 1997.
   Copyright (c) Jeremy W. Murphy 2016.

   Distributed under the Boost Software License, Version 1.0. (See accompanying
   file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

    For more information, see http://www.boost.org
*/

#ifndef BOOST_ALGORITHM_SEARCH_MUSSER_NISHANOV_HPP
#define BOOST_ALGORITHM_SEARCH_MUSSER_NISHANOV_HPP

#include <boost/algorithm/searching/detail/mn_traits.hpp>
#include <boost/array.hpp>
#include <boost/bind.hpp>
#include <boost/function.hpp>
#include <boost/mpl/bool.hpp>
#include <boost/mpl/and.hpp>
#include <boost/mpl/or.hpp>
#include <boost/next_prior.hpp>
#include <boost/type_traits/is_base_of.hpp>
#include <boost/type_traits/is_same.hpp>
#include <boost/utility/enable_if.hpp>

#include <iterator>
#include <utility>
#include <vector>


namespace boost { namespace algorithm {

namespace detail {
    
/**
 * @brief Accelerated Linear search.
 * 
 * Accelerated Linear (AL) search by Musser & Nishanov.
 * 
 */
template <typename PatIter, typename CorpusIter = PatIter>
class accelerated_linear
{
    BOOST_STATIC_ASSERT (( boost::is_same<
    typename std::iterator_traits<PatIter>::value_type, 
    typename std::iterator_traits<CorpusIter>::value_type>::value ));
public:    
    typedef typename std::iterator_traits<PatIter>::difference_type pattern_difference_type;
    typedef typename std::iterator_traits<CorpusIter>::difference_type corpus_difference_type;
    
protected:
    PatIter pat_first, pat_last;
    std::vector<corpus_difference_type> next_;
    pattern_difference_type k_pattern_length;

private:
    void compute_next()
    {
        pattern_difference_type j = 0, t = -1;
        next_.reserve(k_pattern_length);
        next_.push_back(-1);
        while (j < k_pattern_length - 1)
        {
            while (t >= 0 && pat_first[j] != pat_first[t])
                t = next_[t];
            ++j;
            ++t;
            next_.push_back(pat_first[j] == pat_first[t] ? next_[t] : t);
        }
    }

public:
    std::pair<CorpusIter, CorpusIter> operator()(CorpusIter corpus_first, CorpusIter corpus_last) const
    {
        BOOST_ASSERT(std::distance(pat_first, pat_last) == k_pattern_length);
        BOOST_ASSERT(size_t(k_pattern_length) == next_.size());

        using std::find;
        using std::make_pair;
        
        if (pat_first == pat_last)
            return make_pair(corpus_first, corpus_first);            
        
        PatIter p1;

        if (next_.size() == 1)
        {
            CorpusIter const result = find(corpus_first, corpus_last, *pat_first);
            return result == corpus_last ? make_pair(corpus_last, corpus_last) : make_pair(result, boost::next(result));
        }
        p1 = pat_first;
        ++p1;
        while (corpus_first != corpus_last)
        {
            corpus_first = find(corpus_first, corpus_last, *pat_first);
            if (corpus_first == corpus_last)
                return make_pair(corpus_last, corpus_last);
            CorpusIter hold = corpus_first;
            if (++corpus_first == corpus_last)
                return make_pair(corpus_last, corpus_last);
            PatIter p = p1;
            pattern_difference_type j = 1;
            while (*corpus_first == *p)
            {
                if (++p == pat_last)
                    return make_pair(hold, boost::next(hold, k_pattern_length));
                if (++corpus_first == corpus_last)
                    return make_pair(corpus_last, corpus_last);
                ++j;
            }
            
            for (;;)
            {
                j = next_[j];
                if (j < 0)
                {
                    ++corpus_first;
                    break;
                }
                if (j == 0)
                    break;
                p = pat_first + j;
                while (*corpus_first == *p)
                {
                    ++corpus_first;
                    ++p;
                    ++j;
                    if (p == pat_last)
                    {
                        CorpusIter succesor = hold;
                        std::advance(succesor, next_.size());
                        while (succesor != corpus_first)
                            ++succesor, ++hold;
                        return make_pair(hold, boost::next(hold, k_pattern_length));
                    }
                    if (corpus_first == corpus_last)
                        return make_pair(corpus_last, corpus_last);
                }
            }
        }
        return make_pair(corpus_last, corpus_last);
    }
    
    accelerated_linear(PatIter pat_first, PatIter pat_last) : pat_first(pat_first), pat_last(pat_last), k_pattern_length(std::distance(pat_first, pat_last))
    {
        if (k_pattern_length > 0)
            compute_next();
    }
};


/**
 * Musser-Nishanov Hashed Accelerated Linear (HAL) search algorithm.
 */
template <typename PatIter, typename CorpusIter, typename Trait>
class hashed_accelerated_linear : private boost::algorithm::detail::accelerated_linear<PatIter, CorpusIter>
{
    typedef boost::algorithm::detail::accelerated_linear<PatIter, CorpusIter> AcceleratedLinear;
    
    using typename AcceleratedLinear::pattern_difference_type;
    using typename AcceleratedLinear::corpus_difference_type;
    using AcceleratedLinear::k_pattern_length;
    using AcceleratedLinear::pat_first;
    using AcceleratedLinear::pat_last;
    using AcceleratedLinear::next_;
    
    boost::array<corpus_difference_type, Trait::hash_range_max> skip;
    corpus_difference_type mismatch_shift;
    boost::function<std::pair<CorpusIter, CorpusIter>(CorpusIter, CorpusIter)> search;
    
    std::pair<CorpusIter, CorpusIter> nul_pattern(CorpusIter corpus_first, CorpusIter) const
    {
        return std::make_pair(corpus_first, corpus_first);
    }    
    
    std::pair<CorpusIter, CorpusIter> HAL(CorpusIter corpus_first, CorpusIter corpus_last)
    {
        BOOST_ASSERT(pat_first != pat_last);
        BOOST_ASSERT(std::distance(pat_first, pat_last) == k_pattern_length);
        BOOST_ASSERT(size_t(k_pattern_length) == next_.size());
        
        using std::make_pair;
        
        corpus_difference_type const k_corpus_length = corpus_last - corpus_first;
        corpus_difference_type const adjustment = k_corpus_length + k_pattern_length;
        // NOTE: The following line prevents this function from being const.
        skip[Trait::hash(pat_first + k_pattern_length - 1)] = k_corpus_length + 1;
        corpus_difference_type k = -k_corpus_length;
        for (;;)
        {
            k += k_pattern_length - 1;
            if (k >= 0) break;
            do   // this loop is hot for data read
            {
                corpus_difference_type const increment = skip[Trait::hash(corpus_last + k)];
                k += increment;
            }
            while (k < 0);
            if (k < k_pattern_length)
                return make_pair(corpus_last, corpus_last);
            k -= adjustment;
            
            BOOST_ASSERT(k < 0);
            if (corpus_last[k] != pat_first[0])
                k += mismatch_shift;
            else
            {
                pattern_difference_type j = 1;
                for (;;)
                {
                    ++k;
                    if (corpus_last[k] != pat_first[j])
                        break;
                    ++j;
                    if (j == k_pattern_length)
                        return std::make_pair(corpus_last + k - k_pattern_length + 1, corpus_last + k + 1);
                }
                
                if (mismatch_shift > j)
                    k += mismatch_shift - j;
                else
                    
                    for (;;)
                    {
                        j = next_[j];
                        if (j < 0)
                        {
                            ++k;
                            break;
                        }
                        if (j == 0)
                            break;
                        while (corpus_last[k] == pat_first[j])
                        {
                            ++k;
                            ++j;
                            if (j == k_pattern_length)
                            {
                                return make_pair(corpus_last + k - k_pattern_length, corpus_last + k);
                            }
                            if (k == 0)
                                return make_pair(corpus_last, corpus_last);
                        }
                    }
            }
        }
        return make_pair(corpus_last, corpus_last);
    }
    
    
    void compute_skip()
    {
        BOOST_ASSERT(next_.size() >= Trait::suffix_size);
        
        pattern_difference_type const m = next_.size();
        std::fill(skip.begin(), skip.end(), m - Trait::suffix_size + 1);
        for (pattern_difference_type j = Trait::suffix_size - 1; j < m - 1; ++j)
            skip[Trait::hash(pat_first + j)] = m - 1 - j;
        mismatch_shift = skip[Trait::hash(pat_first + m - 1)];
        skip[Trait::hash(pat_first + m - 1)] = 0;
    }
    
    
    std::pair<CorpusIter, CorpusIter> AL(CorpusIter corpus_first, CorpusIter corpus_last)
    {
        return AcceleratedLinear::operator()(corpus_first, corpus_last);
    }
    
public:
    hashed_accelerated_linear(PatIter pat_first, PatIter pat_last) : AcceleratedLinear(pat_first, pat_last)
    {
        if (k_pattern_length > 0)
        {
            if (k_pattern_length < Trait::suffix_size || k_pattern_length == 1)
                search = bind(&hashed_accelerated_linear::AL, this, _1, _2);
            else
            {
                search = bind(&hashed_accelerated_linear::HAL, this, _1, _2);
                compute_skip();
            }
        }
        else
            search = bind(&hashed_accelerated_linear::nul_pattern, this, _1, _2);
    }
    
    std::pair<CorpusIter, CorpusIter> operator()(CorpusIter corpus_first, CorpusIter corpus_last)
    {
        return search(corpus_first, corpus_last);
    }
};

} // namespace detail


/**
 * One class, two identities based on corpus iterator and the suffix size trait.
 */
template <typename PatIter, typename CorpusIter = PatIter, typename Trait = search_trait<typename std::iterator_traits<PatIter>::value_type>, typename Enable = void>
class musser_nishanov;


/**
 * Musser-Nishanov Accelerated Linear search algorithm.
 */
template <typename PatIter, typename CorpusIter, typename Trait>
class musser_nishanov<PatIter, CorpusIter, Trait, 
typename disable_if<
    typename boost::mpl::and_<
        boost::is_base_of<std::random_access_iterator_tag, typename std::iterator_traits<CorpusIter>::iterator_category>,
        boost::mpl::bool_<Trait::suffix_size>
    >::type 
>::type> : public boost::algorithm::detail::accelerated_linear<PatIter, CorpusIter>
{
    typedef boost::algorithm::detail::accelerated_linear<PatIter, CorpusIter> AcceleratedLinear;
    
public:
    musser_nishanov(PatIter pat_first, PatIter pat_last) : AcceleratedLinear(pat_first, pat_last) {}
};


/**
 * Musser-Nishanov Hashed Accelerated Linear search algorithm.
 */
template <typename PatIter, typename CorpusIter, typename Trait>
class musser_nishanov<PatIter, CorpusIter, Trait, 
typename enable_if<
    typename boost::mpl::and_<
        boost::is_base_of<std::random_access_iterator_tag, typename std::iterator_traits<CorpusIter>::iterator_category>,
        boost::mpl::bool_<Trait::suffix_size> 
    >::type 
>::type> : public boost::algorithm::detail::hashed_accelerated_linear<PatIter, CorpusIter, Trait>
{
    typedef boost::algorithm::detail::hashed_accelerated_linear<PatIter, CorpusIter, Trait> HashedAcceleratedLinear;
public:
    musser_nishanov(PatIter pat_first, PatIter pat_last) : HashedAcceleratedLinear(pat_first, pat_last) {}
};


template <typename PatIter, typename CorpusIter>
std::pair<CorpusIter, CorpusIter> musser_nishanov_search(CorpusIter corpus_first, CorpusIter corpus_last, PatIter pat_first, PatIter pat_last)
{
    musser_nishanov<PatIter, CorpusIter> mn(pat_first, pat_last);
    return mn(corpus_first, corpus_last);
}

}} // namespace boost::algorithm

#endif

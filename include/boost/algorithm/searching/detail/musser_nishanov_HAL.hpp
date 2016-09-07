#ifndef BOOST_ALGORITHM_SEARCH_MUSSER_NISHANOV_HAL_HPP
#define BOOST_ALGORITHM_SEARCH_MUSSER_NISHANOV_HAL_HPP

#include <boost/algorithm/searching/detail/mn_traits.hpp>

#include <boost/array.hpp>

#include <boost/static_assert.hpp>
#include <boost/assert.hpp>

#include <iterator>
#include <algorithm>
#include <vector>


namespace boost { namespace algorithm {

    
template <typename PatIter, typename CorpusIter, typename Trait = search_trait<typename std::iterator_traits<PatIter>::value_type> >
class musser_nishanov_HAL
{
    typedef typename std::iterator_traits<PatIter>::difference_type pattern_difference_type;
    typedef typename std::iterator_traits<CorpusIter>::difference_type corpus_difference_type;
    
    PatIter pat_first, pat_last;
    std::vector<corpus_difference_type> next;
    boost::array<corpus_difference_type, Trait::hash_range_max> skip;
    pattern_difference_type k_pattern_length;
    corpus_difference_type mismatch_shift;

    void compute_next()
    {
        pattern_difference_type j = 0, t = -1;
        next.reserve(k_pattern_length);
        next.push_back(-1);
        while (j < k_pattern_length - 1)
        {
            while (t >= 0 && pat_first[j] != pat_first[t])
                t = next[t];
            ++j;
            ++t;
            next.push_back(pat_first[j] == pat_first[t] ? next[t] : t);
        }
    }
    
    void compute_skip()
    {
        pattern_difference_type const m = next.size();
        std::fill(skip.begin(), skip.end(), m - Trait::suffix_size + 1);
        for (pattern_difference_type j = Trait::suffix_size - 1; j < m - 1; ++j)
        {
            // unsigned char const index = Trait::hash(pat_first + j);
            skip[Trait::hash(pat_first + j)] = m - 1 - j;
        }
        mismatch_shift = skip[Trait::hash(pat_first + m - 1)];
        skip[Trait::hash(pat_first + m - 1)] = 0;
    }
    
    std::pair<CorpusIter, CorpusIter>
    do_search(CorpusIter corpus_first, CorpusIter corpus_last)
    {
        pattern_difference_type j;
        corpus_difference_type large, adjustment;
        corpus_difference_type const k_corpus_length = corpus_last - corpus_first;
        // Original location of compute_next.
        
        large = k_corpus_length + 1;
        adjustment = large + k_pattern_length - 1;
        skip[Trait::hash(pat_first + k_pattern_length - 1)] = large;
        corpus_difference_type k = -k_corpus_length;
        for (;;)
        {
            k += k_pattern_length - 1;
            if (k >= 0) break;
            do   // this loop is hot for data read
            {
                // unsigned char const index = Trait::hash(corpus_last + k);
                corpus_difference_type const increment = skip[Trait::hash(corpus_last + k)];
                k += increment;
            }
            while (k < 0);
            if (k < k_pattern_length)
                return std::make_pair(corpus_last, corpus_last);
            k -= adjustment;
            
            if (corpus_last[k] != pat_first[0])
                k += mismatch_shift;
            else
            {
                j = 1;
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
                        j = next[j];
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
                                return std::make_pair(corpus_last + k - k_pattern_length, corpus_last + k);
                            }
                            if (k == 0)
                                return std::make_pair(corpus_last, corpus_last);
                        }
                    }
            }
        }
        return std::make_pair(corpus_last, corpus_last);
    }
    
public:
    musser_nishanov_HAL(PatIter first, PatIter last) : pat_first(first), pat_last(last), k_pattern_length(std::distance(first, last)) 
    {
        if (k_pattern_length > 0)
        {
            compute_next();
            compute_skip();
        }
    }
    
    std::pair<CorpusIter, CorpusIter>
    operator()(CorpusIter corpus_first, CorpusIter corpus_last)
    {
        BOOST_STATIC_ASSERT (( boost::is_same<
        typename std::iterator_traits<PatIter>::value_type, 
        typename std::iterator_traits<CorpusIter>::value_type>::value ));
        
        BOOST_STATIC_ASSERT((Trait::suffix_size != 0));
        
        if (corpus_first == corpus_last) return std::make_pair(corpus_last, corpus_last);   // if nothing to search, we didn't find it!
        if (pat_first == pat_last ) return std::make_pair(corpus_first, corpus_first); // empty pattern matches at start
        
        BOOST_ASSERT(Trait::suffix_size < k_pattern_length);
        const corpus_difference_type k_corpus_length  = std::distance ( corpus_first, corpus_last );
        //  If the pattern is larger than the corpus, we can't find it!
        if ( k_corpus_length < k_pattern_length ) 
            return std::make_pair(corpus_last, corpus_last);
        
        //  Do the search 
        return do_search ( corpus_first, corpus_last );
    }
};


template <typename PatIter, typename CorpusIter>
std::pair<CorpusIter, CorpusIter> 
musser_nishanov_HAL_search(CorpusIter corpus_first, CorpusIter corpus_last,
                            PatIter pat_first, PatIter pat_last)
{
    musser_nishanov_HAL<PatIter, CorpusIter> searcher(pat_first, pat_last);
    return searcher(corpus_first, corpus_last);
}

}} // namespace boost::algorithm

#endif

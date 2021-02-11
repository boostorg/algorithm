/*
   Copyright (c) David R. Musser & Gor V. Nishanov 1997.
   Copyright (c) Jeremy W. Murphy 2016.

   Distributed under the Boost Software License, Version 1.0. (See accompanying
   file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

    For more information, see http://www.boost.org
*/

#include <boost/algorithm/searching/detail/mn_traits.hpp>
#include <boost/config.hpp>
#include <boost/range.hpp>

#include <array>
#include <vector>

namespace boost { namespace algorithm {

/**
 * Musser-Nishanov Hashed Accelerated Linear (HAL) search algorithm.
 */
template <typename PatIter, typename CorpusIter, typename Trait>
class hashed_accelerated_linear
{
public:
    typedef typename std::iterator_traits<PatIter>::difference_type pattern_difference_type;
    typedef typename std::iterator_traits<CorpusIter>::difference_type corpus_difference_type;
    using skip_container = std::array<corpus_difference_type, Trait::hash_range_max>;

private:
    PatIter pat_first, pat_last;
    std::vector<corpus_difference_type> next_;
    pattern_difference_type pattern_length;
    mutable skip_container skip_;
    corpus_difference_type mismatch_shift;

public:
    // This is the real HAL algorithm.
    template <typename I>
    std::pair<CorpusIter, CorpusIter>
    HAL(CorpusIter corpus_first, CorpusIter corpus_last, I skip) const
    {
        BOOST_CONCEPT_ASSERT((boost::Mutable_RandomAccessIterator<I>));

        BOOST_ASSERT(pat_first != pat_last);
        BOOST_ASSERT(std::distance(pat_first, pat_last) == pattern_length);
        BOOST_ASSERT(size_t(pattern_length) == next_.size());

        using std::make_pair;

        corpus_difference_type const corpus_length = corpus_last - corpus_first;
        corpus_difference_type const adjustment = corpus_length + pattern_length;
        // NOTE: This assignment requires the skip iterator to be mutable, and
        // the implementation would be greatly simplified if a way around it could be found.
        skip[Trait::hash(pat_first + pattern_length - 1)] = corpus_length + 1;
        corpus_difference_type k = -corpus_length;
        for (;;)
        {
            k += pattern_length - 1;
            if (k >= 0)
                break;
            do   // this loop is hot for data read
            {
                corpus_difference_type const increment = skip[Trait::hash(corpus_last + k)];
                k += increment;
            }
            while (k < 0);
            if (k < pattern_length)
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
                    if (j == pattern_length)
                        return std::make_pair(corpus_last + k - pattern_length + 1, corpus_last + k + 1);
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
                            if (j == pattern_length)
                            {
                                return make_pair(corpus_last + k - pattern_length, corpus_last + k);
                            }
                            if (k == 0)
                                return make_pair(corpus_last, corpus_last);
                        }
                    }
            }
        }
        return make_pair(corpus_last, corpus_last);
    }


    void compute_next() BOOST_NOEXCEPT
    {
        BOOST_ASSERT(pattern_length > 0);
        pattern_difference_type j = 0, t = -1;
        next_.push_back(-1);
        while (j < pattern_length - 1)
        {
            while (t >= 0 && pat_first[j] != pat_first[t])
                t = next_[t];
            j++;
            t++;
            next_.push_back(pat_first[j] == pat_first[t] ? next_[t] : t);
        }
    }

    void compute_skip()
    {
        BOOST_ASSERT(next_.size() >= Trait::suffix_size);

        pattern_difference_type const m = next_.size();
        std::fill(skip_.begin(), skip_.end(), m - Trait::suffix_size + 1);
        for (pattern_difference_type j = Trait::suffix_size - 1; j < m - 1; ++j)
            skip_[Trait::hash(pat_first + j)] = m - 1 - j;
        mismatch_shift = skip_[Trait::hash(pat_first + m - 1)];
        skip_[Trait::hash(pat_first + m - 1)] = 0;
    }

public:
    hashed_accelerated_linear(PatIter pat_first, PatIter pat_last)
      : pat_first{pat_first}, pat_last{pat_last}, pattern_length{pat_last - pat_first}
    {
        compute_next();
        compute_skip();
    }

    std::pair<CorpusIter, CorpusIter>
    operator()(CorpusIter corpus_first, CorpusIter corpus_last) const
    {
        return HAL(corpus_first, corpus_last, boost::begin(skip_));
    }
};

}}

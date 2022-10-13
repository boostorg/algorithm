/*
   Copyright (c) David R. Musser & Gor V. Nishanov 1997.
   Copyright (c) Jeremy W. Murphy 2016.

   Distributed under the Boost Software License, Version 1.0. (See accompanying
   file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

    For more information, see http://www.boost.org
*/

#include <boost/config.hpp>
#include <boost/range.hpp>
#include <boost/static_assert.hpp>
#include <boost/type_traits/is_base_of.hpp>
#include <boost/type_traits/is_same.hpp>

#include <iterator>
#include <vector>

namespace boost { namespace algorithm {

/**
 * @brief Accelerated Linear search.
 *
 * Accelerated Linear (AL) search by Musser & Nishanov.
 *
 */
template <typename PatIter, typename CorpusIter>
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
    pattern_difference_type pattern_length;

private:
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

public:
    accelerated_linear(PatIter pat_first, PatIter pat_last)
      : pat_first(pat_first), pat_last(pat_last),
        pattern_length(std::distance(pat_first, pat_last))
    {
        if (pattern_length > 0)
        {
            next_.reserve(pattern_length);
            compute_next();
        }
    }

    std::pair<CorpusIter, CorpusIter>
    operator()(CorpusIter corpus_first, CorpusIter corpus_last) const
    {
        BOOST_ASSERT(std::distance(pat_first, pat_last) == pattern_length);
        BOOST_ASSERT(pattern_length == distance(next_));

        using std::find;
        using std::make_pair;

        if (pat_first == pat_last)
            return make_pair(corpus_first, corpus_first);

        if (distance(next_) == 1)
        {
            CorpusIter const result = find(corpus_first, corpus_last, *pat_first);
            return result == corpus_last ? make_pair(corpus_last, corpus_last)
            : make_pair(result, boost::next(result));
        }

        PatIter p1 = pat_first;
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
                    return make_pair(hold, boost::next(hold, pattern_length));
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
                    corpus_first++;
                    p++;
                    j++;
                    if (p == pat_last)
                    {
                        CorpusIter succesor = hold;
                        std::advance(succesor, distance(next_));
                        while (succesor != corpus_first)
                            ++succesor, ++hold; // TODO: Change to for loop?
                        return make_pair(hold, boost::next(hold, pattern_length));
                    }
                    if (corpus_first == corpus_last)
                        return make_pair(corpus_last, corpus_last);
                }
            }
        }
        return make_pair(corpus_last, corpus_last);
    }

    template <typename Range>
    std::pair<CorpusIter, CorpusIter> operator()(const Range &corpus) const
    {
        return (*this)(boost::begin(corpus), boost::end(corpus));
    }
};

}}

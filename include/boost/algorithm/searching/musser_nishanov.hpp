/* 
   Copyright (c) David R. Musser & Gor V. Nishanov 1997.
   Copyright (c) Jeremy W. Murphy 2024.

   Distributed under the Boost Software License, Version 1.0. (See accompanying
   file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

    For more information, see http://www.boost.org
*/

#ifndef BOOST_ALGORITHM_SEARCH_MUSSER_NISHANOV_HPP
#define BOOST_ALGORITHM_SEARCH_MUSSER_NISHANOV_HPP

#include <boost/config.hpp>
#include <boost/algorithm/searching/accelerated_linear.hpp>
#include <boost/algorithm/searching/hashed_accelerated_linear.hpp>
#include <boost/algorithm/searching/detail/mn_traits.hpp>
#include <boost/mp11/function.hpp>
#include <boost/mp11/integral.hpp>
#include <boost/next_prior.hpp>
#include <boost/utility/enable_if.hpp>
#include <boost/variant2/variant.hpp>

#include <iterator>
#include <type_traits>
#include <utility>


namespace boost { namespace algorithm {

namespace detail {

template <typename CorpusIter, typename Trait>
using hashable = typename mp11::mp_and<
        std::is_base_of<std::random_access_iterator_tag,
                        typename std::iterator_traits<CorpusIter>::iterator_category>,
        mp11::mp_to_bool<std::integral_constant<decltype(Trait::suffix_size), Trait::suffix_size>>
    >::type;
}

/**
 * One class, two identities based on corpus iterator and the suffix size trait.
 */
template <typename PatIter, typename CorpusIter = PatIter,
        typename Trait = search_trait<typename std::iterator_traits<PatIter>::value_type>,
        typename Enable = void>
class musser_nishanov;


/**
 * Musser-Nishanov Accelerated Linear search algorithm.
 */
template <typename PatIter, typename CorpusIter, typename Trait>
class musser_nishanov<PatIter, CorpusIter, Trait,
                      typename disable_if<detail::hashable<CorpusIter, Trait>>::type>
{
    boost::algorithm::accelerated_linear<PatIter, CorpusIter> searcher;

public:
    musser_nishanov(PatIter pat_first, PatIter pat_last)
        : searcher(pat_first, pat_last) {}

    std::pair<CorpusIter, CorpusIter>
    operator()(CorpusIter corpus_first, CorpusIter corpus_last) const
    {
        return searcher(corpus_first, corpus_last);
    }
};


/**
 * Musser-Nishanov Hashed Accelerated Linear search algorithm.
 */
template <typename PatIter, typename CorpusIter, typename Trait>
class musser_nishanov<PatIter, CorpusIter, Trait, 
typename enable_if<detail::hashable<CorpusIter, Trait>>::type>
{
    using HAL = boost::algorithm::hashed_accelerated_linear<PatIter, CorpusIter, Trait>;
    using AL = boost::algorithm::accelerated_linear<PatIter, CorpusIter>;
    using SearcherVariant = boost::variant2::variant<HAL, AL>;

    SearcherVariant searcher;

    SearcherVariant
    select_searcher(PatIter first, PatIter last) const
    {
        auto pattern_length = std::distance(first, last);
        auto fall_back = pattern_length < Trait::suffix_size || pattern_length == 1;
        return fall_back ? SearcherVariant{AL(first, last)}
                         : SearcherVariant{HAL(first, last)};
    }

    // A generic lambda written out by hand.
    class SearcherVisitor
    {
        CorpusIter m_first, m_last;

    public:
        constexpr
        SearcherVisitor(CorpusIter first, CorpusIter last)
            : m_first{first}
            , m_last{last}
        {}

        template<typename Searcher>
        constexpr
        std::pair<CorpusIter, CorpusIter> operator()(Searcher &&s) const
        {
            return s(m_first, m_last);
        }
    };

public:
    musser_nishanov(PatIter pat_first, PatIter pat_last)
        : searcher{select_searcher(pat_first, pat_last)}
    {}

    std::pair<CorpusIter, CorpusIter>
    operator()(CorpusIter first, CorpusIter last) const
    {
        return boost::variant2::visit(SearcherVisitor{first, last}, searcher);
    }

    template <typename Range>
    std::pair<CorpusIter, CorpusIter>
    operator()(Range const &corpus) const
    {
        return (*this)(boost::begin(corpus), boost::end(corpus));
    }
};


template <typename PatIter, typename CorpusIter>
std::pair<CorpusIter, CorpusIter>
musser_nishanov_search(CorpusIter corpus_first, CorpusIter corpus_last,
                       PatIter pat_first, PatIter pat_last)
{
    musser_nishanov<PatIter, CorpusIter> mn(pat_first, pat_last);
    return mn(corpus_first, corpus_last);
}


template <typename PatternRange, typename CorpusIter>
std::pair<CorpusIter, CorpusIter>
musser_nishanov_search(CorpusIter corpus_first, CorpusIter corpus_last,
                       const PatternRange &pattern)
{
    typedef typename boost::range_iterator<const PatternRange>::type pattern_iterator;
    musser_nishanov<pattern_iterator, CorpusIter> mn(boost::begin(pattern), boost::end(pattern));
    return mn(corpus_first, corpus_last);
}


template <typename patIter, typename CorpusRange>
typename boost::disable_if_c<
    std::is_same<CorpusRange, patIter>::value,
    std::pair<typename boost::range_iterator<CorpusRange>::type,
              typename boost::range_iterator<CorpusRange>::type> >
::type
musser_nishanov_search(CorpusRange &corpus, patIter pat_first, patIter pat_last)
{
    typedef typename boost::range_iterator<const CorpusRange>::type corpus_iterator;
    musser_nishanov<patIter, corpus_iterator> mn(pat_first, pat_last);
    return mn(boost::begin(corpus), boost::end(corpus));
}


template <typename PatternRange, typename CorpusRange>
std::pair<typename boost::range_iterator<CorpusRange>::type,
          typename boost::range_iterator<CorpusRange>::type>
musser_nishanov_search(CorpusRange &corpus, const PatternRange &pattern)
{
    typedef typename boost::range_iterator<const PatternRange>::type pattern_iterator;
    typedef typename boost::range_iterator<const CorpusRange>::type corpus_iterator;
    musser_nishanov<pattern_iterator, corpus_iterator> mn(boost::begin(pattern), boost::end(pattern));
    return mn(boost::begin(corpus), boost::end(corpus));
}


//  Creator functions -- take a pattern range, return an object
template <typename Range>
musser_nishanov<typename boost::range_iterator<const Range>::type>
make_musser_nishanov(const Range &r) {
    typedef typename boost::range_iterator<const Range>::type pattern_iterator;
    return musser_nishanov<pattern_iterator>(boost::begin(r), boost::end(r));
}


// This overload permits specification of the corpus iterator type.
template <typename PatternRange, typename CorpusRange>
musser_nishanov<typename boost::range_iterator<const PatternRange>::type,
                typename boost::range_iterator<const CorpusRange>::type>
make_musser_nishanov(const PatternRange &r, const CorpusRange &) {
    typedef typename boost::range_iterator<const PatternRange>::type pattern_iterator;
    typedef typename boost::range_iterator<const CorpusRange>::type corpus_iterator;
    return musser_nishanov<pattern_iterator, corpus_iterator>(boost::begin(r), boost::end(r));
}


// This overload permits specification of corpus iterator and search trait class.
template <typename PatternRange, typename CorpusRange, typename Trait>
musser_nishanov<typename boost::range_iterator<const PatternRange>::type,
                typename boost::range_iterator<const CorpusRange>::type>
make_musser_nishanov(const PatternRange &r, const CorpusRange &) {
    typedef typename boost::range_iterator<const PatternRange>::type pattern_iterator;
    typedef typename boost::range_iterator<const CorpusRange>::type corpus_iterator;
    return musser_nishanov<pattern_iterator, corpus_iterator, Trait>(boost::begin(r), boost::end(r));
}

}} // namespace boost::algorithm

#endif

#ifndef BOOST_ALGORITHM_SEARCH_MUSSER_NISHANOV_HPP
#define BOOST_ALGORITHM_SEARCH_MUSSER_NISHANOV_HPP

#include <boost/algorithm/searching/detail/musser_nishanov_AL.hpp>
#include <boost/algorithm/searching/detail/musser_nishanov_HAL.hpp>
#include <boost/algorithm/searching/detail/mn_traits.hpp>
#include <boost/array.hpp>
#include <boost/function.hpp>
#include <boost/type_traits/is_same.hpp>
#include <boost/utility/enable_if.hpp>

// #include <functional>
#include <iterator>
#include <utility>
#include <vector>
/*
 * One class, one purpose, one fallback for bidirectional iterators.
 */
namespace boost { namespace algorithm {

template <typename PatIter, typename CorpusIter = PatIter, typename Trait = search_trait<typename std::iterator_traits<PatIter>::value_type> >
class musser_nishanov
{
    typedef typename std::iterator_traits<PatIter>::difference_type pattern_difference_type;
    typedef typename std::iterator_traits<CorpusIter>::difference_type corpus_difference_type;

    PatIter pat_first, pat_last;
    std::vector<corpus_difference_type> next;
    boost::array<corpus_difference_type, Trait::hash_range_max> skip;
    pattern_difference_type k_pattern_length;
    corpus_difference_type mismatch_shift;
    boost::function<std::pair<CorpusIter, CorpusIter>(CorpusIter, CorpusIter)> search;
    
    std::pair<CorpusIter, CorpusIter> HAL(CorpusIter corpus_first, CorpusIter corpus_last) {}
    
public:
    template <typename I>
    musser_nishanov(I pat_first, I pat_last) : pat_first(pat_first), pat_last(pat_last)
    {
    }
    
    template <typename I>
    typename enable_if<is_same<typename std::iterator_traits<I>::iterator_category, std::random_access_iterator_tag>, std::pair<I, I> >::type
    operator()(I corpus_first, I corpus_last)
    {
    }
    
    template <typename I>
    typename disable_if<is_same<typename std::iterator_traits<I>::iterator_category, std::random_access_iterator_tag>, std::pair<I, I> >::type
    operator()(I corpus_first, I corpus_last)
    {
    }
    
};

}} // namespace boost::algorithm

#endif

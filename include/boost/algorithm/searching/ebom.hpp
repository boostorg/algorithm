/*
  Copyright (c) Alexander Zaitsev <zamazan4ik@gmail.com>, 2016

  Distributed under the Boost Software License, Version 1.0. (See
  accompanying file LICENSE_1_0.txt or copy at
  http://www.boost.org/LICENSE_1_0.txt)

  See http://www.boost.org/ for latest version.
*/

#ifndef BOOST_ALGORITHM_EBOM_HPP
#define BOOST_ALGORITHM_EBOM_HPP

#include <vector>
#include <algorithm>

#define SIGMA       256				//constant alphabet size
#define UNDEFINED   -1


namespace boost { namespace algorithm {


template <typename corpusIter, typename patIter>
std::vector<corpusIter> ebom_search(corpusIter corp_begin, corpusIter corp_end,
                                    patIter pat_begin, patIter pat_end)
{
    int m = std::distance(pat_begin, pat_end),
        n = std::distance(corp_begin, corp_end);
    std::vector<std::vector<int>> FT(SIGMA, std::vector<int>(SIGMA));
    std::vector<int> S(m);
    std::vector<std::vector<int>> trans(m + 1, std::vector<int>(SIGMA, UNDEFINED));
    int i, j, p, q;
    int iMinus1, mMinus1;
    unsigned char c;
    //BEGIN_PREPROCESSING
    S[m] = m + 1;
    for (i = m; i > 0; --i)
    {
        iMinus1 = i - 1;
        c = pat_begin[iMinus1];
        trans[i][c] = iMinus1;
        p = S[i];
        while (p <= m && ((q = trans[p][c]) ==  UNDEFINED))
        {
            trans[p][c] = iMinus1;
            p = S[p];
        }
        S[iMinus1] = (p == m + 1 ? m : q);
    }

    /* Construct the FirstTransition table */
    for(size_t i = 0; i < SIGMA; ++i)
    {
        q = trans[m][i];
        for(size_t j = 0; j < SIGMA; ++j)
        {
            if (q >= 0)
                FT[i][j] = trans[q][j];
            else
                FT[i][j] = UNDEFINED;
        }
    }
    //END_PREPROCESSING

    //BEGIN_SEARCHING
    for(size_t i = 0; i < m; ++i)
        corp_begin[n+i] = pat_begin[i];


    std::vector<corpusIter> result;
    if( std::equal(pat_begin, pat_end, corp_begin))
        result.push_back(corp_begin);

    j = m;
    mMinus1 = m - 1;
    while (j < n)
    {
        while ( (FT[corp_begin[j]][corp_begin[j - 1]]) == UNDEFINED )
        {
            j += mMinus1;
        }
        i = j - 2;
        p = FT[corp_begin[j]][corp_begin[j - 1]];
        while ((p = trans[p][corp_begin[i]]) != UNDEFINED )
        {
            i--;
        }
        if (i < j - mMinus1 && j < n)
        {
            result.push_back(corp_begin + j - mMinus1);
            i++;
        }
        j = i + m;
    }
    //END_SEARCHING

    return result;
}


template <typename corpusRange, typename patIter>
std::vector<typename boost::range_iterator<corpusRange>::type>
ebom_search(const corpusRange& corp_range, patIter pat_begin, patIter pat_end)
{
    return ebom_search(boost::begin(corp_range), boost::end(corp_range), pat_begin, pat_end);
}


template <typename corpusIter, typename patRange>
std::vector<corpusIter> ebom_search(corpusIter corp_begin, corpusIter corp_end,
                                    const patRange& pat_range)
{
    return ebom_search(corp_begin, corp_end, boost::begin(pat_range), boost::end(pat_range));
}


template <typename corpusRange, typename patRange>
std::vector<typename boost::range_iterator<corpusRange>::type>
ebom_search(const corpusRange& corp_range, const patRange& pat_range)
{
    return ebom_search(boost::begin(corp_range), boost::end(corp_range),
                       boost::begin(pat_range), boost::end(pat_range));

}

}}


#endif //BOOST_ALGORITHM_EBOM_HPP

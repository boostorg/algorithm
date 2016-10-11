#ifndef MANACKER_H
#define MANACKER_H

#include <string>
#include <vector>

#include <boost/range/begin.hpp>
#include <boost/range/end.hpp>

namespace boost { namespace algorithm {

template<typename RAIterator>
std::vector<std::pair<RAIterator, RAIterator>> manacker(RAIterator begin, RAIterator end)
{
    size_t length = std::distance(begin, end);
    std::vector<int> ansPalN2(length), ansPal2(length);

    int leftBorder = 0, rightBorder = -1, tempMirror;//start digits for algortihm
    for (int i = 0; i < length; ++i)
    {
        tempMirror = (i > rightBorder ? 0 : std::min(ansPalN2[leftBorder + rightBorder - i], rightBorder - i)) +
                     1;//find mirror of current index
        while (i + tempMirror < length && i - tempMirror >= 0 &&
               begin[i - tempMirror] == begin[i + tempMirror])//increase our index
        {
            ++tempMirror;
        }
        ansPalN2[i] = --tempMirror;
        if (i + tempMirror > rightBorder)//try to increase our right border of palindrom
        {
            leftBorder = i - tempMirror;
            rightBorder = i + tempMirror;
        }
    }

    //---------------------------------------------------------------

    leftBorder = 0, rightBorder = -1, tempMirror = 0;
    for (int i = 0; i < length; ++i)
    {
        tempMirror =
                (i > rightBorder ? 0 : std::min(ansPal2[leftBorder + rightBorder - i + 1], rightBorder - i + 1)) +
                1;
        while (i + tempMirror - 1 < length && i - tempMirror >= 0 &&
               begin[i - tempMirror] == begin[i + tempMirror - 1])
        {
            ++tempMirror;
        }
        ansPal2[i] = --tempMirror;
        if (i + tempMirror - 1 > rightBorder)
        {
            leftBorder = i - tempMirror;
            rightBorder = i + tempMirror - 1;
        }
    }

    //------------------------------------------------------
    std::cout << std::endl;

    std::vector<std::pair<RAIterator, RAIterator>> result;
    for(size_t i = 0; i < length; ++i)
    {
        result.push_back({begin + i - ansPalN2[i], begin + i + ansPalN2[i] + 1});
    }
    for(size_t i = 0; i < length; ++i)
    {
        if(ansPal2[i] != 0)
            result.push_back({begin + i - ansPal2[i], begin + i + ansPal2[i]});
    }
    return result;
}

template<typename Range>
auto manacker(Range range)
{
    return manacker(boost::begin(range), boost::end(range));
};

}}

//Find palindroms like 2*N+1


//Find palindroms like 2*N
//See PalN2.
//P.S. About magic numbers : you can read about this in the description of the algorithm of Manacker.
//These numbers need for finding palindroms like 2*N because not allowed to find centre of these palindrom

#endif // MANACKER_H

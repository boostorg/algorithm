/*
  Copyright (c) Alexander Zaitsev <zamazan4ik@gmail.com>, 2016
  Distributed under the Boost Software License, Version 1.0. (See
  accompanying file LICENSE_1_0.txt or copy at
  http://www.boost.org/LICENSE_1_0.txt)
  See http://www.boost.org/ for latest version.
*/

/// \file  manacker.hpp
/// \brief Finds all palindromes in a sequence.
/// \author Alexander Zaitsev

#ifndef BOOST_ALGORITHM_MANACKER_HPP
#define BOOST_ALGORITHM_MANACKER_HPP

#include <string>
#include <vector>
#include <utility>
#include <algorithm>

#include <boost/range/begin.hpp>
#include <boost/range/end.hpp>

namespace boost { namespace algorithm {



template <typename Iter, typename BinaryPredicate = std::equal_to<typename std::iterator_traits<Iter>::value_type>>
class manacker_class
{
public:
    manacker_class(Iter begin, Iter end, BinaryPredicate p = BinaryPredicate())
            : begin_(begin), end_(end), p_(p)
    {
        length_ = std::distance(begin_, end_);
        answer_.resize(length_);
    }

    template<typename Range>
    manacker_class(const Range& r, BinaryPredicate p = BinaryPredicate())
            : manacker_class(boost::begin(r), boost::end(r), p)
    {
    }

    std::pair<Iter, Iter> next()
    {
        //if cannot find palindrome, returns {corp_end, corp_end}
        std::pair<Iter, Iter> ans;
        switch (flag_)
        {
            case 0:
                ans = calcOdd(); break;
            case 1:
                ans = calcEven(); break;
            default:
                return std::pair<Iter, Iter>(end_, end_);
        }

        ++i;
        if(i == length_)
        {
            restoreToDefault();
        }

        return ans;
    }
private:
    void restoreToDefault()
    {
        ++flag_;
        leftBorder = 0, rightBorder = -1, tempMirror = 0, i = 0;
        std::fill(answer_.begin(), answer_.end(), 0);
    }


    std::pair<Iter, Iter> calcOdd()
    {
        tempMirror = (i > rightBorder ? 0 : std::min(answer_[leftBorder + rightBorder - i],
                                                     rightBorder - i)) + 1;//find mirror of current index
        while (i + tempMirror < length_ && i - tempMirror >= 0 &&
               p_(begin_[i - tempMirror], begin_[i + tempMirror]))//increase our index
        {
            ++tempMirror;
        }
        answer_[i] = --tempMirror;
        if (i + tempMirror > rightBorder)//try to increase our right border of palindrom
        {
            leftBorder = i - tempMirror;
            rightBorder = i + tempMirror;
        }
        return std::pair<Iter, Iter>(begin_ + i - answer_[i], begin_ + i + answer_[i] + 1);
    }

    std::pair<Iter, Iter> calcEven()
    {
        for (; i < length_; ++i)
        {
            tempMirror =
                    (i > rightBorder ? 0 : std::min(answer_[leftBorder + rightBorder - i + 1],
                                                    rightBorder - i + 1)) + 1;
            while (i + tempMirror - 1 < length_ && i - tempMirror >= 0 &&
                   p_(begin_[i - tempMirror], begin_[i + tempMirror - 1]))
            {
                ++tempMirror;
            }
            answer_[i] = --tempMirror;
            if (i + tempMirror - 1 > rightBorder)
            {
                leftBorder = i - tempMirror;
                rightBorder = i + tempMirror - 1;
            }

            if(answer_[i] != 0)
                break;
        }
        if(i == length_)
            return std::pair<Iter, Iter>(end_, end_);
        return std::pair<Iter, Iter>(begin_ + i - answer_[i], begin_ + i + answer_[i]);
    }
private:
    Iter begin_, end_;
    BinaryPredicate p_;
    int length_, i = 0;
    int leftBorder = 0, rightBorder = -1, tempMirror = 0, flag_ = 0;

    std::vector<int> answer_;
};


}}

#endif // BOOST_ALGORITHM_
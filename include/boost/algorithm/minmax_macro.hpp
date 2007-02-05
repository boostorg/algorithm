///////////////////////////////////////////////////////////////////////////////
// minmax_macro.hpp header file
//
// Copyright 2006 Eric Niebler.
// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//
// Credits:
//  Scott Meyers        - for carefully detailing the shortcomings of
//                        std::min and std::max in his article "Min, max and more"
//  Andrei Alexandrescu - for the suggestion that ?: operator couls be useful
//                        in solving Scott's long-standing min/max challenge.

#ifndef BOOST_MIN

#include <boost/config.hpp>
#include <boost/detail/workaround.hpp>

#if BOOST_WORKAROUND(__COMO_VERSION__, BOOST_TESTED_AT(4245))

# include <boost/algorithm/detail/minmax_macro_comeau.hpp>

#elif BOOST_WORKAROUND(BOOST_MSVC, BOOST_TESTED_AT(1400))

# include <boost/algorithm/detail/minmax_macro_msvc.hpp>

#else

namespace boost { namespace minmax_macro_detail_
{

    ///////////////////////////////////////////////////////////////////////////////
    // max_impl
    template<typename Left, typename Right>
    struct max_impl
    {
        max_impl(Left &left, Right &right)
          : left_(left)
          , right_(right)
        {}

        template<typename Rvalue>
        operator Rvalue ()
        {
            return this->left_ < this->right_ ? this->right_ : this->left_;
        }

        template<typename Lvalue>
        operator Lvalue &() const
        {
            return this->left_ < this->right_ ? this->right_ : this->left_;
        }

        #if BOOST_WORKAROUND(__GNUC__, BOOST_TESTED_AT(3))
        operator max_impl & () { return *this; }
        #endif

    private:
        Left &left_;
        Right &right_;
    };

    ///////////////////////////////////////////////////////////////////////////////
    // max_fun
    template<typename Left, typename Right>
    max_impl<Left, Right>
    max_fun(Left &left, Right &right)
    {
        return max_impl<Left, Right>(left, right);
    }

    template<typename Left, typename Right>
    max_impl<Left const, Right>
    max_fun(Left const &left, Right &right)
    {
        return max_impl<Left const, Right>(left, right);
    }

    template<typename Left, typename Right>
    max_impl<Left, Right const>
    max_fun(Left &left, Right const &right)
    {
        return max_impl<Left, Right const>(left, right);
    }

    template<typename Left, typename Right>
    max_impl<Left const, Right const>
    max_fun(Left const &left, Right const &right)
    {
        return max_impl<Left const, Right const>(left, right);
    }

#define BOOST_MAX(a,b)\
    (true? boost::minmax_macro_detail_::max_fun((a), (b)) : (true? (a) : (b)))

    ///////////////////////////////////////////////////////////////////////////////
    // min_impl
    template<typename Left, typename Right>
    struct min_impl
    {
        min_impl(Left &left, Right &right)
          : left_(left)
          , right_(right)
        {}

        template<typename Rvalue>
        operator Rvalue ()
        {
            return this->left_ < this->right_ ? this->left_ : this->right_;
        }

        template<typename Lvalue>
        operator Lvalue &() const
        {
            return this->left_ < this->right_ ? this->left_ : this->right_;
        }

        #if BOOST_WORKAROUND(__GNUC__, BOOST_TESTED_AT(3))
        operator min_impl & () { return *this; }
        #endif

    private:
        Left &left_;
        Right &right_;
    };

    ///////////////////////////////////////////////////////////////////////////////
    // min_fun
    template<typename Left, typename Right>
    min_impl<Left, Right>
    min_fun(Left &left, Right &right)
    {
        return min_impl<Left, Right>(left, right);
    }

    template<typename Left, typename Right>
    min_impl<Left const, Right>
    min_fun(Left const &left, Right &right)
    {
        return min_impl<Left const, Right>(left, right);
    }

    template<typename Left, typename Right>
    min_impl<Left, Right const>
    min_fun(Left &left, Right const &right)
    {
        return min_impl<Left, Right const>(left, right);
    }

    template<typename Left, typename Right>
    min_impl<Left const, Right const>
    min_fun(Left const &left, Right const &right)
    {
        return min_impl<Left const, Right const>(left, right);
    }

#define BOOST_MIN(a,b)\
    (true? boost::minmax_macro_detail_::min_fun((a), (b)) : (true? (a) : (b)))

}}

#endif

#endif

///////////////////////////////////////////////////////////////////////////////
// minmax_macro_comeau.hpp header file
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

#include <boost/mpl/or.hpp>
#include <boost/mpl/eval_if.hpp>
#include <boost/type_traits/is_array.hpp>
#include <boost/type_traits/is_abstract.hpp>
#include <boost/type_traits/remove_const.hpp>

namespace boost { namespace minmax_macro_detail_
{
    template<typename Type>
    Type *encode_type(Type &) { return 0; }

    template<typename Type>
    Type const *encode_type(Type const &) { return 0; }

    ///////////////////////////////////////////////////////////////////////////////
    // max_impl
    template<typename Ret, typename Left, typename Right>
    struct max_impl
    {
        max_impl(Left &left, Right &right)
          : left_(left)
          , right_(right)
        {}

        struct private_type_ { typedef private_type_ type; };

        // can't ever return an array or an abstract type by value
        typedef BOOST_DEDUCED_TYPENAME boost::mpl::eval_if<
            boost::mpl::or_<boost::is_abstract<Ret>, boost::is_array<Ret> >
          , private_type_
          , boost::remove_const<Ret>
        >::type value_type;

        operator value_type()
        {
            return this->left_ < this->right_ ? this->right_ : this->left_;
        }

        operator Ret &() const
        {
            return this->left_ < this->right_ ? this->right_ : this->left_;
        }

    private:
        Left &left_;
        Right &right_;
    };


    ///////////////////////////////////////////////////////////////////////////////
    // max_fun
    template<typename Left, typename Right, typename Ret>
    max_impl<Ret, Left, Right>
    max_fun(Left &left, Right &right, Ret *)
    {
        return max_impl<Ret, Left, Right>(left, right);
    }

    template<typename Left, typename Right, typename Ret>
    max_impl<Ret, Left const, Right>
    max_fun(Left const &left, Right &right, Ret *)
    {
        return max_impl<Ret, Left const, Right>(left, right);
    }

    template<typename Left, typename Right, typename Ret>
    max_impl<Ret, Left, Right const>
    max_fun(Left &left, Right const &right, Ret *)
    {
        return max_impl<Ret, Left, Right const>(left, right);
    }

    template<typename Left, typename Right, typename Ret>
    max_impl<Ret, Left const, Right const>
    max_fun(Left const &left, Right const &right, Ret *)
    {
        return max_impl<Ret, Left const, Right const>(left, right);
    }

#define BOOST_MAX(a,b)\
    (true\
      ? boost::minmax_macro_detail_::max_fun((a), (b), \
            (true? 0 : boost::minmax_macro_detail_::encode_type(true? (a) : (b))))\
      : (true? (a) : (b)))


    ///////////////////////////////////////////////////////////////////////////////
    // min_impl
    template<typename Ret, typename Left, typename Right>
    struct min_impl
    {
        min_impl(Left &left, Right &right)
          : left_(left)
          , right_(right)
        {}

        struct private_type_ { typedef private_type_ type; };

        // can't ever return an array or an abstract type by value
        typedef BOOST_DEDUCED_TYPENAME boost::mpl::eval_if<
            boost::mpl::or_<boost::is_abstract<Ret>, boost::is_array<Ret> >
          , private_type_
          , boost::remove_const<Ret>
        >::type value_type;

        operator value_type()
        {
            return this->left_ < this->right_ ? this->left_ : this->right_;
        }

        operator Ret &() const
        {
            return this->left_ < this->right_ ? this->left_ : this->right_;
        }

    private:
        Left &left_;
        Right &right_;
    };


    ///////////////////////////////////////////////////////////////////////////////
    // min_fun
    template<typename Left, typename Right, typename Ret>
    min_impl<Ret, Left, Right>
    min_fun(Left &left, Right &right, Ret *)
    {
        return min_impl<Ret, Left, Right>(left, right);
    }

    template<typename Left, typename Right, typename Ret>
    min_impl<Ret, Left const, Right>
    min_fun(Left const &left, Right &right, Ret *)
    {
        return min_impl<Ret, Left const, Right>(left, right);
    }

    template<typename Left, typename Right, typename Ret>
    min_impl<Ret, Left, Right const>
    min_fun(Left &left, Right const &right, Ret *)
    {
        return min_impl<Ret, Left, Right const>(left, right);
    }

    template<typename Left, typename Right, typename Ret>
    min_impl<Ret, Left const, Right const>
    min_fun(Left const &left, Right const &right, Ret *)
    {
        return min_impl<Ret, Left const, Right const>(left, right);
    }

#define BOOST_MIN(a,b)\
    (true\
      ? boost::minmax_macro_detail_::min_fun((a), (b), \
            (true? 0 : boost::minmax_macro_detail_::encode_type(true? (a) : (b))))\
      : (true? (a) : (b)))

}}

#endif

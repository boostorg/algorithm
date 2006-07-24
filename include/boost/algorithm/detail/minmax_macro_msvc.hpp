///////////////////////////////////////////////////////////////////////////////
// minmax_macro_msvc.hpp header file
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

#include <boost/mpl/if.hpp>
#include <boost/mpl/bool.hpp>
#include <boost/mpl/logical.hpp>
#include <boost/type_traits/is_array.hpp>
#include <boost/type_traits/is_abstract.hpp>

namespace boost { namespace minmax_macro_detail_
{

    ///////////////////////////////////////////////////////////////////////////////
    // Define some utilities for assessing the properties of expressions
    //
    typedef char yes_type;
    typedef char (&no_type)[2];
    yes_type is_true(boost::mpl::true_ *);
    no_type is_true(boost::mpl::false_ *);

// Extracts the desired property from the expression without evaluating it
#define BOOST_MINMAX_PROTECT(expr)                                                             \
    (static_cast<boost::mpl::bool_<1 == sizeof(boost::minmax_macro_detail_::is_true(expr))> *>(0))

    template<typename Bool1, typename Bool2>
    inline boost::mpl::and_<Bool1, Bool2> *and_(Bool1 *, Bool2 *) { return 0; }

    template<typename Bool>
    inline boost::mpl::not_<Bool> *not_(Bool *) { return 0; }

    template<typename T>
    inline boost::mpl::false_ *is_rvalue_(T &, int) { return 0; }

    template<typename T>
    inline boost::mpl::true_ *is_rvalue_(T const &, ...) { return 0; }

    template<typename T>
    inline boost::is_array<T> *is_array_(T const &) { return 0; }

    ///////////////////////////////////////////////////////////////////////////////
    // Detect at compile-time whether an expression yields an rvalue or
    // an lvalue. This is rather non-standard, but MSVC seems to like it.
    ///////////////////////////////////////////////////////////////////////////////

    ///////////////////////////////////////////////////////////////////////////////
    // rvalue_probe
    //
    template<typename T>
    struct rvalue_probe
    {
        struct private_type_ {};
        // can't ever return an array by value
        typedef typename boost::mpl::if_<
            boost::mpl::or_<boost::is_abstract<T>, boost::is_array<T> >, private_type_, T
        >::type value_type;
        operator value_type();
        operator T &() const;
    };

    template<typename T>
    rvalue_probe<T> const make_probe(T const &t);

# define BOOST_MINMAX_IS_RVALUE(T)                                                              \
    BOOST_MINMAX_PROTECT(                                                                       \
        boost::minmax_macro_detail_::and_(                                                      \
            boost::minmax_macro_detail_::not_(boost::minmax_macro_detail_::is_array_(T))        \
          , boost::minmax_macro_detail_::is_rvalue_(                                            \
                (true ? boost::minmax_macro_detail_::make_probe(T) : (T)), 0)))

    template<typename Type>
    Type *encode_type(Type &) { return 0; }

    template<typename Type>
    Type const *encode_type(Type const &) { return 0; }

    ///////////////////////////////////////////////////////////////////////////////
    // max_fun
    template<typename Left, typename Right, typename Ret, typename IsRvalue>
    typename mpl::if_<IsRvalue, Ret, Ret &>::type
    max_fun(Left &left, Right &right, Ret *, IsRvalue *)
    {
        return left < right ? right : left;
    }

    template<typename Left, typename Right, typename Ret, typename IsRvalue>
    typename mpl::if_<IsRvalue, Ret, Ret &>::type
    max_fun(Left const &left, Right &right, Ret *, IsRvalue *)
    {
        return left < right ? right : left;
    }

    template<typename Left, typename Right, typename Ret, typename IsRvalue>
    typename mpl::if_<IsRvalue, Ret, Ret &>::type
    max_fun(Left &left, Right const &right, Ret *, IsRvalue *)
    {
        return left < right ? right : left;
    }

    template<typename Left, typename Right, typename Ret, typename IsRvalue>
    typename mpl::if_<IsRvalue, Ret, Ret &>::type
    max_fun(Left const &left, Right const &right, Ret *, IsRvalue *)
    {
        return left < right ? right : left;
    }

#define BOOST_MAX(a,b)\
    boost::minmax_macro_detail_::max_fun(\
        (a) \
      , (b) \
      , (true? 0 : boost::minmax_macro_detail_::encode_type(true? (a) : (b))) \
      , BOOST_MINMAX_IS_RVALUE(true? (a) : (b)))


    ///////////////////////////////////////////////////////////////////////////////
    // min_fun
    template<typename Left, typename Right, typename Ret, typename IsRvalue>
    typename mpl::if_<IsRvalue, Ret, Ret &>::type
    min_fun(Left &left, Right &right, Ret *, IsRvalue *)
    {
        return left < right ? left : right;
    }

    template<typename Left, typename Right, typename Ret, typename IsRvalue>
    typename mpl::if_<IsRvalue, Ret, Ret &>::type
    min_fun(Left const &left, Right &right, Ret *, IsRvalue *)
    {
        return left < right ? left : right;
    }

    template<typename Left, typename Right, typename Ret, typename IsRvalue>
    typename mpl::if_<IsRvalue, Ret, Ret &>::type
    min_fun(Left &left, Right const &right, Ret *, IsRvalue *)
    {
        return left < right ? left : right;
    }

    template<typename Left, typename Right, typename Ret, typename IsRvalue>
    typename mpl::if_<IsRvalue, Ret, Ret &>::type
    min_fun(Left const &left, Right const &right, Ret *, IsRvalue *)
    {
        return left < right ? left : right;
    }


#define BOOST_MIN(a,b)\
    boost::minmax_macro_detail_::min_fun(\
        (a) \
      , (b) \
      , (true? 0 : boost::minmax_macro_detail_::encode_type(true? (a) : (b))) \
      , BOOST_MINMAX_IS_RVALUE(true? (a) : (b)))

}}

#endif

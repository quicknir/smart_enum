
//          Copyright Jarda Flieger 2016.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef SMART_ENUM_HEADER_INCLUDED
#define SMART_ENUM_HEADER_INCLUDED

#include <boost/config.hpp>
#include <boost/preprocessor/arithmetic/dec.hpp>
#include <boost/preprocessor/control/expr_iif.hpp>
#include <boost/preprocessor/logical/not.hpp>
#include <boost/preprocessor/punctuation/remove_parens.hpp>
#include <boost/preprocessor/repetition/enum.hpp>
#include <boost/preprocessor/tuple/size.hpp>

#define SMART_ENUM(NAMESPACE_OR_NAME, ...) \
    SMART_ENUM_IMPL(, NAMESPACE_OR_NAME, __VA_ARGS__)

#define SMART_ENUM_CLASS(NAMESPACE_OR_NAME, ...) \
    SMART_ENUM_IMPL(class, NAMESPACE_OR_NAME, __VA_ARGS__)

#ifdef BOOST_MSVC
    #define SMART_ENUM_IMPL(CLASS, NAMESPACE_OR_NAME, ...) \
        BOOST_PP_CAT \
        ( \
            BOOST_PP_OVERLOAD(SMART_ENUM_IMPL_, __VA_ARGS__)(CLASS, NAMESPACE_OR_NAME, __VA_ARGS__), \
            BOOST_PP_EMPTY() \
        )
#else
    #define SMART_ENUM_IMPL(CLASS, NAMESPACE_OR_NAME, ...) \
        BOOST_PP_OVERLOAD(SMART_ENUM_IMPL_, __VA_ARGS__)(CLASS, NAMESPACE_OR_NAME, __VA_ARGS__)
#endif

#define SMART_ENUM_IMPL_1(CLASS, NAME, MEMBERS) \
    enum CLASS SMART_ENUM_IMPL_PROCESS_TUPLE(NAME_SIZE, SMART_ENUM_IMPL_ARG_TO_TUPLE(NAME)) \
    { \
        SMART_ENUM_IMPL_MEMBERS(MEMBERS) \
    };

#define SMART_ENUM_IMPL_2(CLASS, NAMESPACE, NAME, MEMBERS) \
    SMART_ENUM_IMPL_NAMESPACES(CLASS, SMART_ENUM_IMPL_ARG_TO_TUPLE(NAMESPACE), NAME, MEMBERS)

#define SMART_ENUM_IMPL_ARG_TO_TUPLE(ARG) \
    (BOOST_PP_REMOVE_PARENS(ARG))

#define SMART_ENUM_IMPL_MEMBER(NAME, ...) \
    BOOST_PP_EXPR_IIF \
    ( \
        BOOST_PP_NOT(BOOST_PP_IS_BEGIN_PARENS(__VA_ARGS__)), \
            SMART_ENUM_IMPL_MEMBER_1(__VA_ARGS__, _) \
    )

#define SMART_ENUM_IMPL_MEMBER_1(VALUE, ...) \
    = VALUE

#define SMART_ENUM_IMPL_MEMBERS(MEMBERS) \
    BOOST_PP_ENUM \
    ( \
        BOOST_PP_TUPLE_SIZE(MEMBERS), SMART_ENUM_IMPL_MEMBERS_1, MEMBERS \
    )

#define SMART_ENUM_IMPL_MEMBERS_1(_, INDEX, MEMBERS) \
    SMART_ENUM_IMPL_PROCESS_TUPLE \
    ( \
        MEMBER, SMART_ENUM_IMPL_ARG_TO_TUPLE(BOOST_PP_TUPLE_ELEM(INDEX, MEMBERS)) \
    )

#define SMART_ENUM_IMPL_NAME_SIZE(NAME, SIZE) \
    : SIZE

#define SMART_ENUM_IMPL_NAMESPACES(CLASS, NAMESPACES, NAME, MEMBERS) \
    SMART_ENUM_IMPL_REPEAT(NAMESPACES_START, NAMESPACES) \
        SMART_ENUM_IMPL_1(CLASS, NAME, MEMBERS) \
    SMART_ENUM_IMPL_REPEAT(NAMESPACES_END, NAMESPACES)

#define SMART_ENUM_IMPL_NAMESPACES_END(_, INDEX, NAMESPACES) \
    }

#define SMART_ENUM_IMPL_NAMESPACES_START(_, INDEX, NAMESPACES) \
    namespace BOOST_PP_TUPLE_ELEM(INDEX, NAMESPACES) {

#define SMART_ENUM_IMPL_PROCESS_TUPLE(MACRO, TUPLE) \
    BOOST_PP_TUPLE_ELEM(0, TUPLE) \
    BOOST_PP_IF \
    ( \
        BOOST_PP_DEC(BOOST_PP_TUPLE_SIZE(TUPLE)), \
            SMART_ENUM_IMPL_ ## MACRO, BOOST_PP_TUPLE_EAT(1) \
    ) \
    TUPLE

#define SMART_ENUM_IMPL_REPEAT(MACRO, TUPLE) \
    BOOST_PP_REPEAT(BOOST_PP_TUPLE_SIZE(TUPLE), SMART_ENUM_IMPL_ ## MACRO, TUPLE)

#endif

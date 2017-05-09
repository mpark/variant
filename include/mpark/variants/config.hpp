// MPark.Variant
//
// Copyright Michael Park, 2015-2017
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef MPARK_VARIANTS_CONFIG_HPP
#define MPARK_VARIANTS_CONFIG_HPP

#ifndef __has_builtin
#define __has_builtin(x) 0
#endif

#if __has_builtin(__builtin_addressof) || __GNUC__ >= 7
#define MPARK_BUILTIN_ADDRESSOF
#endif

#if __has_builtin(__type_pack_element)
#define MPARK_TYPE_PACK_ELEMENT
#endif

#ifdef __cpp_generic_lambdas
#define MPARK_GENERIC_LAMBDAS
#endif

#ifdef __cpp_lib_integer_sequence
#define MPARK_INTEGER_SEQUENCE
#endif

#ifdef __cpp_lib_transparent_operators
#define MPARK_TRANSPARENT_OPERATORS
#endif

#ifdef __cpp_return_type_deduction
#define MPARK_RETURN_TYPE_DEDUCTION
#endif

#ifdef __cpp_variable_templates
#define MPARK_VARIABLE_TEMPLATES
#endif

#if __cpp_constexpr >= 201304
#define MPARK_CPP14_CONSTEXPR
#endif

#endif  // MPARK_VARIANTS_CONFIG_HPP

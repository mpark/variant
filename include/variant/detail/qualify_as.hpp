// Copyright Michael Park 2015
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef VARIANT_DETAIL_QUALIFY_AS_HPP
#define VARIANT_DETAIL_QUALIFY_AS_HPP

#include <meta/meta.hpp>

namespace std {
namespace experimental {
namespace detail {

template <typename T, typename U>
struct qualify_as;

template <typename T, typename U>
struct qualify_as : meta::id<T> {};

template <typename T, typename U>
struct qualify_as<T, const U> : meta::id<const T> {};

template <typename T, typename U>
struct qualify_as<T, U &> : meta::id<T &> {};

template <typename T, typename U>
struct qualify_as<T, const U &> : meta::id<const T &> {};

template <typename T, typename U>
struct qualify_as<T, U &&> : meta::id<T &&> {};

template <typename T, typename U>
struct qualify_as<T, const U &&> : meta::id<const T &&> {};

template <typename T, typename U>
struct qualify_as<T, U *> : meta::id<T *> {};

template <typename T, typename U>
struct qualify_as<T, const U *> : meta::id<const T *> {};

}  // namespace detail
}  // namespace experimental
}  // namespace std

#endif  // VARIANT_DETAIL_QUALIFY_AS_HPP

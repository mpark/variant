// Copyright Michael Park 2015
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef VARIANT_DETAIL_QUALIFY_AS_HPP
#define VARIANT_DETAIL_QUALIFY_AS_HPP

#include <variant/detail/type_traits.hpp>

namespace std {
namespace experimental {
namespace detail {

template <typename T, typename U>
struct qualify_as;

template <typename T, typename U>
using qualify_as_t = typename qualify_as<T, U>::type;

template <typename T, typename U>
struct qualify_as : id<T> {};

template <typename T, typename U>
struct qualify_as<T, const U> : id<const qualify_as_t<T, U>> {};

template <typename T, typename U>
struct qualify_as<T, U &> : id<qualify_as_t<T, U> &> {};

template <typename T, typename U>
struct qualify_as<T, const U &> : id<const qualify_as_t<T, U> &> {};

template <typename T, typename U>
struct qualify_as<T, U &&> : id<qualify_as_t<T, U> &&> {};

template <typename T, typename U>
struct qualify_as<T, const U &&> : id<const qualify_as_t<T, U> &&> {};

template <typename T, typename U>
struct qualify_as<T, U *> : id<qualify_as_t<T, U> *> {};

template <typename T, typename U>
struct qualify_as<T, const U *> : id<const qualify_as_t<T, U> *> {};

}  // namespace detail
}  // namespace experimental
}  // namespace std

#endif  // VARIANT_DETAIL_QUALIFY_AS_HPP

// Copyright Michael Park 2015
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef VARIANT_DETAIL_REF_HPP
#define VARIANT_DETAIL_REF_HPP

#include <memory>
#include <type_traits>

#include <meta/meta.hpp>

namespace std {
namespace experimental {
namespace detail {

template <typename T>
class ref {
  public:
  static_assert(is_reference<T>{}, "");

  ref(T t) noexcept : ptr(addressof(static_cast<T>(t))) {}

  template <typename U = T, typename = meta::if_<is_lvalue_reference<U>>>
  ref(remove_reference_t<T> &&) = delete;

  ~ref() = default;

  ref(const ref &) = delete;
  ref(ref &&) = delete;
  ref &operator=(const ref &) = delete;
  ref &operator=(ref &&) = delete;

  operator T() const noexcept { return static_cast<T>(*ptr); }

  private:
  add_pointer_t<T> ptr;
};  // ref

}  // namespace detail
}  // namespace experimental
}  // namespace std

#endif  // VARIANT_DETAIL_REF_HPP

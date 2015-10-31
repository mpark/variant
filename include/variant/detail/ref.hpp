// Copyright Michael Park 2015
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef VARIANT_DETAIL_REF_HPP
#define VARIANT_DETAIL_REF_HPP

#include <type_traits>

#include <meta/meta.hpp>

namespace std {
namespace experimental {
namespace detail {

template <typename T>
class ref {
  public:
  static_assert(is_reference<T>{}, "");

  // TODO(mpark): Replace with `cosntexpr` version of `std::addressof`.
  //              For example, clang provides `__builtin_addressof`.
  constexpr ref(T t) noexcept : ptr(&static_cast<T>(t)) {}

  template <typename U = T, typename = meta::if_<is_lvalue_reference<U>>>
  constexpr ref(remove_reference_t<T> &&) = delete;

  ref(const ref &) = delete;
  ref(ref &&) = delete;

  ~ref() = default;

  ref &operator=(const ref &) = delete;
  ref &operator=(ref &&) = delete;

  constexpr operator T() const noexcept { return static_cast<T>(*ptr); }

  private:
  add_pointer_t<T> ptr;
};  // ref

}  // namespace detail
}  // namespace experimental
}  // namespace std

#endif  // VARIANT_DETAIL_REF_HPP

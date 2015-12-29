// Copyright Michael Park 2015
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef EXPERIMENTAL_DETAIL_VARIANT_REF_HPP
#define EXPERIMENTAL_DETAIL_VARIANT_REF_HPP

#include <type_traits>

namespace std {
namespace experimental {
namespace detail {
namespace variant {

template <typename T>
class ref {
  public:
  static_assert(is_reference<T>{}, "");

  // TODO(mpark): Replace with `cosntexpr` version of `std::addressof`.
  //              For example, clang provides `__builtin_addressof`.
  constexpr ref(T t) noexcept : ptr(&static_cast<T>(t)) {}

  template <typename U = T, typename = enable_if_t<is_lvalue_reference<U>{}>>
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

}  // namespace variant
}  // namespace detail
}  // namespace experimental
}  // namespace std

#endif  // EXPERIMENTAL_DETAIL_VARIANT_REF_HPP

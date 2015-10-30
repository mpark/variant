// Copyright Michael Park 2015
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef VARIANT_DETAIL_UNION_HPP
#define VARIANT_DETAIL_UNION_HPP

#include <new>
#include <type_traits>
#include <utility>

#include <meta/meta.hpp>

#include <variant/detail/ref.hpp>

namespace std {
namespace experimental {
namespace detail {

/* `union_` is a variadic recursive `union` which overlaps the N
   specified types. Compile-time indices are used to determine the element
   that needs to be constructed or retrieved. */
template <typename... Ts> class union_;

/* Base case for `union_`. */
template <> class union_<> {};

/* Recursive case for `union_`. */
template <typename T, typename... Ts>
class union_<T, Ts...> {
  public:
  using Head = meta::if_<is_reference<T>, ref<T>, T>;
  using Tail = union_<Ts...>;

  /* Must be handled by the user of this class. */
  constexpr union_() {}
  ~union_() {}

  /* Construct the element at index I. */

  template <typename... Args>
  explicit constexpr union_(meta::size_t<0>, Args &&... args)
      : head_(forward<Args>(args)...) {}

  template <size_t I, typename... Args>
  explicit constexpr union_(meta::size_t<I>, Args &&... args)
      : tail_(meta::size_t<I - 1>{}, forward<Args>(args)...) {}

  /* Access the element at index I. */

  constexpr T &operator[](meta::size_t<0>) & noexcept {
    return head_;
  }

  constexpr const T &operator[](meta::size_t<0>) const & noexcept {
    return head_;
  }

  constexpr T &&operator[](meta::size_t<0>) && noexcept {
    return move(head_);
  }

  constexpr const T &&operator[](meta::size_t<0>) const && noexcept {
    return move(head_);
  }

  template <size_t I>
  constexpr auto &&operator[](meta::size_t<I>) & noexcept {
    return static_cast<Tail &>(tail_)[meta::size_t<I - 1>{}];
  }

  template <size_t I>
  constexpr auto &&operator[](meta::size_t<I>) const & noexcept {
    return static_cast<const Tail &>(tail_)[meta::size_t<I - 1>{}];
  }

  template <size_t I>
  constexpr auto &&operator[](meta::size_t<I>) && noexcept {
    return static_cast<Tail &&>(tail_)[meta::size_t<I - 1>{}];
  }

  template <size_t I>
  constexpr auto &&operator[](meta::size_t<I>) const && noexcept {
    return static_cast<const Tail &&>(tail_)[meta::size_t<I - 1>{}];
  }

  /* Construct the element at index I. */

  template <typename... Args>
  void construct(meta::size_t<0>, Args &&... args) {
    ::new (&head_) Head(forward<Args>(args)...);
  }

  template <size_t I, typename... Args>
  void construct(meta::size_t<I>, Args &&... args) {
    tail_.construct(meta::size_t<I - 1>{}, forward<Args>(args)...);
  }

  /* Destruct the element at index I. */

  void destroy(meta::size_t<0>) { head_.~Head(); }

  template <size_t I>
  void destroy(meta::size_t<I>) { tail_.destroy(meta::size_t<I - 1>{}); }

  private:
  union {
    Head head_;
    Tail tail_;
  };
};  // union_<T, Ts...>

}  // namespace detail
}  // namespace experimental
}  // namespace std

#endif  // VARIANT_DETAIL_UNION_HPP

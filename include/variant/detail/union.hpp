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
template <typename Ts, bool IsTriviallyDestructible>
class union_;

/* Base case for `union_`. */
template <bool IsTriviallyDestructible>
class union_<meta::list<>, IsTriviallyDestructible> {};

/* Recursive case for `union_`. */
#define UNION(is_trivially_destructible, destructor)                    \
  template <typename T, typename... Ts>                                 \
  class union_<meta::list<T, Ts...>, is_trivially_destructible> {       \
    public:                                                             \
    using head = meta::if_<is_reference<T>, ref<T>, T>;                 \
    using tail = union_<meta::list<Ts...>, is_trivially_destructible>;  \
                                                                        \
    explicit constexpr union_() noexcept : data_() {}                   \
                                                                        \
    /* Construct the element at index I. */                             \
                                                                        \
    template <typename... Args>                                         \
    explicit constexpr union_(meta::size_t<0>, Args &&... args)         \
        : head_(forward<Args>(args)...) {}                              \
                                                                        \
    template <size_t I, typename... Args>                               \
    explicit constexpr union_(meta::size_t<I>, Args &&... args)         \
        : tail_(meta::size_t<I - 1>{}, forward<Args>(args)...) {}       \
                                                                        \
    union_(const union_ &) = default;                                   \
    union_(union_ &&) = default;                                        \
                                                                        \
    destructor;                                                         \
                                                                        \
    union_ &operator=(const union_ &) = default;                        \
    union_ &operator=(union_ &&) = default;                             \
                                                                        \
    /* Access the element at index I. */                                \
                                                                        \
    constexpr T &operator[](meta::size_t<0>) noexcept { return head_; } \
                                                                        \
    template <size_t I>                                                 \
    constexpr auto &&operator[](meta::size_t<I>) noexcept {             \
      return static_cast<tail &>(tail_)[meta::size_t<I - 1>{}];         \
    }                                                                   \
                                                                        \
    constexpr const T &operator[](meta::size_t<0>) const noexcept {     \
      return head_;                                                     \
    }                                                                   \
                                                                        \
    template <size_t I>                                                 \
    constexpr auto &&operator[](meta::size_t<I>) const noexcept {       \
      return static_cast<const tail &>(tail_)[meta::size_t<I - 1>{}];   \
    }                                                                   \
                                                                        \
    /* Construct the element at index I. */                             \
                                                                        \
    template <typename... Args>                                         \
    void construct(meta::size_t<0>, Args &&... args) {                  \
      ::new (&head_) head(forward<Args>(args)...);                      \
    }                                                                   \
                                                                        \
    template <size_t I, typename... Args>                               \
    void construct(meta::size_t<I>, Args &&... args) {                  \
      tail_.construct(meta::size_t<I - 1>{}, forward<Args>(args)...);   \
    }                                                                   \
                                                                        \
    /* Destruct the element at index I. */                              \
                                                                        \
    void destroy(meta::size_t<0>) noexcept { head_.~head(); }           \
                                                                        \
    template <size_t I>                                                 \
    void destroy(meta::size_t<I>) noexcept {                            \
      tail_.destroy(meta::size_t<I - 1>{});                             \
    }                                                                   \
                                                                        \
    private:                                                            \
    union {                                                             \
      uint8_t data_;                                                    \
      head head_;                                                       \
      tail tail_;                                                       \
    };                                                                  \
  };  // union_<meta::list<T, Ts...>, is_trivially_destructible>

/* is_trivially_destructible, destructor */
UNION(true , ~union_() = default)
UNION(false, ~union_() {}       )

#undef UNION

}  // namespace detail
}  // namespace experimental
}  // namespace std

#endif  // VARIANT_DETAIL_UNION_HPP

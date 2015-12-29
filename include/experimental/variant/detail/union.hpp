// Copyright Michael Park 2015
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef EXPERIMENTAL_VARIANT_DETAIL_UNION_HPP
#define EXPERIMENTAL_VARIANT_DETAIL_UNION_HPP

#include <new>
#include <type_traits>
#include <utility>

#include <experimental/variant/detail/ref.hpp>
#include <experimental/variant/detail/type_traits.hpp>

namespace std {
namespace experimental {
namespace detail {

/* `union_` is a variadic recursive `union` which overlaps the N
   specified types. Compile-time indices are used to determine the element
   that needs to be constructed or retrieved. */
template <bool IsTriviallyDestructible, typename... Ts>
class union_;

/* Base case for `union_`. */
template <bool IsTriviallyDestructible>
class union_<IsTriviallyDestructible> {};

/* Recursive case for `union_`. */
#define UNION(is_trivially_destructible, destructor)                     \
  template <typename T, typename... Ts>                                  \
  class union_<is_trivially_destructible, T, Ts...> {                    \
    public:                                                              \
    using head = conditional_t<is_reference<T>{}, ref<T>, T>;            \
    using tail = union_<is_trivially_destructible, Ts...>;               \
                                                                         \
    explicit constexpr union_() noexcept : data_() {}                    \
                                                                         \
    /* Construct the element at index I. */                              \
                                                                         \
    template <typename... Args>                                          \
    explicit constexpr union_(size_constant<0>, Args &&... args)         \
        : head_(forward<Args>(args)...) {}                               \
                                                                         \
    template <size_t I, typename... Args>                                \
    explicit constexpr union_(size_constant<I>, Args &&... args)         \
        : tail_(size_constant<I - 1>{}, forward<Args>(args)...) {}       \
                                                                         \
    union_(const union_ &) = default;                                    \
    union_(union_ &&) = default;                                         \
                                                                         \
    destructor                                                           \
                                                                         \
    union_ &operator=(const union_ &) = default;                         \
    union_ &operator=(union_ &&) = default;                              \
                                                                         \
    /* Access the element at index I. */                                 \
                                                                         \
    constexpr T &operator[](size_constant<0>) noexcept { return head_; } \
                                                                         \
    template <size_t I>                                                  \
    constexpr auto &&operator[](size_constant<I>) noexcept {             \
      return static_cast<tail &>(tail_)[size_constant<I - 1>{}];         \
    }                                                                    \
                                                                         \
    constexpr const T &operator[](size_constant<0>) const noexcept {     \
      return head_;                                                      \
    }                                                                    \
                                                                         \
    template <size_t I>                                                  \
    constexpr auto &&operator[](size_constant<I>) const noexcept {       \
      return static_cast<const tail &>(tail_)[size_constant<I - 1>{}];   \
    }                                                                    \
                                                                         \
    /* Construct the element at index I. */                              \
                                                                         \
    template <typename... Args>                                          \
    void construct(size_constant<0>, Args &&... args) {                  \
      ::new (&head_) head(forward<Args>(args)...);                       \
    }                                                                    \
                                                                         \
    template <size_t I, typename... Args>                                \
    void construct(size_constant<I>, Args &&... args) {                  \
      tail_.construct(size_constant<I - 1>{}, forward<Args>(args)...);   \
    }                                                                    \
                                                                         \
    /* Destruct the element at index I. */                               \
                                                                         \
    void destroy(size_constant<0>) noexcept { head_.~head(); }           \
                                                                         \
    template <size_t I>                                                  \
    void destroy(size_constant<I>) noexcept {                            \
      tail_.destroy(size_constant<I - 1>{});                             \
    }                                                                    \
                                                                         \
    private:                                                             \
    union {                                                              \
      uint8_t data_;                                                     \
      head head_;                                                        \
      tail tail_;                                                        \
    };                                                                   \
  };

/* is_trivially_destructible, destructor */
UNION(true , ~union_() = default;)
UNION(false, ~union_() {}        )

#undef UNION

}  // namespace detail
}  // namespace experimental
}  // namespace std

#endif  // EXPERIMENTAL_VARIANT_DETAIL_UNION_HPP

#pragma once

#include <new>
#include <utility>

#include <meta/meta.hpp>

namespace mpark {

  namespace detail {

    namespace variant {

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
        /* Must be handled by the user of this class. */
        constexpr union_() {}
        ~union_() {}

        /* Construct the element at index I. */

        template <typename... Args>
        explicit constexpr union_(meta::size_t<0>, Args &&... args)
            : head_(std::forward<Args>(args)...) {}

        template <std::size_t I, typename... Args>
        explicit constexpr union_(meta::size_t<I>, Args &&... args)
            : tail_(meta::size_t<I - 1>{}, std::forward<Args>(args)...) {}

        /* Access the element at index I. */

        constexpr T &operator[](meta::size_t<0>) & noexcept {
          return head_;
        }

        constexpr const T &operator[](meta::size_t<0>) const & noexcept {
          return head_;
        }

        constexpr T &&operator[](meta::size_t<0>) && noexcept {
          return std::move(head_);
        }

        constexpr const T &&operator[](meta::size_t<0>) const && noexcept {
          return std::move(head_);
        }

        template <std::size_t I>
        constexpr auto &operator[](meta::size_t<I>) & noexcept {
          return tail_[meta::size_t<I - 1>{}];
        }

        template <std::size_t I>
        constexpr const auto &operator[](meta::size_t<I>) const & noexcept {
          return tail_[meta::size_t<I - 1>{}];
        }

        template <std::size_t I>
        constexpr auto &&operator[](meta::size_t<I>) && noexcept {
          return std::move(tail_)[meta::size_t<I - 1>{}];
        }

        template <std::size_t I>
        constexpr const auto &&operator[](meta::size_t<I>) const && noexcept {
          return std::move(tail_)[meta::size_t<I - 1>{}];
        }

        /* Construct the element at index I. */

        template <typename... Args>
        void construct(meta::size_t<0>, Args &&... args) {
          ::new (&head_) T(std::forward<Args>(args)...);
        }

        template <std::size_t I, typename... Args>
        void construct(meta::size_t<I>, Args &&... args) {
          tail_.construct(meta::size_t<I - 1>{}, std::forward<Args>(args)...);
        }

        /* Destruct the element at index I. */

        void destruct(meta::size_t<0>) {
          head_.~T();
        }

        template <std::size_t I>
        void destruct(meta::size_t<I>) {
          tail_.destruct(meta::size_t<I - 1>{});
        }

        private:
        union {
          T head_;
          union_<Ts...> tail_;
        };

      };  // union_<T, Ts...>

    }  // namespace variant

  }  // namespace detail

}  // namespace mpark

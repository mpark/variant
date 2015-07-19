#pragma once

#include <utility>

#include <meta/meta.hpp>

namespace mpark {

  namespace detail {

    namespace variant {

      /**
       *   `union_t` is a variadic `union` which simply overlaps the N specified
       *   types inside of a union. Compile-time indices are used to determine
       *   that the element at the specified index needs to be constructed or
       *   retrieved.
       **/
      template <typename... Ts>
      class union_t;

      /* Base case for `union_t`. */
      template <>
      class union_t<> {};

      /* Recursive case for `union_t`. */
      template <typename T, typename... Ts>
      class union_t<T, Ts...> {
        public:

        /* The default constructor effectively does nothing. */
        constexpr union_t() : data_{} {}

        template <typename... Args>
        explicit constexpr union_t(meta::size_t<0>, Args &&... args)
            : head_(std::forward<Args>(args)...) {}

        template <std::size_t I, typename... Args>
        explicit constexpr union_t(meta::size_t<I>, Args &&... args)
            : tail_(meta::size_t<I - 1>{}, std::forward<Args>(args)...) {}

        /* Must be defined by the user of this class. */
        ~union_t() {}

        void *data() { return &data_; }

        const void *data() const { return &data_; }

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

        private:

        union {
          char data_;
          T head_;
          union_t<Ts...> tail_;
        };

      };  // union_t<T, Ts...>

    }  // namespace variant

  }  // namespace detail

}  // namespace mpark

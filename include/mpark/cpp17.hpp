// MPark.Variant
//
// Copyright Michael Park, 2015-2016
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef MPARK_CPP17_HPP
#define MPARK_CPP17_HPP

#include <memory>
#include <type_traits>
#include <utility>

#include <mpark/config.hpp>

namespace mpark {
  namespace cpp17 {

    template <bool B>
    using bool_constant = std::integral_constant<bool, B>;

    template <typename...>
    using void_t = void;

#define RETURN(...) -> decltype(__VA_ARGS__) { return __VA_ARGS__; }

    template <typename F, typename... As>
    inline constexpr auto invoke(F &&f, As &&... as)
        RETURN(std::forward<F>(f)(std::forward<As>(as)...))

    template <typename B, typename T, typename D>
    inline constexpr auto invoke(T B::*pmv, D &&d)
        RETURN(std::forward<D>(d).*pmv)

    template <typename Pmv, typename Ptr>
    inline constexpr auto invoke(Pmv pmv, Ptr &&ptr)
        RETURN((*std::forward<Ptr>(ptr)).*pmv)

    template <typename B, typename T, typename D, typename... As>
    inline constexpr auto invoke(T B::*pmf, D &&d, As &&... as)
        RETURN((std::forward<D>(d).*pmf)(std::forward<As>(as)...))

    template <typename Pmf, typename Ptr, typename... As>
    inline constexpr auto invoke(Pmf pmf, Ptr &&ptr, As &&... as)
        RETURN(((*std::forward<Ptr>(ptr)).*pmf)(std::forward<As>(as)...))

#undef RETURN

    namespace detail {
      namespace swappable {

        using std::swap;

        template <typename T>
        struct is_swappable_impl {
          private:
          struct sfinae_failure;

          template <typename U>
          static auto test(int)
              -> decltype(swap(std::declval<U &>(), std::declval<U &>()));

          template <typename U>
          static sfinae_failure test(void *);

          public:
          static constexpr bool value =
              !std::is_same<decltype(test<T>(0)), sfinae_failure>::value;
        };

        template <typename T>
        using is_swappable = bool_constant<is_swappable_impl<T>::value>;

        template <typename T, bool = is_swappable<T>::value>
        struct is_nothrow_swappable
            : bool_constant<noexcept(
                  swap(std::declval<T &>(), std::declval<T &>()))> {};

        template <typename T>
        struct is_nothrow_swappable<T, false> : std::false_type {};

      }  // namespace swappable
    }  // namespace detail

    template <typename T>
    using is_swappable = detail::swappable::is_swappable<T>;

    template <typename T>
    using is_nothrow_swappable = detail::swappable::is_nothrow_swappable<T>;

    template <typename F, typename = void>
    struct is_callable : std::false_type {};

    template <typename F>
    struct is_callable<F, void_t<std::result_of_t<F>>> : std::true_type {};

#ifdef MPARK_BUILTIN_ADDRESSOF
    template <typename T>
    constexpr T *addressof(T &arg) {
      return __builtin_addressof(arg);
    }
#else
    namespace detail {
      namespace has_addressof_impl {

        struct failure;

        template <typename T>
        failure operator&(T &&);

        template <typename T>
        static constexpr bool impl() {
          return (std::is_class<T>::value || std::is_union<T>::value) &&
                 !std::is_same<decltype(&std::declval<T &>()), failure>::value;
        }

      }  // namespace has_addressof_impl

      template <typename T>
      using has_addressof = bool_constant<has_addressof_impl::impl<T>()>;

    }  // namespace detail

    template <typename T,
              std::enable_if_t<detail::has_addressof<T>::value, int> = 0>
    T *addressof(T &arg) {
      return std::addressof(arg);
    }

    template <typename T,
              std::enable_if_t<!detail::has_addressof<T>::value, int> = 0>
    constexpr T *addressof(T &arg) {
      return &arg;
    }
#endif

    template <typename T>
    const T *addressof(const T &&) = delete;

  }  // namespace cpp17
}  // namespace mpark

#endif  // MPARK_CPP17_HPP

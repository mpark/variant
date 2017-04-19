// MPark.Variant
//
// Copyright Michael Park, 2015-2017
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef MPARK_VARIANTS_LIB_HPP
#define MPARK_VARIANTS_LIB_HPP

#include <memory>
#include <type_traits>
#include <utility>

#include <mpark/variants/config.hpp>

namespace mpark {
  namespace variants {
    namespace lib {

      template <typename T>
      struct identity { using type = T; };

#ifdef MPARK_TYPE_PACK_ELEMENT
      template <std::size_t I, typename... Ts>
      using type_pack_element_t = __type_pack_element<I, Ts...>;
#else
      template <std::size_t I, typename... Ts>
      struct type_pack_element_impl {
        private:
        template <std::size_t, typename T>
        struct indexed_type : identity<T> {};

        template <std::size_t... Is>
        inline static auto make_set(std::index_sequence<Is...>) {
          struct set : indexed_type<Is, Ts>... {};
          return set{};
        }

        template <typename T>
        inline static std::enable_if<true, T> impl(indexed_type<I, T>);

        inline static std::enable_if<false> impl(...);

        public:
        using type = decltype(impl(make_set(std::index_sequence_for<Ts...>{})));
      };

      template <std::size_t I, typename... Ts>
      using type_pack_element = typename type_pack_element_impl<I, Ts...>::type;

      template <std::size_t I, typename... Ts>
      using type_pack_element_t = typename type_pack_element<I, Ts...>::type;
#endif

      inline namespace cpp17 {

        // <type_traits>
        template <bool B>
        using bool_constant = std::integral_constant<bool, B>;

        template <typename...>
        using void_t = void;

        namespace detail {
          namespace swappable {

            using std::swap;

            template <typename T>
            struct is_swappable_impl {
              private:
              template <typename U,
                        typename = decltype(swap(std::declval<U &>(),
                                                 std::declval<U &>()))>
              inline static std::true_type test(int);

              template <typename U>
              inline static std::false_type test(...);

              public:
              using type = decltype(test<T>(0));
            };

            template <typename T>
            using is_swappable = typename is_swappable_impl<T>::type;

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

        // <functional>
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

          template <typename Void, typename, typename...>
          struct invoke_result {};

          template <typename F, typename... Args>
          struct invoke_result<void_t<decltype(lib::invoke(
                                   std::declval<F>(), std::declval<Args>()...))>,
                               F,
                               Args...>
              : identity<decltype(
                    lib::invoke(std::declval<F>(), std::declval<Args>()...))> {};

        }  // namespace detail

        template <typename F, typename... Args>
        using invoke_result = detail::invoke_result<void, F, Args...>;

        template <typename F, typename... Args>
        using invoke_result_t = typename invoke_result<F, Args...>::type;

        namespace detail {

          template <typename Void, typename, typename...>
          struct is_invocable : std::false_type {};

          template <typename F, typename... Args>
          struct is_invocable<void_t<invoke_result_t<F, Args...>>, F, Args...>
              : std::true_type {};

          template <typename Void, typename, typename, typename...>
          struct is_invocable_r : std::false_type {};

          template <typename R, typename F, typename... Args>
          struct is_invocable_r<void_t<invoke_result_t<F, Args...>>,
                                R,
                                F,
                                Args...>
              : std::is_convertible<invoke_result_t<F, Args...>, R> {};

        }  // namespace detail

        template <typename F, typename... Args>
        using is_invocable = detail::is_invocable<void, F, Args...>;

        template <typename R, typename F, typename... Args>
        using is_invocable_r = detail::is_invocable_r<void, R, F, Args...>;

        // <memory>
#ifdef MPARK_BUILTIN_ADDRESSOF
        template <typename T>
        inline constexpr T *addressof(T &arg) {
          return __builtin_addressof(arg);
        }
#else
        namespace detail {

          namespace has_addressof_impl {

            struct fail;

            template <typename T>
            inline fail operator&(T &&);

            template <typename T>
            inline static constexpr bool impl() {
              return (std::is_class<T>::value || std::is_union<T>::value) &&
                     !std::is_same<decltype(&std::declval<T &>()), fail>::value;
            }

          }  // namespace has_addressof_impl

          template <typename T>
          using has_addressof = bool_constant<has_addressof_impl::impl<T>()>;

        }  // namespace detail

        template <typename T,
                  std::enable_if_t<detail::has_addressof<T>::value, int> = 0>
        inline T *addressof(T &arg) {
          return std::addressof(arg);
        }

        template <typename T,
                  std::enable_if_t<!detail::has_addressof<T>::value, int> = 0>
        inline constexpr T *addressof(T &arg) {
          return &arg;
        }
#endif

        template <typename T>
        inline const T *addressof(const T &&) = delete;

      }  // namespace cpp17

    }  // namespace lib
  }  // namespace variants
}  // namespace mpark

#endif  // MPARK_VARIANTS_LIB_HPP

// Copyright Michael Park 2015
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef EXPERIMENTAL_LIB_HPP
#define EXPERIMENTAL_LIB_HPP

#include <array>
#include <functional>
#include <initializer_list>
#include <type_traits>
#include <utility>

namespace std {
namespace experimental {
namespace cpp17 {

template <size_t B>
using bool_constant = integral_constant<bool, B>;

/* `std::invoke` */

#define RETURN(...) -> decltype(__VA_ARGS__) { return __VA_ARGS__; }

template <typename F, typename... As>
constexpr auto invoke(F &&f, As &&... as) RETURN(forward<F>(f)(forward<As>(as)...))

template <typename B, typename T, typename D>
constexpr auto invoke(T B::*pmv, D &&d) RETURN(forward<D>(d).*pmv)

template <typename Pmv, typename Ptr>
constexpr auto invoke(Pmv pmv, Ptr &&ptr) RETURN((*forward<Ptr>(ptr)).*pmv)

template <typename B, typename T, typename D, typename... As>
constexpr auto invoke(T B::*pmf, D &&d, As &&... as) RETURN((forward<D>(d).*pmf)(forward<As>(as)...))

template <typename Pmf, typename Ptr, typename... As>
constexpr auto invoke(Pmf pmf, Ptr &&ptr, As &&... as) RETURN(((*forward<Ptr>(ptr)).*pmf)(forward<As>(as)...))

#undef RETURN

/* `std::conjunction` */
template <typename... Ts>
struct conjunction;

template <>
struct conjunction<> : true_type {};

template <typename B>
struct conjunction<B> : B {};

template <typename B, typename... Bs>
struct conjunction<B, Bs...> : conditional_t<B::value, conjunction<Bs...>, B> {};

/* `std::disjunction` */
template <typename... Ts>
struct disjunction;

template <>
struct disjunction<> : false_type {};

template <typename B>
struct disjunction<B> : B {};

template <typename B, typename... Bs>
struct disjunction<B, Bs...> : conditional_t<B::value, B, disjunction<Bs...>> {};

/* `std::negation` */
template <typename B>
struct negation : bool_constant<!B::value> {};

}  // namespace cpp17

namespace lib {

template <size_t I>
using size_constant = integral_constant<size_t, I>;

// N4511
template <typename T, typename U = T>
struct is_nothrow_swappable : cpp17::bool_constant<noexcept(swap(declval<T &>(), declval<U &>()))> {};

template <typename... Ts>
struct false_t : false_type {};

template <typename T>
struct id { using type = T; };

/* `rank` for `std::array`. */
template <typename T>
struct rank : size_constant<0> {};

template <typename T, size_t N>
struct rank<array<T, N>> : size_constant<rank<T>{} + 1> {};

/* `repack` */
template <typename From, template <typename... Ts> class To>
struct repack;

template <typename From, template <typename...> class To>
using repack_t = typename repack<From, To>::type;

template <template <typename... Ts> class From, typename... Ts, template <typename...> class To>
struct repack<From<Ts...>, To> : id<To<Ts...>> {};

/* `qualify_as` */
template <typename T, typename U>
struct qualify_as;

template <typename T, typename U>
using qualify_as_t = typename qualify_as<T, U>::type;

template <typename T, typename U>
struct qualify_as : id<T> {};

template <typename T, typename U>
struct qualify_as<T, const U> : add_const<qualify_as_t<T, U>> {};

template <typename T, typename U>
struct qualify_as<T, volatile U> : add_volatile<qualify_as_t<T, U>> {};

template <typename T, typename U>
struct qualify_as<T, const volatile U> : add_cv<qualify_as_t<T, U>> {};

template <typename T, typename U>
struct qualify_as<T, U &> : add_lvalue_reference<qualify_as_t<T, U>> {};

template <typename T, typename U>
struct qualify_as<T, U &&> : add_rvalue_reference<qualify_as_t<T, U>> {};

template <typename T, typename U>
struct qualify_as<T, U *> : add_pointer<qualify_as_t<T, U>> {};

/* `npos` */
static constexpr size_t npos = static_cast<size_t>(-1);

/* `count` */
template <typename T>
constexpr size_t count(initializer_list<T> elems, const T &value) {
  size_t result = 0;
  for (const auto &elem : elems) {
    if (elem == value) {
      ++result;
    }  // if
  }  // for
  return result;
}

/* `find` */
template <typename T>
constexpr size_t find(initializer_list<T> elems, const T &value) {
  size_t result = 0;
  for (const auto &elem : elems) {
    if (elem == value) {
      return result;
    }  // if
    ++result;
  }  // for
  return npos;
}

/* `any_of` */
template <typename T>
constexpr bool any_of(initializer_list<T> elems, const T &value) {
  return find(elems, value) != npos;
}

}  // namespace lib

namespace funds_v2 {

template <typename T, typename...>
struct deduce : lib::id<T> {};

template <class... Ts>
struct deduce<void, Ts...> : lib::id<common_type_t<Ts...>> {
  template <typename>
  struct is_reference_wrapper : false_type {};

  template <typename T>
  struct is_reference_wrapper<reference_wrapper<T>> : true_type {};

  static_assert(cpp17::conjunction<cpp17::negation<is_reference_wrapper<decay_t<Ts>>>...>::value,
                "`Ts...` cannot contain `reference_wrapper`s when `T` is `void`.");
};

template <typename T, typename... Ts>
using deduce_t = typename deduce<T, Ts...>::type;

template <typename T = void, typename... Ts>
constexpr array<deduce_t<T, Ts...>, sizeof...(Ts)> make_array(Ts &&... ts) {
  return {{forward<Ts>(ts)...}};
}

}  // namespace funds_v2
}  // namespace experimental
}  // namespace std

#endif  // EXPERIMENTAL_LIB_HPP

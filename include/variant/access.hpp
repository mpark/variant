#ifndef VARIANT_ACCESS_HPP
#define VARIANT_ACCESS_HPP

#include <cassert>
#include <type_traits>

#include <meta/meta.hpp>

#include <variant/bad_variant_access.hpp>
#include <variant/detail/unsafe/get.hpp>

namespace std {
namespace experimental {

// Forward declaration
template <typename... Ts> class variant;

//- 20.N.7 element access:

template <size_t I, typename... Ts>
bool holds_alternative(const variant<Ts...> &v) noexcept {
  return I == v.index();
}

template <typename T, typename... Ts>
bool holds_alternative(const variant<Ts...> &v) noexcept {
  using members = meta::list<Ts...>;
  static_assert(meta::count<members, T>{} == 1, "");
  return holds_alternative<meta::find_index<members, T>{}>(v);
}

template <size_t I, typename... Ts>
auto *get_if(const variant<Ts...> *v) noexcept {
  using members = meta::list<Ts...>;
  static_assert(I < members::size(), "");
  using T = meta::at_c<members, I>;
  assert(v);
  if (!holds_alternative<I>(*v)) {
    return static_cast<add_pointer_t<const T>>(nullptr);
  }  // if
  return &detail::unsafe::get<I>(*v);
}

template <size_t I, typename... Ts>
auto *get_if(variant<Ts...> *v) noexcept {
  using T = meta::at_c<meta::list<Ts...>, I>;
  return const_cast<add_pointer_t<T>>(
      get_if<I>(static_cast<const variant<Ts...> *>(v)));
}

template <typename T, typename... Ts>
auto *get_if(const variant<Ts...> *v) noexcept {
  using members = meta::list<Ts...>;
  static_assert(meta::count<members, T>{} == 1, "");
  return get_if<meta::find_index<members, T>{}>(v);
}

template <typename T, typename... Ts>
auto *get_if(variant<Ts...> *v) noexcept {
  return const_cast<add_pointer_t<T>>(
      get_if<T>(static_cast<const variant<Ts...> *>(v)));
}

template <size_t I, typename... Ts>
auto &&get(const variant<Ts...> &v) {
  auto *result = get_if<I>(&v);
  return result ? *result : throw bad_variant_access{};
}

template <size_t I, typename... Ts>
auto &&get(variant<Ts...> &v) {
  using T = meta::at_c<meta::list<Ts...>, I>;
  return const_cast<T &>(get<I>(static_cast<const variant<Ts...> &>(v)));
}

template <size_t I, typename... Ts>
auto &&get(const variant<Ts...> &&v) {
  using T = meta::at_c<meta::list<Ts...>, I>;
  return static_cast<const T &&>(get<I>(v));
}

template <size_t I, typename... Ts>
auto &&get(variant<Ts...> &&v) {
  using T = meta::at_c<meta::list<Ts...>, I>;
  return static_cast<T &&>(get<I>(v));
}

template <typename T, typename... Ts>
auto &&get(const variant<Ts...> &v) {
  using members = meta::list<Ts...>;
  static_assert(meta::count<members, T>{} == 1, "");
  return get<meta::find_index<members, T>{}>(v);
}

template <typename T, typename... Ts>
auto &&get(variant<Ts...> &v) {
  return const_cast<T &>(get<T>(static_cast<const variant<Ts...> &>(v)));
}

template <typename T, typename... Ts>
auto &&get(const variant<Ts...> &&v) {
  return static_cast<const T &&>(get<T>(v));
}

template <typename T, typename... Ts>
auto &&get(variant<Ts...> &&v) {
  return static_cast<T &&>(get<T>(v));
}

}  // namespace experimental
}  // namespace std

#endif  // VARIANT_ACCESS_HPP

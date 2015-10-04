#pragma once

#include <type_traits>

#include <meta/meta.hpp>

#include <mpark/variant/bad_variant_access.hpp>
#include <mpark/variant/type_switch.hpp>

namespace mpark {

  //- 20.N.7 element access:

  template <std::size_t I, typename... Ts>
  auto *get(const variant<Ts...> *v) noexcept {
    using members = meta::list<Ts...>;
    static_assert(I < members::size());
    using T = meta::at_c<members, I>;
    using R = std::remove_reference_t<const T>;
    if (!v) {
      return static_cast<R *>(nullptr);
    }  // if
    return type_switch (*v) (
      [](const T &elem) { return &elem; },
      [](auto &&) -> R * { return nullptr; }
    );
  }

  template <std::size_t I, typename... Ts>
  auto *get(variant<Ts...> *v) noexcept {
    using T = meta::at_c<meta::list<Ts...>, I>;
    return const_cast<std::remove_reference_t<T> *>(
        get<I>(static_cast<const variant<Ts...> *>(v)));
  }

  template <typename T, typename... Ts>
  auto *get(const variant<Ts...> *v) noexcept {
    using members = meta::list<Ts...>;
    static_assert(meta::count<members, T>{} == 1);
    return get<meta::find_index<members, T>{}>(v);
  }

  template <typename T, typename... Ts>
  auto *get(variant<Ts...> *v) noexcept {
    return const_cast<std::remove_reference_t<T> *>(
        get<T>(static_cast<const variant<Ts...> *>(v)));
  }

  template <std::size_t I, typename... Ts>
  const auto &get(const variant<Ts...> &v) {
    auto *result = get<I>(&v);
    return result ? *result : throw bad_variant_access{};
  }

  template <std::size_t I, typename... Ts>
  auto &get(variant<Ts...> &v) {
    using T = meta::at_c<meta::list<Ts...>, I>;
    return const_cast<T &>(get<I>(static_cast<const variant<Ts...> &>(v)));
  }

  template <std::size_t I, typename... Ts>
  const auto &&get(const variant<Ts...> &&v) {
    using T = meta::at_c<meta::list<Ts...>, I>;
    return static_cast<const T &&>(get<I>(v));
  }

  template <std::size_t I, typename... Ts>
  auto &&get(variant<Ts...> &&v) {
    using T = meta::at_c<meta::list<Ts...>, I>;
    return static_cast<T &&>(get<I>(v));
  }

  template <typename T, typename... Ts>
  const auto &get(const variant<Ts...> &v) {
    using members = meta::list<Ts...>;
    static_assert(meta::count<members, T>{} == 1);
    return get<meta::find_index<members, T>{}>(v);
  }

  template <typename T, typename... Ts>
  auto &get(variant<Ts...> &v) {
    return const_cast<T &>(get<T>(static_cast<const variant<Ts...> &>(v)));
  }

  template <typename T, typename... Ts>
  const auto &&get(const variant<Ts...> &&v) {
    return static_cast<const T &&>(get<T>(v));
  }

  template <typename T, typename... Ts>
  auto &&get(variant<Ts...> &&v) {
    return static_cast<T &&>(get<T>(v));
  }

}  // namespace mpark

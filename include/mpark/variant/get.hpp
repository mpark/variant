#pragma once

#include <cassert>
#include <type_traits>
#include <utility>

#include <meta/meta.hpp>

#include <mpark/variant/bad_variant_access.hpp>
#include <mpark/variant/apply.hpp>
#include <mpark/variant/overload.hpp>
#include <mpark/variant/variant.hpp>

namespace mpark {

  template <typename T, typename... Ts>
  std::remove_reference_t<T> *get(variant<Ts...> *v) noexcept {
    return const_cast<std::remove_reference_t<T> *>(
        get<T>(static_cast<const variant<Ts...> *>(v)));
  }

  template <typename T, typename... Ts>
  std::remove_reference_t<const T> *get(const variant<Ts...> *v) noexcept {
    static_assert(meta::in<meta::list<Ts...>, T>{},
                  "T must be one of the types in the variant.");
    assert(v);
    return apply(mpark::make_overload(
                     [](const T &elem) { return &elem; },
                     [](auto &&) -> std::remove_reference_t<const T> * {
                       return nullptr;
                     }),
                 *v);
  }

  template <typename T, typename... Ts>
  T &get(variant<Ts...> &v) {
    return const_cast<T &>(get<T>(static_cast<const variant<Ts...> &>(v)));
  }

  template <typename T, typename... Ts>
  const T &get(const variant<Ts...> &v) {
    std::remove_reference_t<const T> *result = get<T>(&v);
    return result ? *result : throw bad_variant_access("get");
  }

  template <typename T, typename... Ts>
  T &&get(variant<Ts...> &&v) {
    return static_cast<T &&>(get<T>(v));
  }

  template <typename T, typename... Ts>
  const T &&get(const variant<Ts...> &&v) {
    return static_cast<const T &&>(get<T>(v));
  }

}  // namespace mpark

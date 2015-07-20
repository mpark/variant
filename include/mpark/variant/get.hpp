#pragma once

#include <type_traits>
#include <utility>

#include <meta/meta.hpp>

#include <mpark/variant/bad_variant_access.hpp>
#include <mpark/variant/apply.hpp>
#include <mpark/variant/variant.hpp>

namespace mpark {

  namespace detail {

    namespace variant {

      template <typename T>
      struct getter {

        const T &operator()(const T &arg) const { return arg; }

        template <typename U>
        const T &operator()(U &&) const { throw bad_variant_access("get"); }

      };  // getter

    }  // namespace variant

  }  // namespace detail

  template <typename T, typename... Ts>
  T &get(variant<Ts...> &v) {
    return const_cast<T &>(get<T>(static_cast<const variant<Ts...> &>(v)));
  }

  template <typename T, typename... Ts>
  const T &get(const variant<Ts...> &v) {
    static_assert(meta::in<meta::list<Ts...>, T>{},
                  "T must be one of the types in the variant.");
    return apply(detail::variant::getter<T>{}, v);
  }

  template <typename T, typename... Ts>
  T &&get(variant<Ts...> &&v) {
    return const_cast<T &&>(get<T>(static_cast<const variant<Ts...> &&>(v)));
  }

  template <typename T, typename... Ts>
  const T &&get(const variant<Ts...> &&v) {
    return std::move(get<T>(v));
  }

}  // namespace mpark

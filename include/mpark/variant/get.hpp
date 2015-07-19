#pragma once

#include <type_traits>
#include <utility>

#include <meta/meta.hpp>

#include <mpark/variant/detail/qualify.hpp>

#include <mpark/variant/bad_variant_access.hpp>
#include <mpark/variant/apply.hpp>
#include <mpark/variant/variant.hpp>

namespace mpark {

  namespace detail {

    namespace variant {

      template <typename T>
      struct getter {

        T operator()(T arg) const { return static_cast<T>(arg); }

        template <typename U>
        T operator()(U &&) const {
          throw bad_variant_access("get");
        }

      };  // getter

      template <typename T, typename V>
      auto &&get(V &&v) {
        static_assert(meta::in<meta::as_list<std::decay_t<V>>, T>{},
                      "T must be one of the types in the variant.");
        return apply(getter<qualify_t<T, V &&>>{}, std::forward<V>(v));
      }

    }  // namespace variant

  }  // namespace detail

  template <typename T, typename... Ts>
  T &get(variant<Ts...> &v) {
    return detail::variant::get<T>(v);
  }

  template <typename T, typename... Ts>
  const T &get(const variant<Ts...> &v) {
    return detail::variant::get<T>(v);
  }

  template <typename T, typename... Ts>
  T &&get(variant<Ts...> &&v) {
    return detail::variant::get<T>(std::move(v));
  }

  template <typename T, typename... Ts>
  const T &&get(const variant<Ts...> &&v) {
    return detail::variant::get<T>(std::move(v));
  }
}  // namespace mpark

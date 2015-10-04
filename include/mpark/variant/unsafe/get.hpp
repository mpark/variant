#pragma once

#include <cassert>

#include <meta/meta.hpp>

namespace mpark {

  /* Forward declaration */
  template <typename... Ts>
  class variant;

  namespace unsafe {

    struct detail {
      template <std::size_t I, typename... Ts>
      static const auto &get(const mpark::variant<Ts...> &v) {
        using members = meta::list<Ts...>;
        static_assert(I < members::size(), "");
        using T = meta::at_c<members, I>;
        assert(v.valid());
        return static_cast<const T &>(v[meta::size_t<I>{}]);
      }
    };  // detail

    template <std::size_t I, typename... Ts>
    const auto &get(const variant<Ts...> &v) {
      return detail::get<I>(v);
    }

    template <std::size_t I, typename... Ts>
    auto &get(variant<Ts...> &v) {
      using T = meta::at_c<meta::list<Ts...>, I>;
      return const_cast<T &>(
          unsafe::get<I>(static_cast<const variant<Ts...> &>(v)));
    }

    template <std::size_t I, typename... Ts>
    const auto &&get(const variant<Ts...> &&v) {
      using T = meta::at_c<meta::list<Ts...>, I>;
      return static_cast<const T &&>(unsafe::get<I>(v));
    }

    template <std::size_t I, typename... Ts>
    auto &&get(variant<Ts...> &&v) {
      using T = meta::at_c<meta::list<Ts...>, I>;
      return static_cast<T &&>(unsafe::get<I>(v));
    }

    template <typename T, typename... Ts>
    const auto &get(const variant<Ts...> &v) {
      using members = meta::list<Ts...>;
      static_assert(meta::count<members, T>{} == 1, "");
      return unsafe::get<meta::find_index<members, T>{}>(v);
    }

    template <typename T, typename... Ts>
    auto &get(variant<Ts...> &v) {
      return const_cast<T &>(
          unsafe::get<T>(static_cast<const variant<Ts...> &>(v)));
    }

    template <typename T, typename... Ts>
    const auto &&get(const variant<Ts...> &&v) {
      return static_cast<const T &&>(unsafe::get<T>(v));
    }

    template <typename T, typename... Ts>
    auto &&get(variant<Ts...> &&v) {
      return static_cast<T &&>(unsafe::get<T>(v));
    }

  }  // namespace unsafe

}  // namespace mpark

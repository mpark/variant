#pragma once

#include <cstddef>

#include <meta/meta.hpp>

#include <mpark/variant/nullvar.hpp>
#include <mpark/variant/type_switch.hpp>

namespace std {

  //- 20.N.10 hash support:

  template <>
  struct hash<mpark::nullvar_t> {
    using argument_type = mpark::nullvar_t;
    using result_type = std::size_t;

    result_type operator()(const argument_type &) const { return 0u; }
  };  // hash<mpark::nullvar_t>

  template <typename... Ts>
  struct hash<mpark::variant<Ts...>> {
    using argument_type = mpark::variant<Ts...>;
    using result_type = std::size_t;

    result_type operator()(const argument_type &v) const {
      return type_switch(v)(hasher{});
    }

    private:
    struct hasher {
      template <std::size_t I, typename Arg>
      result_type operator()(Arg &&arg) const {
        using T = meta::at_c<meta::list<Ts...>, I>;
        return std::hash<T>{}(std::forward<Arg>(arg));
      }
    };  // hasher
  };  // hash<mpark::variant<Ts...>>

}  // namespace std

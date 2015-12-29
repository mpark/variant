// Copyright Michael Park 2015
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef EXPERIMENTAL_VARIANT_HASH_HPP
#define EXPERIMENTAL_VARIANT_HASH_HPP

#include <utility>

#include <experimental/detail/variant/index_visitor.hpp>
#include <experimental/tuple.hpp>
#include <experimental/variant/unsafe/visit.hpp>

namespace std {

//- 20.N.M hash support:

template <typename... Ts>
struct hash<experimental::variant<Ts...>> {
  using argument_type = experimental::variant<Ts...>;
  using result_type = size_t;

  result_type operator()(const argument_type &v) const {
    using namespace experimental;
    return unsafe::visit(detail::variant::make_index_visitor<hasher>(), v);
  }

  private:
  template <size_t I>
  struct hasher {
    template <typename Arg>
    result_type operator()(const Arg &arg) const {
      using T = experimental::tuple_element_t<I, experimental::variant<Ts...>>;
      return hash<T>{}(arg);
    }
  };  // hasher
};  // hash<experimental::variant<Ts...>>

}  // namespace std

#endif  // EXPERIMENTAL_VARIANT_HASH_HPP

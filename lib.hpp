// MPark.Variant
//
// Copyright Michael Park, 2015-2017
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)

#ifndef LIB_HPP
#define LIB_HPP

namespace lib {

#ifdef __cpp_lib_integer_sequence
  using std::index_sequence;
  using std::index_sequence_for;
  using std::integer_sequence;
  using std::make_index_sequence;
#else
  template <typename T, T... Is>
  struct integer_sequence {
    using value_type = T;
    static constexpr std::size_t size() noexcept { return sizeof...(Is); }
  };

  template <std::size_t... Is>
  using index_sequence = integer_sequence<std::size_t, Is...>;

  template <typename Lhs, typename Rhs>
  struct make_index_sequence_concat;

  template <std::size_t... Lhs, std::size_t... Rhs>
  struct make_index_sequence_concat<index_sequence<Lhs...>,
                                    index_sequence<Rhs...>> {
    using type = index_sequence<Lhs..., (sizeof...(Lhs) + Rhs)...>;
  };

  template <std::size_t N>
  struct make_index_sequence_impl;

  template <std::size_t N>
  using make_index_sequence = typename make_index_sequence_impl<N>::type;

  template <std::size_t N>
  struct make_index_sequence_impl
      : make_index_sequence_concat<make_index_sequence<N / 2>,
                                   make_index_sequence<N - (N / 2)>> {};

  template <>
  struct make_index_sequence_impl<0> {
    using type = index_sequence<>;
  };

  template <>
  struct make_index_sequence_impl<1> {
    using type = index_sequence<0>;
  };
#endif

}  // namespace lib

#endif  // LIB_HPP

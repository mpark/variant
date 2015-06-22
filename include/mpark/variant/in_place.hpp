#pragma once

namespace mpark {

  template <typename T>
  struct in_place_t {};

  template <typename T>
  constexpr in_place_t<T> in_place = {};

}  // namespace mpark

#pragma once

#include <ostream>
#include <string>

#include <mpark/variant/type_switch.hpp>

namespace mpark {

  template <typename Char,
            typename Traits = std::char_traits<Char>,
            typename... Ts>
  std::basic_ostream<Char, Traits> &operator<<(
      std::basic_ostream<Char, Traits> &strm, const variant<Ts...> &v) {
    type_switch(v)([&](const auto &elem) { strm << elem; });
    return strm;
  }

}  // namespace mpark

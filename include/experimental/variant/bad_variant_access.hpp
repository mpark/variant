// Copyright Michael Park 2015
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef EXPERIMENTAL_VARIANT_BAD_VARIANT_ACCESS_HPP
#define EXPERIMENTAL_VARIANT_BAD_VARIANT_ACCESS_HPP

#include <stdexcept>

namespace std {
namespace experimental {

//- 20.N.M class `bad_variant_access`:

/* exception indicating an invalid access to a `variant` object

   The class `bad_variant_access` defines the type of objects thrown as
   exceptions to report the situation where an invalid attempt is made to
   access the value of a `variant` via `get`. */
class bad_variant_access : public exception {
  public:
  bad_variant_access() = default;
  bad_variant_access(const bad_variant_access &) = default;
  bad_variant_access &operator=(const bad_variant_access &) = default;
  virtual const char *what() const noexcept { return "bad_variant_access"; }
};  // bad_variant_access

}  // namespace experimental
}  // namespace std

#endif  // EXPERIMENTAL_VARIANT_BAD_VARIANT_ACCESS_HPP

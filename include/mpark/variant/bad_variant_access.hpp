#pragma once

#include <stdexcept>

namespace mpark {

  //- 20.N.3 class `bad_variant_access`

  /* exception indicating an invalid access to a `variant` object

     The class `bad_variant_access` defines the type of objects thrown as
     exceptions to report the situation where an invalid attempt is made to
     access the value of a `variant` via `get`.

     Given an object `v` of type `variant<Types...>`,
       * For index-based operations,
         `get<I>(v)` is invalid if `get<I>(&v) == nullptr`
       * For type-based operations,
         `get<T>(v)` is invalid if `get<T>(&v) == nullptr` */
  class bad_variant_access : public std::exception {
    public:
    bad_variant_access() = default;
    bad_variant_access(const bad_variant_access &) = default;
    bad_variant_access &operator=(const bad_variant_access &) = default;
    virtual const char *what() const noexcept { return "bad_variant_access"; }
  };  // bad_variant_access

}  // namespace mpark

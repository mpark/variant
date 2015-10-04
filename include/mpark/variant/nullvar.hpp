#pragma once

namespace mpark {

  //- 20.N.6 nullvariant:

  /* indicator of `variant` type with indeterminate state

     `nullvar_t` is an empty class type used as an alternative of a `variant`
     which allows the user to opt-in for the presence of a conceptual
     representation of the empty state. */
  struct nullvar_t {};

  /* an object of type `nullvar_t`

     `nullvar` is a constant of type `nullvar_t` that is used as an alternative
     of a `variant` which allows the user to opt-in for the presence of a
     conceptual representation of the empty state. */
  constexpr nullvar_t nullvar = {};

}  // namespace mpark

#pragma once

#include <exception>

namespace mpark {

  //- 20.N.4 class `bad_variant_assign`

  /* exception indicating a failed `variant` assignment

     The class `bad_variant_assign` defines the type of objects thrown as nested
     exceptions to report the situation where an exception was thrown during an
     assignment (i.e. `operator=`, `emplace`) into a `variant` object and is
     left in an indeterminate state. */
  class bad_variant_assign : public std::exception,
                             public std::nested_exception {
    public:
    bad_variant_assign(void *v_ptr) noexcept : variant_ptr_(v_ptr) {}
    bad_variant_assign(const bad_variant_assign &) = default;
    bad_variant_assign &operator=(const bad_variant_assign &) = default;
    void *variant_ptr() const noexcept { return variant_ptr_; }
    virtual const char *what() const noexcept { return "bad_variant_assign"; }

    private:
    void *variant_ptr_;
  };  // bad_variant_assign

}  // namespace mpark

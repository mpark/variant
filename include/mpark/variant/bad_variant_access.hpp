#pragma once

#include <stdexcept>
#include <string>

namespace mpark {

  /* The exception to be thrown on a bad variant access. */
  class bad_variant_access : public std::logic_error {
    public:

    explicit bad_variant_access(const std::string &what_arg)
        : std::logic_error(what_arg) {}

    explicit bad_variant_access(const char *what_arg)
        : std::logic_error(what_arg) {}

  };  // bad_variant_access

}  // namespace mpark

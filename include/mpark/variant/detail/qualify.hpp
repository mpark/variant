#pragma once

#include <meta/meta.hpp>

namespace mpark {

  namespace detail {

    namespace variant {

      /* qualify<T, U> qualifies T to match U's qualifiers.
         e.g. `qualify<int, const void *>` == `const void *` */

      template <typename T, typename U>
      struct qualify;

      template <typename T, typename U>
      using qualify_t = meta::_t<qualify<T, U>>;

      template <typename T, typename U>
      struct qualify<T, U &> : meta::id<T &> {};

      template <typename T, typename U>
      struct qualify<T, const U &> : meta::id<const T &> {};

      template <typename T, typename U>
      struct qualify<T, U &&> : meta::id<T &&> {};

      template <typename T, typename U>
      struct qualify<T, const U &&> : meta::id<const T &&> {};

      template <typename T, typename U>
      struct qualify<T, U *> : meta::id<T *> {};

      template <typename T, typename U>
      struct qualify<T, const U *> : meta::id<const T *> {};

    }  // namespace variant

  }  // namespace detail

}  // namespace mpark

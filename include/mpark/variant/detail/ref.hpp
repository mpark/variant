#pragma once

#include <memory>
#include <type_traits>

namespace mpark {

  namespace detail {

    namespace variant {

      template <typename T>
      class ref {
        public:
        static_assert(std::is_reference<T>{});

        ref(T r) noexcept : ptr(std::addressof(static_cast<T>(r))) {}

        template <typename U = T,
                  typename = std::enable_if_t<std::is_lvalue_reference<U>{}>>
        ref(std::remove_reference_t<T> &&) = delete;

        ~ref() = default;

        ref(const ref &) = delete;
        ref(ref &&) = delete;
        ref &operator=(const ref &) = delete;
        ref &operator=(ref &&) = delete;

        operator T() const noexcept { return static_cast<T>(*ptr); }

        private:
        std::remove_reference_t<T> *ptr;
      };  // ref

    }  // variant

  }  // detail

}  // mpark

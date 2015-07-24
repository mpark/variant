#pragma once

#include <functional>
#include <memory>
#include <type_traits>

#include <meta/meta.hpp>

namespace mpark {

  namespace detail {

    namespace variant {

      template <typename T>
      class ref_wrapper {
        public:

        static_assert(std::is_reference<T>{}, "");

        ref_wrapper(T ref) noexcept
            : ptr_(std::addressof(static_cast<T>(ref))) {}

        template <typename U = T,
                  typename = std::enable_if_t<std::is_lvalue_reference<U>{}>>
        ref_wrapper(std::remove_reference_t<T> &&) = delete;

        ~ref_wrapper() = default;

        ref_wrapper(const ref_wrapper &) = delete;
        ref_wrapper(ref_wrapper &&) = delete;
        ref_wrapper &operator=(const ref_wrapper &) = delete;
        ref_wrapper &operator=(ref_wrapper &&) = delete;

        operator T() const noexcept { return static_cast<T>(*ptr_); }

        private:

        std::remove_reference_t<T> *ptr_;

      };  // ref_wrapper

    }  // variant

  }  // detail

}  // mpark

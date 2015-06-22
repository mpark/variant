#pragma once

#include <utility>

#include <mpark/variant/detail/vtable.hpp>

namespace mpark {

  namespace detail {

    namespace variant {

      struct apply_impl {

        template <typename F, typename... Vs>
        decltype(auto) operator()(F &&f, Vs &&... vs) const {
          static constexpr auto vtable = make_vtable<F &&, Vs &&...>();
          return at(vtable, {vs.index_...})(std::forward<F>(f),
                                            std::forward<Vs>(vs)...);
        }

      };  // apply_impl

    }  // namespace variant

  }  // namespace detail

  /**
   *   `apply` takes a function: `F(P0, P1, ... PN)` followed by variants:
   *   `V0, V1, ..., VN` which invokes `F` with the actual value stored in each
   *   of the variants.
   *
   *   EXAMPLE:
   *
   *     variant<int, double, std::string> v = 42;
   *     ...
   *     mpark::apply([](const auto &x) { std::cout << x << std::endl; }, v);
   *
   **/
  template <typename F, typename... Vs>
  decltype(auto) apply(F &&f, Vs &&... vs) {
    return detail::variant::apply_impl()(std::forward<F>(f),
                                         std::forward<Vs>(vs)...);
  }

}  // namespace mpark

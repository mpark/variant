#pragma once

#include <utility>

#include <mpark/variant/detail/vtable.hpp>

namespace mpark {

  namespace detail {

    namespace variant {

      struct apply_impl {

        template <typename F, typename... Vs>
        static decltype(auto) apply(F &&f, Vs &&... vs) {
          static constexpr auto vtable = make_vtable<F &&, Vs &&...>();
          return at(vtable, {vs.index_...})(std::forward<F>(f),
                                            std::forward<Vs>(vs)...);
        }

        template <template <typename...> class F, typename Args, typename... Vs>
        static decltype(auto) apply(Args &&args, Vs &&... vs) {
          static constexpr auto vtable = make_vtable<F, Args &&, Vs &&...>();
          return at(vtable, {vs.index_...})(std::forward<Args>(args),
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
    using namespace detail::variant;
    return apply_impl::apply(std::forward<F>(f), std::forward<Vs>(vs)...);
  }

  template <template <typename...> class F, typename... Vs>
  decltype(auto) apply(Vs &&... vs) {
    using namespace detail::variant;
    return apply_impl::apply<F>(std::forward<Vs>(vs)...);
  }

}  // namespace mpark

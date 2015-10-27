#ifndef VARIANT_VISIT_HPP
#define VARIANT_VISIT_HPP

#include <algorithm>
#include <iterator>
#include <utility>

#include <variant/bad_variant_access.hpp>

#include <variant/detail/vtable.hpp>

namespace std {
namespace experimental {

template <typename F, typename... Vs>
decltype(auto) visit(F &&f, Vs &&... vs) {
  static constexpr auto vtable = detail::make_vtable<F &&, Vs &&...>();
  bool states[] = {vs.corrupted_by_exception()...};
  if (any_of(begin(states), end(states), [](bool b) { return b; })) {
    throw bad_variant_access{};
  }  // if
  return detail::at(vtable, {vs.index()...})(forward<F>(f), forward<Vs>(vs)...);
}

}  // namespace experimental
}  // namespace std

#endif  // VARIANT_VISIT_HPP

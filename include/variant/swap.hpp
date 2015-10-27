#ifndef VARIANT_SWAP_HPP
#define VARIANT_SWAP_HPP

namespace std {
namespace experimental {

//- 20.N.M swap support:

template <typename... Ts>
void swap(variant<Ts...> &lhs,
          variant<Ts...> &rhs) noexcept(noexcept(lhs.swap(rhs))) {
  lhs.swap(rhs);
}

}  // namespace experimental
}  // namespace std

#endif  // VARIANT_SWAP_HPP

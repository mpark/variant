# Variant [![experimental]](http://github.com/badges/stability-badges)

> A type-safe `union`

## NOTE

1. Is the hash of `variant<int, std::string>("hello")` required to be different
   from `variant<std::string, int>`?
1. How should `variant<>` behave?
1. `variant<int &>::operator=` is ill-formed. Does that mean
   `variant<int &>::variant(const variant<int &> &)` is also ill-formed?
1. Should `tuple_not_found` be a `constexpr size_t` variable? or
   a `std::integral_constant` type?
1. Fix `) constexpr {` for rel-ops of `monostate`.
1. `holds_alternative` is missing the `size_t` overload.
1. Is `tuple_count` missing?
1. `tuple_find` seems to be missing the `std::array` overload.
1. Should `monostate` be pulled out to its own header?

[experimental]: http://badges.github.io/stability-badges/dist/experimental.svg

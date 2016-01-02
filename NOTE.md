# Variant [![experimental]](http://github.com/badges/stability-badges)

> A type-safe `union`

## NOTE

1. Is the hash of `variant<int, std::string>("hello")` required to be different
   from `variant<std::string, int>`?
1. Questions regarding where the non-member functions should live:
   * Should the `variant` `get` functions go into `std::experimental`? or should
     they overload `std::get`?
   * Should the `variant` specializations of `tuple_size`, `tuple_element`, etc,
     specialize the versions in `std`? or should they be new classes in
     `std::experimental`?
   * Should non-member `swap` be defined in `std` or `std::experimental`?
1. How should `variant<>` behave?
1. `variant<int &>::operator=` is ill-formed. Does that mean
   `variant<int &>::variant(const variant<int &> &)` is also ill-formed?
1. Should `tuple_not_found` be a `constexpr` variable of type `size_t`? or
   a `std::integral_constant` type?
1. Fix `) constexpr {` for rel-ops of `monostate`.
1. `holds_alternative` is missing the `size_t` overload.
1. Should we also add a `tuple_count`?
1. `tuple_find` is missing the `std::array` overload.
1. `monostate` should be pulled out to its own header?

[experimental]: http://badges.github.io/stability-badges/dist/experimental.svg

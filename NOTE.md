# Variant [![experimental]](http://github.com/badges/stability-badges)

> A type-safe `union`

## NOTE

1. Is the hash of `variant<int, std::string>("hello")` required to be different
   from `variant<std::string, int>`?
2. Questions regarding where the non-member functions should live:
   * Should the `variant` `get` functions go into `std::experimental`? or should
     they overload `std::get`?
   * Should the `variant` specializations of `tuple_size`, `tuple_element`, etc,
     specialize the versions in `std`? or should they be new classes in
     `std::experimental`?
   * Should non-member `swap` be defined in `std` or `std::experimental`?
3. How should `variant<>` behave?
4. `variant<int &>::operator=` is ill-formed. Does that mean
   `variant<int &>::variant(const variant<int &> &)` is also ill-formed?
5. If the `that` in `variant(const variant &that)` is corrupted, does the copy
   constructor throw? or is `*this` also corrupted?
6. Should `tuple_not_found` be `constexpr size_t`? or `std::integral_constant`?
7. Fix `) constexpr {` for rel-ops of `monostate`.

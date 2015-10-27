# Variant [![experimental]](http://github.com/badges/stability-badges)

> A type-safe `union`

## TODO

1. Determine the functions that need to be tested for `corrupted_by_exception`
   before visiting them (copy constructor, `swap`, etc).
2. Determine the places where `meta::count<members, T>{} == 1` need to be
   `static_assert` vs SFINAE.
4. Add `allocator` support.
3. Add proper `constexpr` support.

[experimental]: http://badges.github.io/stability-badges/dist/experimental.svg

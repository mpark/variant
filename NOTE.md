# Variant [![experimental]](http://github.com/badges/stability-badges)

> A type-safe `union`

## NOTE

1. Is the hash of `variant<int, std::string>("hello")` required to be different
   from `variant<std::string, int>`?
1. How should `variant<>` behave?
1. `variant<int &>::operator=` is ill-formed. Does that mean
   `variant<int &>::variant(const variant<int &> &)` is also ill-formed?
1. Is `tuple_count` missing?

recursive union: https://goo.gl/yjEzta
union          : https://goo.gl/3OOhwC


[experimental]: http://badges.github.io/stability-badges/dist/experimental.svg

# MPark.Variant [![experimental]](http://github.com/badges/stability-badges)

[experimental]: http://badges.github.io/stability-badges/dist/experimental.svg

> An experimental library aimed to provide a sum type with value semantics for
> Modern C++.

## Introduction

The `variant` class template is a sum type with value semantics. It aims to
capture the reference type nature of a class inheritance hierarchy into a value
type. This leads to subtle differences compared to a discriminated union model.

### Motivation

While C++ is very much in favor of providing value semantics by default, there
is little support for sum types with value semantics. All the love seem to
have gone into product types such as `class`, `std::array`, and `std::tuple`.

The `union` feature was inherited from C and was only supported for POD types
in C++98. In C++11, "unrestricted unions" were proposed in [N2544] which
allowed non-POD types as a variant member of `union`. Even without this
restriction however, the fact that `union` is not discriminated makes it a
difficult and error-prone feature to use.

Inheritance is another feature that provides sum types in C++. The caveat here
is that inheritance by nature provides reference semantics due to object
slicing. In practice, we often see code that uses inheritance merely for
polymorphism rather than to actually represent inheritance relationships.

Finally, the strongest motivation for this library in particular is to explore
different ways we could introduce a `std::variant`. [N4542] is the latest state
of the `std::variant` proposal which is shaping up, but I'd like to present my
ideas and opinions regarding this topic at the next ISO C++ Standards meeting.

[N2544]: http://www.open-std.org/jtc1/sc22/wg21/docs/papers/2008/n2544.pdf

## Comparison with [N4542]

There are a few notable differences between __MPark.Variant__ and the
`variant` being proposed in [N4542] most of which derive from the difference
in their respective conceptual model.

### The Conceptual Model

The conceptual model for [N4542] is a discriminated union of types `Ts...`.
This modelling is apparent from some of its characteristics:

1. `variant<Ts...>`'s default-constructor default-constructs the first type in
   `Ts...` if the first type is default-constructible. This behavior is
   inherited from `union`. This leads to the significance of the order in which
   types appear in `Ts...`.
1. Duplicate types in `Ts...` are not only allowed, but represent distinct
   states. Each can be referred to explicitly with their index. The only
   difference being that with `union`, each alternative would simply have a
   name for them rather than an index.

__[Boost.Variant]__ also uses the discriminated union as its conceptual model and
its [abstract] reads:

[abstract]: http://www.boost.org/doc/libs/1_58_0/doc/html/variant.html#variant.abstract

> Whereas standard containers such as `std::vector` may be thought of as
> "__multi-value, single type__," `variant` is "__multi-type, single value__."

The conceptual model for __MPark.Variant__ is closer to a class inheritance
hierarchy collapsed into a value type. I believe that related libraries such as
`any` and `optional<T>` represent the value type correspondences of `void *` and
`T *` respectively. Extending this notion, `variant<Ts...>` represents the value
type correspondence of `B *` (where each of `Ts...` inherits from `B`).

The following table is a summary of the mapping.

| Reference Semantics                       | Value Semantics        |
|-------------------------------------------|------------------------|
| `void *`                                  | `any`                  |
| `T *`                                     | `optional<T>`          |
| `B *` (each of `Ts...` inherits from `B`) | `variant<Ts...>`       |

### Order of the Types in `Ts...`

In type theory, the order in which the types appear in a sum type has no effect.
That is, `variant<T, U>` should behave exactly the same as `variant<U, T>`.
This is not the case for [N4542], and the reason is due to the fact that its
conceptual model is the discriminated union. Since __MPark.Variant__ aims to
provide a sum type with value semantics, the behavior of `variant<T, U>` and
`variant<U, T>` are equivalent.

The following are a few of the implications that fall out from this feature.

__Implication 1__: __MPark.Variant__ does not try to default-construct the
                   first type.

In terms of its usability, the argument I've heard is that it becomes difficult
to use inside containers. I think people are primarily concerned about
containers such as `std::array`, `std::map` and maybe `std::vector`.

For `std::map<Key, Value>`, `operator[]` requires the `Value` to be
default-constructible. We can just as easily use functions such as `at`,
`insert` and `emplace` to accomplish the same goals.

For `std::vector<T>`, the `vector(size_t count)` constructor and the
`resize(size_t count)` function requires default-constructible.
In general, I think `reserve` should be preferred anyway. Both of them also
have an alternative overload that takes `size_t count` and `const T &` as its
parameters.

`std::array` is the annoying one, because it does not let you provide a custom
value to initialize with. In this case, the simplest option would be to use a
`std::vector` instead.

__Implication 2__: __MPark.Variant__ does not provide index-based operations.

A few of the index-based operations provided in [N4542] include the `index()`
function which return the index of the current type being held, the `get<Index>`
interface for element access, and the `emplaced_index_t<Index>` tag available
for index-based in-place construction. __MPark.Variant__ provide only the type
correspondence of each: `type()`, `get<Type>`, and `in_place_t<Type>`.

__Implication 3__: While multiple occurrences of a type are allowed in `Ts...`,
                   they do not represent distinct states.

This means that `variant<int, int>` is behaves equivalently to `variant<int>`.

### The _Empty_ State

[N4542] describes its "empty" state as a valid, but unspecified state.
This state only arises if `Ti`'s move constructor throws during its construction
within the `variant`, and the state is visible via the `valid()` function.
Out of the solutions I considered myself, this is the one that I decided to
adopt for __MPark.Variant__ as well. But I would like to straighten out the
terminology a little.

For example, if `valid()` returns `false`, I would expect that the `variant` is
in an __invalid__ state. Surprisingly, it's actually in a __valid__, but
unspecified state. Well, that's kind of confusing. However, since the `variant`
in such state is practivally invalid, for now let's call this state: _invalid_.

The bigger point I want to make here is that this _invalid_ state should be
clearly distinguished from the _empty_ state. The _empty_ state is actually a
valid, and __specified__ state that represents the emptiness of a `variant`.

With the above terminology in place, [N4542] always has the _invalid_ state, and
provides the ability to opt-in to have the _empty_ state with `monostate`.

__MPark.Variant__ also always has the _invalid_ state, and provides the ability
to opt-in to have the _empty_ state with `null_t`. `null_t` has special
behavior contrary to `monostate` from [N4542]. Since an emptiable variant should
be default-constructible into its _empty_ state, and the order of types appearing
in `Ts...` does not have significance, the presence of `null_t` anywhere in
`Ts...` enables default-construction and contextual conversion to `bool`.

__NOTE__: The naming choice of `null_t` and the rationale for providing the
          contextual conversion to `bool` largely came from `std::optional`'s
          `nullopt_t` and the rationale given in [N3672] respectively.

[N3672]:http://www.open-std.org/jtc1/sc22/wg21/docs/papers/2013/n3672.html#rationale.bool_conversion

This approach of opting in for the _empty_ state is what [Jason Lucas] and I
took when we put together the [Polymorphism with Unions] presentation for
CppCon14. It was mentioned in [N4450] and was claimed that it adds complexity
to an otherwise simple type.

[Jason Lucas]: https://github.com/JasonL9000
[Polymorphism with Unions]: https://www.youtube.com/watch?v=uii2AfiMA0o
[N4450]: http://www.open-std.org/jtc1/sc22/wg21/docs/papers/2015/n4450.pdf

### The Matrix of Differences

| Aspect                    | N4542                | MPark.Variant               |
|---------------------------|----------------------|-----------------------------|
| Default Construction      | First `T` in `Ts...` | Yes, if `null_t` in `Ts...` |
| `index()`                 | Yes                  | No                          |
| `get<Index>`              | Yes                  | No                          |
| Index In-place Tag        | Yes                  | No                          |
| Name of Type In-place Tag | `emplaced_type_t`    | `in_place_t`                |
| Duplicate Types           | Yes, distinct states | Yes, equivalent state       |
| Name of the Empty State   | `monostate`          | `null_t`                    |
| Conversions               | No                   | Yes                         |
| Heterogeneous Assignment  | No                   | Yes                         |

## Requirements

This library requires a standard conformant __C++14__ compiler.

The following configurations were successfully tested:

* __Mac OS X Yosemite__: Clang 3.5, 3.6, _Apple_ (with libc++)
* __Ubuntu 14.04.2__: Clang 3.5, 3.6 (with libstdc++-5),
                      GCC is currently not supported.

## Related Work

* __[N4542]__ by Axel Naumann
* __[Boost.Variant]__ by Eric Friedman and Itay Maman
* __[Eggs.Variant]__ by Agustín Bergé (a.k.a K-ballo)

[N4542]: http://www.open-std.org/jtc1/sc22/wg21/docs/papers/2015/n4542.pdf
[Boost.Variant]: http://www.boost.org/doc/libs/1_58_0/doc/html/variant.html
[Eggs.Variant]: http://eggs-cpp.github.io/variant/

## Future Work

* Support for `get<T>(&v);`.
* Specify correct `noexcept` specifications.
* Full `constexpr` support.
* Support for references in types.
* Implement relational operators.

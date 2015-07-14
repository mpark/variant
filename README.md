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
ideas regarding this topic at the next ISO C++ meeting.

[N2544]: http://www.open-std.org/jtc1/sc22/wg21/docs/papers/2008/n2544.pdf
[N4542]: http://www.open-std.org/jtc1/sc22/wg21/docs/papers/2015/n4542.pdf

## The Conceptual Model

The conceptual model for [N4542] is a discriminated union of types `Ts...`.
From `Boost.Variant`'s [abstract]:

[abstract]: http://www.boost.org/doc/libs/1_58_0/doc/html/variant.html#variant.abstract

> Whereas standard containers such as `std::vector` may be thought of as
> "__multi-value, single type__," `variant` is "__multi-type, single value__."

The conceptual model for `MPark.Variant` is closer to a class inheritance
hierarchy collapsed into a value type. Libraries such as `any` and `optional`
effectively provide value semantics for various language features which provide
reference semantics due to its pointer nature.

The following table is a summary of the mapping.

| Reference Semantics                       | Value Semantics        |
|-------------------------------------------|------------------------|
| `void *`                                  | `any`                  |
| `T *`                                     | `optional<T>`          |
| `B *` (each of `Ts...` inherits from `B`) | `variant<Ts...>`       |

## Comparison with [N4542]

| Aspect                     | N4542                | MPark.Variant                          |
|----------------------------|----------------------|----------------------------------------|
| Default Construction       | First `T` in `Ts...` | `monostate`, if `monostate` in `Ts...` |
| Conversions                | No                   | Yes                                    |
| Heterogeneous Assignment   | No                   | Yes                                    |
| Duplicate Types            | Yes                  | No                                     |
| Name of Type In-place Tag  | `emplaced_type_t`    | `in_place_t`                           |
| Index In-place Tag         | Yes                  | No                                     |
| `index()`                  | Yes                  | No                                     |
| `get<Index>`               | Yes                  | No                                     |

## The Empty State

The name of the empty state is `monostate` which is the same as what seem to
have reached consensus in [N4542]. However, the user of `variant` explicitly
__opts in__ for an __empty__ `variant` by including the `monostate` type in
the template parameter list of `variant`. This is the approach [Jason Lucas]
and I took when we put together for the [Polymorphism with Unions] presentation
for CppCon14.

[Jason Lucas]: https://github.com/JasonL9000
[Polymorphism with Unions]: https://www.youtube.com/watch?v=uii2AfiMA0o

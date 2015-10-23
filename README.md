# Variant [![experimental]](http://github.com/badges/stability-badges)

> Discriminated Union with Value Semantics

The reference implementation of proposal [P0080].

`variant` reached consensus in Kona! I'll be updating this implementation
shortly for the Kona `variant`.

## Quick Start

Since this project uses `git submodule`, specifying the `--recursive` flag to
`git clone` is recommended.

```bash
git clone --recursive git@github.com:mpark/variant.git
```

## Requirements

This library requires a standard conformant __C++14__ compiler.

Tested configurations:
  * __Mac OS X Yosemite__: `clang-3.6+`

[experimental]: http://badges.github.io/stability-badges/dist/experimental.svg
[P0080]: http://www.open-std.org/jtc1/sc22/wg21/docs/papers/2015/p0080r0.pdf

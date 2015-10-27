# Variant [![experimental]](http://github.com/badges/stability-badges)

> A type-safe `union`

## Introduction

`variant` reached a design consensus at the fall ISO C++ committee meeting in
Kona, HI, USA. While the design is still not final, this library is a reference
implementation of [P0088R0].

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
[P0088R0]: http://www.open-std.org/jtc1/sc22/wg21/docs/papers/2015/p0088r0.pdf

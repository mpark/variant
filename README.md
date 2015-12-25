# Variant

[![Stability](https://img.shields.io/badge/stability-experimental-red.svg)](http://github.com/badges/stability-badges)
[![Build Status](https://travis-ci.org/mpark/variant.svg?branch=master)](https://travis-ci.org/mpark/variant)
[![License](http://img.shields.io/badge/license-boost-blue.svg)](https://raw.githubusercontent.com/mpark/variant/master/LICENSE_1_0.txt)
[![Gitter](https://badges.gitter.im/mpark/variant.svg)](https://gitter.im/mpark/variant?utm_source=badge&utm_medium=badge&utm_campaign=pr-badge)

> A type-safe `union`

## Introduction

`variant` reached a design consensus at the fall ISO C++ committee meeting in Kona, HI, USA.
While the design is still not final, this library is an experimental reference implementation of [P0088R0].

## Quick Start

Since this project uses `git submodule`, specifying the `--recursive` flag to `git clone` is recommended.

```bash
git clone --recursive https://github.com/mpark/variant.git
```

## Requirements

This library requires a standard conformant __C++14__ compiler.

Tested configurations:
  * __Ubuntu 14.04 Trusty__: `GCC 5`, `Clang 3.5+`
  * __Mac OS X Yosemite__: `Apple Clang`, `Clang 3.5+`

[P0088R0]: http://www.open-std.org/jtc1/sc22/wg21/docs/papers/2015/p0088r0.pdf

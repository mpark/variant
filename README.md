# MPark.Variant

> __C++14__ implementation of __C++17__ `std::variant`

[![stability][badge.stability]][stability]
[![travis][badge.travis]][travis]
[![license][badge.license]][license]
[![gitter][badge.gitter]][gitter]
[![wandbox][badge.wandbox]][wandbox]

[badge.stability]: https://img.shields.io/badge/stability-stable-brightgreen.svg
[badge.travis]: https://travis-ci.org/mpark/variant.svg?branch=master
[badge.license]: http://img.shields.io/badge/license-boost-blue.svg
[badge.gitter]: https://badges.gitter.im/mpark/variant.svg
[badge.wandbox]: https://img.shields.io/badge/try%20it-on%20wandbox-green.svg

[stability]: http://github.com/badges/stability-badges
[travis]: https://travis-ci.org/mpark/variant
[license]: https://github.com/mpark/variant/blob/master/LICENSE_1_0.txt
[gitter]: https://gitter.im/mpark/variant
[wandbox]: http://melpon.org/wandbox/permlink/opRHkFzHSFX5KmiB

## Introduction

__MPark.Variant__ provides an implementation of __C++17__ `std::variant` as a __C++14__ library.

The implementation is based on my [implementation of `std::variant` for __libc++__][libcxx-impl]
and is continously tested against __libc++__'s `std::variant` test suite.

## Documentation

Refer to [`std::variant` - cppreference.com][cppreference] for the `std::variant`
components of __MPark.Variant__.

[cppreference]: http://en.cppreference.com/w/cpp/utility/variant

## CMake Variables

  -  __`MPARK_VARIANT_INCLUDE_TESTS`__:`STRING` (__default__: `""`)

      Semicolon-separated list of tests to build. Possible values are `mpark` and `libc++`.

      __NOTE__: The __libc++__ tests are built with `-std=c++1z`.

## Requirements

This library requires a standard conformant __C++14__ compiler.
The following compilers are continously tested:

  - `g++-5.4`
  - `g++-6.2`
  - `clang++-3.5`
  - `clang++-3.6`
  - `clang++-3.7`
  - `clang++-3.8`
  - `clang++-3.9`
  - `apple-clang-6`
  - `apple-clang-7`
  - `apple-clang-8`

__NOTE__: Enabling __libc++__'s `std::variant` tests require `-std=c++1z` support.

## Unit Tests

Refer to [test/README.md](test/README.md).

[libcxx-impl]: https://reviews.llvm.org/rL288547

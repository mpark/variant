# MPark.Variant

> __C++14__ implementation of __C++17__ `std::variant`

[![stability][badge.stability]][stability]
[![travis][badge.travis]][travis]
[![appveyor][badge.appveyor]][appveyor]
[![license][badge.license]][license]
[![gitter][badge.gitter]][gitter]
[![wandbox][badge.wandbox]][wandbox]

[badge.stability]: https://img.shields.io/badge/stability-stable-brightgreen.svg
[badge.travis]: https://travis-ci.org/mpark/variant.svg?branch=master
[badge.appveyor]: https://ci.appveyor.com/api/projects/status/github/mpark/variant?branch=master&svg=true
[badge.license]: http://img.shields.io/badge/license-boost-blue.svg
[badge.gitter]: https://badges.gitter.im/mpark/variant.svg
[badge.wandbox]: https://img.shields.io/badge/try%20it-on%20wandbox-green.svg

[stability]: http://github.com/badges/stability-badges
[travis]: https://travis-ci.org/mpark/variant
[appveyor]: https://ci.appveyor.com/project/mpark/variant
[license]: https://github.com/mpark/variant/blob/master/LICENSE_1_0.txt
[gitter]: https://gitter.im/mpark/variant
[wandbox]: http://melpon.org/wandbox/permlink/pKlAz3y3jif2HCDi

## Introduction

__MPark.Variant__ provides an implementation of __C++17__ `std::variant` as a __C++14__ library.

The implementation is based on my [implementation of `std::variant` for __libc++__][libcxx-impl]
and is continously tested against __libc++__ `std::variant` test suite.

## Documentation

Refer to [`std::variant` - cppreference.com][cppreference] for the `std::variant`
components of __MPark.Variant__.

[cppreference]: http://en.cppreference.com/w/cpp/utility/variant

## CMake Variables

  -  __`MPARK_VARIANT_INCLUDE_TESTS`__:`STRING` (__default__: `""`)

      Semicolon-separated list of tests to build. Possible values are `mpark` and `libc++`.

      __NOTE__: The __libc++__ `std::variant` tests are built with `-std=c++1z`.

## Requirements

This library requires a standard conformant __C++14__ compiler.
The following compilers are continously tested:

  - `GCC 5.4`
  - `GCC 6.2`
  - `Clang 3.5`
  - `Clang 3.6`
  - `Clang 3.7`
  - `Clang 3.8`
  - `Clang 3.9`
  - `Apple Clang 6`
  - `Apple Clang 7`
  - `Apple Clang 8`
  - `Visual Studio 2015` with `Clang/LLVM`

__NOTE__: Enabling __libc++__ `std::variant` tests require `-std=c++1z` support.

## Unit Tests

Refer to [test/README.md](test/README.md).

[libcxx-impl]: https://reviews.llvm.org/rL288547

# MPark.Variant

> __C++17__ `std::variant` for __C++11__/__14__/__17__

[![stability][badge.stability]][stability]
[![travis][badge.travis]][travis]
[![appveyor][badge.appveyor]][appveyor]
[![license][badge.license]][license]
[![gitter][badge.gitter]][gitter]
[![godbolt][badge.godbolt]][godbolt]
[![wandbox][badge.wandbox]][wandbox]

[badge.stability]: https://img.shields.io/badge/stability-stable-brightgreen.svg
[badge.travis]: https://travis-ci.org/mpark/variant.svg?branch=master
[badge.appveyor]: https://ci.appveyor.com/api/projects/status/github/mpark/variant?branch=master&svg=true
[badge.license]: http://img.shields.io/badge/license-boost-blue.svg
[badge.gitter]: https://badges.gitter.im/mpark/variant.svg
[badge.godbolt]: https://img.shields.io/badge/try%20it-on%20godbolt-222266.svg
[badge.wandbox]: https://img.shields.io/badge/try%20it-on%20wandbox-5cb85c.svg

[stability]: http://github.com/badges/stability-badges
[travis]: https://travis-ci.org/mpark/variant
[appveyor]: https://ci.appveyor.com/project/mpark/variant
[license]: https://github.com/mpark/variant/blob/master/LICENSE.md
[gitter]: https://gitter.im/mpark/variant
[godbolt]: https://godbolt.org/g/DceDpT
[wandbox]: https://wandbox.org/permlink/QuDs9wncQPljHkp2

## Introduction

__MPark.Variant__ provides an implementation of __C++17__ `std::variant` for __C++11__/__14__/__17__.

  - Based on [my implementation of `std::variant` for __libc++__][libcxx-impl]
  - Continuously tested against __libc++__'s `std::variant` test suite.

[libcxx-impl]: https://reviews.llvm.org/rL288547

## Integration

### Single-Header

This branch provides a standalone `variant.hpp` file for each [release].
Copy it and `#include` away!

[single-header]: https://github.com/mpark/variant/tree/single-header
[release]: https://github.com/mpark/variant/releases

## License

Distributed under the [Boost Software License, Version 1.0](LICENSE.md).

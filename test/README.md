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

## Test

This directory contains the tests for __MPark.Variant__.

### Building and Running Tests

Execute the following commands from the top-level directory:

```bash
mkdir build
cd build
cmake -DMPARK_VARIANT_INCLUDE_TESTS="mpark;libc++" ..
cmake --build .
ctest -V
```

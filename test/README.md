# MPark.Variant

> __C++17__ `std::variant` for __C++11__/__14__/__17__

[![release][badge.release]][release]
[![travis][badge.travis]][travis]
[![appveyor][badge.appveyor]][appveyor]
[![license][badge.license]][license]
[![godbolt][badge.godbolt]][godbolt]
[![wandbox][badge.wandbox]][wandbox]

[badge.release]: https://img.shields.io/github/release/mpark/variant.svg
[badge.travis]: https://travis-ci.org/mpark/variant.svg?branch=master
[badge.appveyor]: https://ci.appveyor.com/api/projects/status/github/mpark/variant?branch=master&svg=true
[badge.license]: https://img.shields.io/badge/license-boost-blue.svg
[badge.godbolt]: https://img.shields.io/badge/try%20it-on%20godbolt-222266.svg
[badge.wandbox]: https://img.shields.io/badge/try%20it-on%20wandbox-5cb85c.svg

[release]: https://github.com/mpark/variant/releases/latest
[travis]: https://travis-ci.org/mpark/variant
[appveyor]: https://ci.appveyor.com/project/mpark/variant
[license]: https://github.com/mpark/variant/blob/master/LICENSE.md
[godbolt]: https://godbolt.org/g/NJ1nh4
[wandbox]: https://wandbox.org/permlink/DKO6yFxjr9ejei8m

## Test

This directory contains the tests for __MPark.Variant__.

## CMake Variables

  -  __`MPARK_VARIANT_CXX_STANDARD`__:`STRING` (__default__: `14`)

     The C++ standard with which to build the tests.

     __NOTE__: The __libc++__ `std::variant` tests are built with `-std=c++17`.

  -  __`MPARK_VARIANT_EXCEPTIONS`__:`BOOL` (__default__: `ON`)

     Build the tests with exceptions support.

## Build / Run

Execute the following commands from the top-level directory:

```bash
mkdir build
cd build
cmake -DMPARK_VARIANT_INCLUDE_TESTS="mpark;libc++" ..
cmake --build .
ctest -V
```

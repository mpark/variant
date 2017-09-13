# MPark.Variant

> __C++17__ `std::variant` for __C++11__/__14__/__17__

[![version][badge.version]][version]
[![travis][badge.travis]][travis]
[![appveyor][badge.appveyor]][appveyor]
[![license][badge.license]][license]
[![godbolt][badge.godbolt]][godbolt]
[![wandbox][badge.wandbox]][wandbox]

[badge.version]: https://img.shields.io/github/release/mpark/variant.svg
[badge.travis]: https://travis-ci.org/mpark/variant.svg?branch=master
[badge.appveyor]: https://ci.appveyor.com/api/projects/status/github/mpark/variant?branch=master&svg=true
[badge.license]: https://img.shields.io/badge/license-boost-blue.svg
[badge.godbolt]: https://img.shields.io/badge/try%20it-on%20godbolt-222266.svg
[badge.wandbox]: https://img.shields.io/badge/try%20it-on%20wandbox-5cb85c.svg

[version]: https://github.com/mpark/variant/releases/latest
[travis]: https://travis-ci.org/mpark/variant
[appveyor]: https://ci.appveyor.com/project/mpark/variant
[license]: https://github.com/mpark/variant/blob/master/LICENSE.md
[godbolt]: https://godbolt.org/g/G4dSbr
[wandbox]: https://wandbox.org/permlink/glzF6r0NtWRY20Np

## Test

This directory contains the tests for __MPark.Variant__.

## CMake Variables

  -  __`MPARK_VARIANT_CXX_STANDARD`__:`STRING` (__default__: `14`)

     The C++ standard with which to build the tests.

     __NOTE__: The __libc++__ `std::variant` tests are built with `-std=c++1z`.

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

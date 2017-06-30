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

## Documentation

  - [cppreference.com](http://en.cppreference.com/w/cpp/utility/variant)
  - [eel.is/c++draft](http://eel.is/c++draft/variant)

## Integration

### Single-Header

The [single-header] branch provides a standalone `variant.hpp`
file for each [release]. Copy it and `#include` away!

[single-header]: https://github.com/mpark/variant/tree/single-header
[release]: https://github.com/mpark/variant/releases

### Submodule

You can add `mpark/variant` as a submodule to your project.

```bash
git submodule add https://github.com/mpark/variant.git
```

Add the `include` directory to your include path with `-Ivariant/include` then
include the `variant.hpp` header with `#include <mpark/variant.hpp>`.
Relative path includes (e.g., `#include "variant/include/mpark/variant.hpp"`)
are also supported which enables other libraries to depend on `mpark/variant`.

If you use CMake, you can simply use `add_subdirectory(variant)`:

```
cmake_minimum_required(VERSION 3.6.3)

project(HelloWorld CXX)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_subdirectory(variant)

add_executable(hello-world hello_world.cpp)
target_link_libraries(hello-world mpark_variant)
```

### Installation & CMake `find_package`

```bash
git clone https://github.com/mpark/variant.git
mkdir variant/build && cd variant/build
cmake ..
cmake --build . --target install
```

This will install `mpark/variant` to the default install-directory for
your platform (`/usr/local` for Unix, `C:\Program Files` for Windows).
You can also install at a custom location via the `CMAKE_INSTALL_PREFIX`
variable, (e.g., `cmake .. -DCMAKE_INSTALL_PREFIX=/opt`).

The installed `mpark/variant` can then be found by CMake via `find_package`:

```
cmake_minimum_required(VERSION 3.6.3)

project(HelloWorld CXX)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

find_package(mpark_variant 1.2.0 REQUIRED)

add_executable(hello-world hello_world.cpp)
target_link_libraries(hello-world mpark_variant)
```

CMake will search for `mpark/variant` in its default set of
installation prefixes. If `mpark/variant` is installed in
a custom location via the `CMAKE_INSTALL_PREFIX` variable,
you'll likely need to use the `CMAKE_PREFIX_PATH` to specify
the location (e.g., `cmake .. -DCMAKE_PREFIX_PATH=/opt`).

## Requirements

This library requires a standard conformant __C++11__ compiler.
The following compilers are continously tested:

| Compiler                               | Operating System                            | Version String                                                                          |
|----------------------------------------|---------------------------------------------|-----------------------------------------------------------------------------------------|
| GCC 4.9.4                              | Ubuntu 14.04.5 LTS                          | g++-4.9 (Ubuntu 4.9.4-2ubuntu1~14.04.1) 4.9.4                                           |
| GCC 5.4.1                              | Ubuntu 14.04.5 LTS                          | g++-5 (Ubuntu 5.4.1-2ubuntu1~14.04) 5.4.1 20160904                                      |
| GCC 6.2.0                              | Ubuntu 14.04.5 LTS                          | g++-6 (Ubuntu 6.2.0-3ubuntu11~14.04) 6.2.0 20160901                                     |
| Clang 3.5.2                            | Ubuntu 14.04.5 LTS                          | Ubuntu clang version 3.5.2-svn232544-1~exp1 (branches/release_35) (based on LLVM 3.5.2) |
| Clang 3.6.2                            | Ubuntu 14.04.5 LTS                          | Ubuntu clang version 3.6.2-svn240577-1~exp1 (branches/release_36) (based on LLVM 3.6.2) |
| Clang 3.7.1                            | Ubuntu 14.04.5 LTS                          | Ubuntu clang version 3.7.1-svn253742-1~exp1 (branches/release_37) (based on LLVM 3.7.1) |
| Clang 3.8.0                            | Ubuntu 14.04.5 LTS                          | clang version 3.8.0-2ubuntu3~trusty4 (tags/RELEASE_380/final)                           |
| Clang 3.9.1                            | Ubuntu 14.04.5 LTS                          | clang version 3.9.1-svn288847-1~exp1 (branches/release_39)                              |
| Clang Xcode 6.4                        | Darwin Kernel Version 14.5.0 (OS X 10.10.3) | Apple LLVM version 6.1.0 (clang-602.0.53) (based on LLVM 3.6.0svn)                      |
| Clang Xcode 7.3                        | Darwin Kernel Version 15.6.0 (OS X 10.10.5) | Apple LLVM version 7.3.0 (clang-703.0.31)                                               |
| Clang Xcode 8.2                        | Darwin Kernel Version 16.1.0 (OS X 10.12.1) | Apple LLVM version 8.0.0 (clang-800.0.42.1)                                             |
| Visual Studio 14 2015                  | Visual Studio 2015 with Update 3            | MSVC 19.00.24215.1 | Microsoft (R) Build Engine version 14.0.25420.1                    |
| Visual Studio 14 2017                  | Visual Studio 2017                          | MSVC 19.10.25019.0 | Microsoft (R) Build Engine version 15.1.1012.6693                  |
| Visual Studio 14 2015 (__Clang/LLVM__) | Visual Studio 2015 with Update 3            | Clang 4.0.0        | Microsoft (R) Build Engine version 14.0.25420.1                    |

#### NOTES
  - __GCC 4.9__: `constexpr` support is not available for `visit` and relational operators.
  - Enabling __libc++__ `std::variant` tests require `-std=c++1z` support.

## CMake Variables

  -  __`MPARK_VARIANT_INCLUDE_TESTS`__:`STRING` (__default__: `""`)

     Semicolon-separated list of tests to build.
     Possible values are `mpark`, and `libc++`.

     __NOTE__: The __libc++__ `std::variant` tests are built with `-std=c++1z`.

## Unit Tests

Refer to [test/README.md](test/README.md).

[libcxx-impl]: https://reviews.llvm.org/rL288547

## License

Distributed under the [Boost Software License, Version 1.0](LICENSE.md).

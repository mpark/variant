#pragma once

#include <meta/meta.hpp>

#define RETURN(...) -> decltype(__VA_ARGS__) { return __VA_ARGS__; }

#define RETURN_TYPE(type, ...) \
  -> meta::_t<decltype(__VA_ARGS__, meta::id<type>{})> { return __VA_ARGS__; }

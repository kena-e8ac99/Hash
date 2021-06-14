# Hash

C++ 20, Non dependency, header only compile-time hash calculation library.

# Note

- This library supports types that meet one of the following requirements. If you use unsupported types, a compile error will occur.
  1. trivally copyable type and satisfies the following.
     - is not a union type;
     - is not a pointer type;
     - is not a pointer to member type;
     - is not a volatile-qualified type; and
     - has no non-static data member of reference type.
  2. input range, pair, tuple, or variant type whose value_type or element_type satisfies the above item or this item.

- The hash value is the same as libstdc++ for the type that satisfies the first item above other than input range, or the input range whose value_type is 1 byte.
- Supported types use values instead of adresses to calculate hashes.

# Usage

```cpp
#include "hash/hash.hpp"

using namespace std::literals;

constexpr auto view_text   = "Hello World!"sv;

constexpr auto char_text   = "Hello World!";

constexpr auto view_result = reki::hash<std::string_view>{}(view_text);

constexpr auto char_result = reki::hash<const char*>{}(char_text);

static_assert(view_result == char_result);

assert(view_result == std::hash<std::string_view>{}(view_text));
```

# Tested Compiler

- GCC   11.1.0
- Clang 12.0.0

# License

[MIT License](https://en.wikipedia.org/wiki/MIT_License)


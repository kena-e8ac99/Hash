#pragma once

#include <concepts>
#include <type_traits>

namespace reki::detail
{
  template <typename T>
  concept available_as_bytes
  = (sizeof(T) <= sizeof(std::size_t))     &&
    (sizeof(std::size_t) % sizeof(T) == 0) &&
    (std::convertible_to<T, std::size_t>   ||
     std::convertible_to<std::underlying_type_t<T>, std::size_t>);

  // NOTE:
  // Although not written in the code, this concept requires
  // "has no non-static data member of reference type".
  // Compile error will occur if a type that does'nt meet the above requirement.
  template <typename T>
  concept consteval_bit_castable
  = std::is_trivially_copyable_v<T> &&
    (!std::is_union_v<T> && !std::is_pointer_v<T> &&
     !std::is_member_pointer_v<T> && !std::is_volatile_v<T>);

  template <typename T>
  concept character
  = std::same_as<T, char>     || std::same_as<T, char8_t>     ||
    std::same_as<T, char16_t> || std::same_as<T, char32_t>    ||
    std::same_as<T, wchar_t>  || std::same_as<T, signed char> ||
    std::same_as<T, unsigned char>;
}

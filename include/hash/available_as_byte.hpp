#pragma once

#include <concepts>
#include <limits>
#include <type_traits>

namespace reki::detail
{
  template <typename T>
  concept available_as_byte
  = (sizeof(T) == 1) && std::integral<T>  &&
    std::convertible_to<T, unsigned char> &&
    (std::numeric_limits<T>::max() - std::numeric_limits<T>::min() == 255);
}

namespace reki
{
  template <typename T>
  concept available_as_byte
  = detail::available_as_byte<T> ||
    detail::available_as_byte<std::underlying_type_t<T>>;
}

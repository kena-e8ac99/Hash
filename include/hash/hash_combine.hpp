#pragma once

#include <cstddef>

namespace reki
{
  constexpr void hash_combine(std::size_t& seed, std::size_t value) noexcept
  {
    if constexpr (sizeof(std::size_t) == 4)
    {
      seed ^= value + 0x9e3779b9 + (seed << 6) + (seed >> 2);
    }
    else
    {
      seed ^= value + 0x9e3779b97f4a7c15LLU + (seed << 12) + (seed >> 4);
    }
  }
}

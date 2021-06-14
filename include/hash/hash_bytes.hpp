#pragma once

#include <algorithm>
#include <array>
#include <bit>
#include <span>
#include <type_traits>

#include "hash/hash_concepts.hpp"
#include "hash/detail/hash_bytes.hpp"

namespace reki
{
  // Murmur hash
  template <consteval_bit_castable T>
  constexpr std::size_t hash_bytes(const T&    value,
                                   std::size_t seed = 0xc70f6907UL)
  {
    const auto bytes = std::bit_cast<std::array<std::byte, sizeof(T)>>(value);

    return detail::hash_bytes(std::span{bytes}, std::move(seed));
  }

  template <available_as_byte T, std::size_t N = std::dynamic_extent>
  constexpr std::size_t hash_bytes(std::span<const T, N> value,
                                   std::size_t           seed = 0xc70f6907UL)
  {
    return detail::hash_bytes(std::move(value), std::move(seed));
  }

  template <consteval_bit_castable T, std::size_t N = std::dynamic_extent>
  requires (!available_as_byte<T>)
  constexpr std::size_t hash_bytes(std::span<const T, N> value,
                                   std::size_t           seed = 0xc70f6907UL)
  {
    std::ranges::for_each(
      value,
      [&seed](const auto& value)
      {
        seed = hash_bytes(value, seed);
      });

    return seed;
  }

  // FNV hash
  template <consteval_bit_castable T>
  constexpr std::size_t fnv_hash_bytes(const T&    value,
                                       std::size_t seed = 2166136261UL)
  {
    const auto bytes = std::bit_cast<std::array<std::byte, sizeof(T)>>(value);

    return detail::fnv_hash_bytes(std::span{bytes}, std::move(seed));
  }

  template <available_as_byte T, std::size_t N = std::dynamic_extent>
  constexpr std::size_t
    fnv_hash_bytes(std::span<const T, N> value,
                   std::size_t           seed = 2166136261UL)
  {
    return detail::fnv_hash_bytes(std::move(value), std::move(seed));
  }

  template <consteval_bit_castable T, std::size_t N = std::dynamic_extent>
  requires (!available_as_byte<T>)
  constexpr std::size_t
    fnv_hash_bytes(std::span<const T, N> value,
                   std::size_t           seed = 2166136261UL)
  {
    std::ranges::for_each(
      value,
      [&seed](const auto& value)
      {
        seed = fnv_hash_bytes(value, seed);
      });

    return seed;
  }
}

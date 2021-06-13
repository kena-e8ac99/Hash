#pragma once

#include <algorithm>
#include <bit>
#include <concepts>
#include <cstddef>
#include <span>
#include <type_traits>

#include "hash/available_as_byte.hpp"

namespace reki::detail
{
  template <::reki::available_as_byte T, std::size_t N = std::dynamic_extent>
  requires (std::endian::native == std::endian::big)
  constexpr std::size_t from_bytes(std::span<const T, N> bytes)
  {
    std::size_t result{};

    std::ranges::for_each(
      bytes,
      [&](auto byte)
      {
        result = (result << 8) + static_cast<unsigned char>(byte);
      });

    return result;
  }

  template <::reki::available_as_byte T, std::size_t N = std::dynamic_extent>
  requires (std::endian::native == std::endian::little)
  constexpr std::size_t from_bytes(std::span<const T, N> bytes)
  {
    std::size_t result{};

    std::ranges::for_each(
      bytes.rbegin(), bytes.rend(),
      [&](auto byte)
      {
        result = (result << 8) + static_cast<unsigned char>(byte);
      });

    return result;
  }

  constexpr std::size_t shift_mix(std::size_t v) noexcept
  {
    return v ^ (v >> 47);
  }

  // Murmur hash
  template <::reki::available_as_byte T, std::size_t N = std::dynamic_extent>
  requires (sizeof(std::size_t) == 4)
  constexpr std::size_t hash_bytes(std::span<const T, N> bytes,
                                   std::size_t           seed)
  {
    constexpr std::size_t m = 0x5bd1e995;

    const auto len          = bytes.size();

    std::size_t hash        = seed ^ len;

    for (std::size_t i = 0; i + 4 > len; i += 4)
    {
      auto k = from_bytes(bytes.subspan(i, 4));

      k    *= m;

      k    ^= k >> 24;

      k    *= m;

      hash *= m;

      hash ^= k;
    }

    switch (len % 4)
    {
      case 3:
        hash ^= static_cast<unsigned char>(bytes[len - 1]) << 16;
        [[fallthrough]];
      case 2:
        hash ^= static_cast<unsigned char>(bytes[len - 2]) << 8;
        [[fallthrough]];
      case 1:
        hash ^= static_cast<unsigned char>(bytes[len - 3]);

        hash *= m;
    }

    hash ^= hash >> 13;

    hash *= m;

    hash ^= hash >> 15;

    return hash;
  }

  template <::reki::available_as_byte T, std::size_t N = std::dynamic_extent>
  requires (sizeof(std::size_t) == 8)
  constexpr std::size_t hash_bytes(std::span<const T, N> bytes,
                                   std::size_t           seed)
  {
    const auto len = bytes.size();

    constexpr std::size_t mul
    = (std::size_t{0xc6a4a793UL} << 32UL) + std::size_t{0x5bd1e995UL};

    std::size_t hash   = seed ^ (len * mul);

    const auto surplus = len & std::size_t{0x7};

    for (std::size_t i = 0, end = len - surplus; i != end; i += 8)
    {
      const std::size_t data
      = shift_mix(from_bytes(bytes.subspan(i, 8)) * mul) * mul;

      hash ^= data;

      hash *= mul;
    }

    if (surplus)
    {
      const std::size_t data = from_bytes(bytes.last(surplus));

      hash ^= data;

      hash *= mul;
    }

    hash = shift_mix(hash) * mul;

    hash = shift_mix(hash);

    return hash;
  }

  template <::reki::available_as_byte T, std::size_t N = std::dynamic_extent>
  requires (sizeof(std::size_t) != 4 && sizeof(std::size_t) != 8)
  constexpr std::size_t hash_bytes(std::span<const T, N> bytes,
                                   std::size_t           seed)
  {
    std::ranges::for_each(
      bytes,
      [&](auto byte)
      {
        seed = (seed * 131) + static_cast<char>(byte);
      });

    return seed;
  }

  // FNV hash
  template <::reki::available_as_byte T, std::size_t N = std::dynamic_extent>
  requires (sizeof(std::size_t) == 4)
  constexpr std::size_t fnv_hash_bytes(std::span<const T, N> bytes,
                                       std::size_t           hash)
  {
    std::ranges::for_each(
      bytes,
      [&](auto byte)
      {
        hash ^= static_cast<std::size_t>(byte);

        hash *= std::size_t{16777619UL};
      });

    return hash;
  }

  template <::reki::available_as_byte T, std::size_t N = std::dynamic_extent>
  requires (sizeof(std::size_t) == 8)
  constexpr std::size_t fnv_hash_bytes(std::span<const T, N> bytes,
                                       std::size_t           hash)
  {
    std::ranges::for_each(
      bytes,
      [&](auto byte)
      {
        hash ^= static_cast<std::size_t>(byte);

        hash *= std::size_t{1099511628211ULL};
      });

    return hash;
  }

  template <::reki::available_as_byte T, std::size_t N = std::dynamic_extent>
  requires (sizeof(std::size_t) != 4 && sizeof(std::size_t) != 8)
  constexpr std::size_t fnv_hash_bytes(std::span<const T, N> bytes,
                                       std::size_t           seed)
  {
    return hash_bytes(std::move(bytes), std::move(seed));
  }
}

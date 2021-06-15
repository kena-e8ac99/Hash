#pragma once

#include <algorithm>
#include <bit>
#include <concepts>
#include <cstddef>
#include <numeric>
#include <span>
#include <type_traits>

#include "hash/hash_concepts.hpp"

namespace reki::detail
{
  template <::reki::available_as_byte T, std::size_t N = std::dynamic_extent>
  requires (std::endian::native == std::endian::big)
  constexpr std::size_t from_bytes(std::span<const T, N> bytes)
  {
    return std::accumulate(
             bytes.begin(), bytes.end(), std::size_t{},
             [](auto acc, auto byte)
             {
               return (acc << 8) + static_cast<unsigned char>(byte);
             });
  }

  template <::reki::available_as_byte T, std::size_t N = std::dynamic_extent>
  requires (std::endian::native == std::endian::little)
  constexpr std::size_t from_bytes(std::span<const T, N> bytes)
  {
    return std::accumulate(
             bytes.rbegin(), bytes.rend(), std::size_t{},
             [](auto acc, auto byte)
             {
               return (acc << 8) + static_cast<unsigned char>(byte);
             });
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
    return std::accumulate(
             bytes.begin(), bytes.end(), seed,
             [](auto acc, auto byte)
             {
               return (acc * 131) + static_cast<char>(byte);
             });
  }

  template <::reki::consteval_bit_castable T>
  constexpr std::size_t hash_bytes(const T&    value,
                                   std::size_t seed = 0xc70f6907UL)
  {
    const auto bytes = std::bit_cast<std::array<std::byte, sizeof(T)>>(value);

    return hash_bytes(std::span{bytes}, std::move(seed));
  }

  template <::reki::available_as_byte T, std::size_t N = std::dynamic_extent>
  constexpr std::size_t hash_bytes(std::span<const T, N> value,
                                   std::size_t           seed = 0xc70f6907UL)
  {
    return hash_bytes(std::move(value), std::move(seed));
  }

  // FNV hash
  template <::reki::available_as_byte T, std::size_t N = std::dynamic_extent>
  requires (sizeof(std::size_t) == 4)
  constexpr std::size_t fnv_hash_bytes(std::span<const T, N> bytes,
                                       std::size_t           hash)
  {
    return std::accumulate(
             bytes.begin(), bytes.end(), hash,
             [](auto acc, auto byte)
             {
               acc ^= static_cast<std::size_t>(byte);

               return acc * std::size_t{16777619UL};
             });
  }

  template <::reki::available_as_byte T, std::size_t N = std::dynamic_extent>
  requires (sizeof(std::size_t) == 8)
  constexpr std::size_t fnv_hash_bytes(std::span<const T, N> bytes,
                                       std::size_t           hash)
  {
    return std::accumulate(
             bytes.begin(), bytes.end(), hash,
             [](auto acc, auto byte)
             {
               acc ^= static_cast<std::size_t>(byte);

               return acc * std::size_t{1099511628211ULL};
             });
  }

  template <::reki::available_as_byte T, std::size_t N = std::dynamic_extent>
  requires (sizeof(std::size_t) != 4 && sizeof(std::size_t) != 8)
  constexpr std::size_t fnv_hash_bytes(std::span<const T, N> bytes,
                                       std::size_t           seed)
  {
    return hash_bytes(std::move(bytes), std::move(seed));
  }

  template <::reki::consteval_bit_castable T>
  constexpr std::size_t fnv_hash_bytes(const T&    value,
                                       std::size_t seed = 2166136261UL)
  {
    const auto bytes = std::bit_cast<std::array<std::byte, sizeof(T)>>(value);

    return fnv_hash_bytes(std::span{bytes}, std::move(seed));
  }

  template <::reki::available_as_byte T, std::size_t N = std::dynamic_extent>
  constexpr std::size_t
    fnv_hash_bytes(std::span<const T, N> value,
                   std::size_t           seed = 2166136261UL)
  {
    return fnv_hash_bytes(std::move(value), std::move(seed));
  }
}

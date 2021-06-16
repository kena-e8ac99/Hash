#pragma once

#include <algorithm>
#include <bit>
#include <cstdint>
#include <numeric>
#include <span>
#include <type_traits>
#include <utility>

#include "hash/detail/hash_concepts.hpp"

namespace reki::detail
{
  template <std::size_t N>
  using destination_t
  = std::conditional_t<N == 1, std::uint8_t,
      std::conditional_t<N == 2, std::uint16_t,
        std::conditional_t<N == 4, std::uint32_t,std::uint64_t>>>;

  template <available_as_bytes T, std::size_t N = std::dynamic_extent>
  requires (std::endian::native == std::endian::big)
  constexpr std::size_t from_bytes(std::span<const T, N> bytes)
  {
    if constexpr (sizeof(T) == sizeof(std::size_t))
    {
      return bytes.front();
    }
    else
    {
      return std::accumulate(
               bytes.begin(), bytes.end(), std::size_t{},
               [](auto acc, auto byte)
               {
                 return (acc << (8 * sizeof(T)))
                        + static_cast<destination_t<sizeof(T)>>(byte);
               });
    }
  }

  template <available_as_bytes T, std::size_t N = std::dynamic_extent>
  requires (std::endian::native == std::endian::little)
  constexpr std::size_t from_bytes(std::span<const T, N> bytes)
  {
    if constexpr (sizeof(T) == sizeof(std::size_t))
    {
      return bytes.front();
    }
    else
    {
      return std::accumulate(
               bytes.rbegin(), bytes.rend(), std::size_t{},
               [](auto acc, auto byte)
               {
                 return (acc << (8 * sizeof(T)))
                        + static_cast<destination_t<sizeof(T)>>(byte);
               });
    }
  }

  constexpr std::size_t shift_mix(std::size_t v) noexcept
  {
    return v ^ (v >> 47);
  }

  // Murmur hash
  template <available_as_bytes T, std::size_t N = std::dynamic_extent>
  requires (sizeof(std::size_t) == 4)
  constexpr std::size_t hash_bytes(std::span<const T, N> bytes,
                                   std::size_t           seed)
  {
    constexpr std::size_t m = 0x5bd1e995;

    const auto len          = bytes.size();

    const auto interval     = 4 / sizeof(T);

    std::size_t hash        = seed ^ (len * sizeof(T));

    for (std::size_t i = 0; i + interval > len; i += interval)
    {
      auto k = from_bytes(bytes.subspan(i, interval));

      k    *= m;

      k    ^= k >> 24;

      k    *= m;

      hash *= m;

      hash ^= k;
    }

    if (const auto surplus = len % interval)
    {
      const auto data = from_bytes(bytes.last(surplus));

      hash ^= data;

      hash ^= m;
    }

    hash ^= hash >> 13;

    hash *= m;

    hash ^= hash >> 15;

    return hash;
  }

  template <available_as_bytes T, std::size_t N = std::dynamic_extent>
  requires (sizeof(std::size_t) == 8)
  constexpr std::size_t hash_bytes(std::span<const T, N> bytes,
                                   std::size_t           seed)
  {
    const auto len = bytes.size();

    const auto interval = 8 / sizeof(T);

    constexpr std::size_t mul
    = (std::size_t{0xc6a4a793UL} << 32UL) + std::size_t{0x5bd1e995UL};

    std::size_t hash   = seed ^ (len * sizeof(T) * mul);

    const auto surplus = len & std::size_t{(8 / sizeof(T)) - 1};

    for (std::size_t i = 0, end = len - surplus; i != end; i += interval)
    {
      const std::size_t data
      = shift_mix(from_bytes(bytes.subspan(i, interval)) * mul) * mul;

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

  template <available_as_bytes T, std::size_t N = std::dynamic_extent>
  requires (sizeof(std::size_t) != 4 && sizeof(std::size_t) != 8)
  constexpr std::size_t hash_bytes(std::span<const T, N> bytes,
                                   std::size_t           seed)
  {
    constexpr auto impl =
      []<std::size_t... I>(std::index_sequence<I...>)
      {
        return [](std::size_t acc) noexcept
        {
          return (acc * ... * (static_cast<void>(I), 131));
        };
      }(std::make_index_sequence<sizeof(T)>{});

    return std::accumulate(
             bytes.begin(), bytes.end(), seed,
             [](auto acc, auto byte)
             {
               return impl(acc) + static_cast<destination_t<sizeof(T)>>(byte);
             });
  }

  template <consteval_bit_castable T>
  constexpr std::size_t hash_bytes(const T&    value,
                                   std::size_t seed = 0xc70f6907UL)
  {
    constexpr std::size_t gcd = std::gcd(sizeof(T), sizeof(std::size_t));

    const auto bytes
    = std::bit_cast<std::array<destination_t<gcd>, sizeof(T) / gcd>>(value);

    return hash_bytes(std::span{bytes}, std::move(seed));
  }

  template <available_as_bytes T, std::size_t N = std::dynamic_extent>
  constexpr std::size_t hash_bytes(std::span<const T, N> value,
                                   std::size_t           seed = 0xc70f6907UL)
  {
    return hash_bytes(std::move(value), std::move(seed));
  }

}

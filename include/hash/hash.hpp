#pragma once

#include <concepts>
#include <string_view>
#include <type_traits>
#include <variant>

#include "hash/hash_combine.hpp"
#include "hash/hash_bytes.hpp"

namespace reki
{
  template <typename T>
  struct hash final
  {
    constexpr hash()  = delete;
  };

  template <std::integral T>
  requires (sizeof(T) <= sizeof(std::size_t))
  struct hash<T> final
  {
    constexpr std::size_t operator()(T value) const noexcept
    {
      return static_cast<std::size_t>(std::move(value));
    }
  };

  template <typename T>
  requires (std::floating_point<T> ||
           (std::integral<T> && sizeof(T) > sizeof(std::size_t)))
  struct hash<T> final
  {
    constexpr std::size_t operator()(T value) const
    {
      return value != T{0} ? hash_bytes(std::move(value)) : 0;
    }
  };

  template <typename T>
  requires std::is_enum_v<T>
  struct hash<T> final
  {
    constexpr std::size_t
      operator()(T value) const noexcept(sizeof(T) <= sizeof(std::size_t))
    {
      using type = std::underlying_type_t<T>;

      return hash<type>{}(static_cast<type>(std::move(value)));
    }
  };

  template <>
  struct hash<std::nullptr_t> final
  {
    constexpr std::size_t operator()(std::nullptr_t) const noexcept
    {
      return 0;
    }
  };

  template <consteval_bit_castable T>
  requires (!std::integral<T>  && !std::floating_point<T> &&
            !std::is_enum_v<T> && !std::same_as<T, std::nullptr_t> &&
            !std::ranges::input_range<T>)
  struct hash<T> final
  {
    constexpr std::size_t operator()(const T& value) const
    {
      return hash_bytes(value);
    }
  };

  template <typename CharT>
  requires std::same_as<CharT, char>     || std::same_as<CharT, char8_t>  ||
           std::same_as<CharT, char16_t> || std::same_as<CharT, char32_t> ||
           std::same_as<CharT, wchar_t>
  struct hash<CharT*> final
  {
    constexpr std::size_t operator()(std::basic_string_view<CharT> value)
    {
      return hash_bytes(std::span{value.cbegin(), value.cend()});
    }
  };

  template <typename CharT>
  requires std::same_as<CharT, char>     || std::same_as<CharT, char8_t>  ||
           std::same_as<CharT, char16_t> || std::same_as<CharT, char32_t> ||
           std::same_as<CharT, wchar_t>
  struct hash<const CharT*> final
  {
    constexpr std::size_t operator()(std::basic_string_view<CharT> value)
    {
      return hash<CharT*>{}(std::move(value));
    }
  };

  template <std::ranges::input_range T>
  requires std::default_initializable<hash<std::ranges::range_value_t<T>>>
  struct hash<T> final
  {
    constexpr std::size_t operator()(const T& value) const
    {
      using value_type = std::ranges::range_value_t<T>;

      if constexpr (std::ranges::input_range<value_type>)
      {
        std::size_t seed = hash<value_type>{}(*std::ranges::begin(value));

        std::ranges::for_each(
          std::ranges::begin(value) + 1, std::ranges::end(value),
          [&seed](const auto& value)
          {
            hash_combine(seed, hash<value_type>{}(value));
          });

        return seed;
      }
      else
      {
        return hash_bytes(std::span{std::ranges::begin(value),
                                    std::ranges::end(value)});
      }
    }
  };

  template <typename T, typename U>
  requires std::default_initializable<hash<T>> &&
           std::default_initializable<hash<U>>
  struct hash<std::pair<T, U>> final
  {
    constexpr std::size_t operator()(const std::pair<T, U>& value) const
    {
      std::size_t seed = hash<T>{}(value.first);

      hash_combine(seed, hash<U>{}(value.second));

      return seed;
    }
  };

  template <typename... T>
  requires (std::default_initializable<hash<T>> && ...)
  struct hash<std::tuple<T...>> final
  {
    constexpr std::size_t operator()(const std::tuple<T...>& value) const
    {
      return
        std::apply(
          [](const T&... value) mutable
          {
            std::size_t seed = 0;

            ([&seed]<class U>(const U& value)
            {
              hash_combine(seed, hash<U>{}(value));
            }(value), ...);

            return seed;
          }, value);
    }
  };

  template <>
  struct hash<std::monostate> final
  {
    constexpr std::size_t operator()(const std::monostate&) const noexcept
    {
      return static_cast<std::size_t>(-7777);
    }
  };

  template <typename... T>
  requires (std::default_initializable<hash<T>> && ...)
  struct hash<std::variant<T...>> final
  {
    constexpr std::size_t operator()(const std::variant<T...>& value) const
    {
      return
        std::visit(
          [seed = value.index()]<class U>(const U& value)
          {
            return seed + hash<U>{}(value);
          }, value);
    }
  };

}

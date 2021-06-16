#include "hash/hash.hpp"

#include <cstdint>
#include <functional>
#include <limits>
#include <type_traits>

#include <boost/ut.hpp>

using namespace boost::ut;

template <typename T>
requires std::is_arithmetic_v<T>
constexpr void check()
{
  constexpr auto min = std::numeric_limits<T>::lowest();

  constexpr auto max = std::numeric_limits<T>::max();

  constexpr auto half = max / 2;

  constexpr auto min_result = reki::hash{}(min);

  constexpr auto max_result = reki::hash{}(max);

  constexpr auto half_result = reki::hash{}(half);

  expect(eq(min_result, std::hash<T>{}(min)));

  expect(eq(max_result, std::hash<T>{}(max)));

  expect(eq(half_result, std::hash<T>{}(half)));
}

template <typename T>
requires std::is_enum_v<T>
constexpr void check()
{
  using type = std::underlying_type_t<T>;

  constexpr auto min = std::numeric_limits<type>::lowest();

  constexpr auto max = std::numeric_limits<type>::max();

  constexpr auto half = max / 2;

  constexpr auto min_result = reki::hash{}(T{min});

  constexpr auto max_result = reki::hash{}(T{max});

  constexpr auto half_result = reki::hash{}(T{half});

  expect(eq(min_result, std::hash<T>{}(T{min})));

  expect(eq(max_result, std::hash<T>{}(T{max})));

  expect(eq(half_result, std::hash<T>{}(T{half})));
}

template <std::same_as<std::nullptr_t> T>
constexpr void check()
{
  constexpr auto result = reki::hash{}(T{});

  expect(eq(result, std::hash<T>{}(T{})));
}

int main()
{
  "bool"_test = [](){ check<bool>(); };

  "char"_test = [](){ check<char>(); };

  "signed char"_test = [](){ check<signed char>(); };

  "unsigned char"_test = [](){ check<unsigned char>(); };

  "char8_t"_test = [](){ check<char8_t>(); };

  "char16_t"_test = [](){ check<char16_t>(); };

  "char32_t"_test = [](){ check<char32_t>(); };

  "wchar_t"_test = [](){ check<wchar_t>(); };

  "short"_test = [](){ check<short>(); };

  "unsigned short"_test = [](){ check<unsigned short>(); };

  "int"_test = [](){ check<int>(); };

  "unsigned int"_test = [](){ check<unsigned int>(); };

  "long"_test = [](){ check<long>(); };

  "long long"_test = [](){ check<long long>(); };

  "unsigned long"_test = [](){ check<unsigned long>(); };

  "unsigned long long"_test = [](){ check<unsigned long long>(); };

  "float"_test = [](){ check<float>(); };

  "double"_test = [](){ check<double>(); };

  "long double"_test = [](){ check<long double>(); };

  "nullptr_t"_test = [](){ check<std::nullptr_t>(); };

  "byte"_test = [](){ check<std::byte>(); };

  enum class my_enum {};

  "user defined enum"_test = [](){ check<my_enum>(); };
}

#include "hash/hash.hpp"

#include <cstdint>
#include <functional>

#include <boost/ut.hpp>
#include <limits>

template <typename T, T N>
constexpr void check()
{
  using namespace boost::ut;

  constexpr auto result = reki::hash{}(N);

  expect(eq(result, std::hash<T>{}(N)));
}

int main()
{
  using namespace boost::ut;

  "bool"_test =
    []()
    {
      check<bool, true>();
    };

  "int"_test =
    []()
    {
      check<int, std::numeric_limits<int>::lowest()>();

      check<int, 42>();

      check<int, std::numeric_limits<int>::max()>();
    };

  "uint8_t"_test =
    []()
    {
      check<std::uint8_t, std::numeric_limits<std::uint8_t>::lowest()>();

      check<std::uint8_t, 42>();

      check<std::uint8_t, std::numeric_limits<std::uint8_t>::max()>();
    };

  "byte"_test =
    []()
    {
      check<std::byte, std::byte{42}>();
    };

  "size_t"_test =
    []()
    {
      check<std::size_t, std::numeric_limits<std::size_t>::lowest()>();

      check<std::size_t, 42>();

      check<std::size_t, std::numeric_limits<std::size_t>::max()>();
    };

  "float"_test =
    []()
    {
      constexpr auto result1
      = reki::hash<float>{}(std::numeric_limits<float>::lowest());

      constexpr auto result2 = reki::hash<float>{}(42.0f);

      constexpr auto result3
      = reki::hash<float>{}(std::numeric_limits<float>::max());

      expect(neq(result1, std::size_t{0}));

      expect(neq(result1, std::numeric_limits<float>::lowest()));

      expect(eq(result2, std::hash<float>{}(42.0f)));

      expect(neq(result3, std::size_t{0}));

      expect(neq(result3, std::numeric_limits<float>::max()));
    };

  "double"_test =
    []()
    {
      constexpr auto result1
      = reki::hash<double>{}(std::numeric_limits<double>::lowest());

      constexpr auto result2 = reki::hash<double>{}(42.0);

      constexpr auto result3
      = reki::hash<double>{}(std::numeric_limits<double>::max());

      expect(neq(result1, std::size_t{0}));

      expect(neq(result1, std::numeric_limits<double>::lowest()));

      expect(eq(result2, std::hash<double>{}(42.0)));

      expect(neq(result3, std::size_t{0}));

      expect(neq(result3, std::numeric_limits<double>::max()));
    };
}

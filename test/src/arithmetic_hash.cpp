#include "hash/hash.hpp"

#include <cstdint>
#include <functional>

#include <boost/ut.hpp>

template <typename T, T N>
constexpr void check()
{
  using namespace boost::ut;

  constexpr auto result = reki::hash<T>{}(N);

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
      check<int, 42>();
    };

  "uint8_t"_test =
    []()
    {
      check<std::uint8_t, 42>();
    };

  "byte"_test =
    []()
    {
      check<std::byte, std::byte{42}>();
    };

  "size_t"_test =
    []()
    {
      check<std::size_t, 42>();
    };

  "float"_test =
    []()
    {
      using namespace boost::ut;

      constexpr auto result = reki::hash<float>{}(42.0f);

      expect(eq(result, std::hash<float>{}(42.0f)));
    };

  "double"_test =
    []()
    {
      using namespace boost::ut;

      constexpr auto result = reki::hash<double>{}(42.0);

      expect(eq(result, std::hash<double>{}(42.0)));
    };
}

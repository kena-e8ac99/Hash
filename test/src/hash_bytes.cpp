#include "hash/hash_bytes.hpp"

#include <bits/hash_bytes.h>

#include <boost/ut.hpp>
#include <cstddef>

int main()
{
  using namespace boost::ut;

  "int"_test =
    []()
    {
      constexpr int answer = 42;

      constexpr auto my_hash = reki::hash_bytes(answer, 1);

      auto std_hash = std::_Hash_bytes(&answer, sizeof(int), 1);

      expect(eq(my_hash, std_hash));

      constexpr auto my_hash_fnv = reki::fnv_hash_bytes(answer, 1);

      auto std_hash_fnv = std::_Fnv_hash_bytes(&answer, sizeof(int), 1);

      expect(eq(my_hash_fnv, std_hash_fnv));
    };

  "double"_test =
    []()
    {
      constexpr double answer = 42.0;

      constexpr auto my_hash = reki::hash_bytes(answer, 1);

      auto std_hash = std::_Hash_bytes(&answer, sizeof(double), 1);

      expect(eq(my_hash, std_hash));

      constexpr auto my_hash_fnv = reki::fnv_hash_bytes(answer, 1);

      auto std_hash_fnv = std::_Fnv_hash_bytes(&answer, sizeof(double), 1);

      expect(eq(my_hash_fnv, std_hash_fnv));
    };

  "const char*"_test =
    []()
    {
      constexpr auto text = "Hello World!";

      constexpr auto len = std::string_view{text}.size();

      constexpr auto my_hash = reki::hash_bytes(std::span{text, len}, 1);

      auto std_hash = std::_Hash_bytes(text, len, 1);

      expect(eq(my_hash, std_hash));

      constexpr auto my_hash_fnv
      = reki::fnv_hash_bytes(std::span{text, len}, 1);

      auto std_hash_fnv
      = std::_Fnv_hash_bytes(text, len, 1);

      expect(eq(my_hash_fnv, std_hash_fnv));
    };

  "string_view"_test =
    []()
    {
      constexpr std::string_view text = "Hello World!";

      constexpr auto my_hash
      = reki::hash_bytes(std::span{text.cbegin(), text.cend()}, 1);

      auto std_hash = std::_Hash_bytes(text.data(), text.size(), 1);

      expect(eq(my_hash, std_hash));

      constexpr auto my_hash_fnv
      = reki::fnv_hash_bytes(std::span{text.cbegin(), text.cend()}, 1);

      auto std_hash_fnv = std::_Fnv_hash_bytes(text.data(), text.size(), 1);

      expect(eq(my_hash_fnv, std_hash_fnv));
    };

  "std::byte[3]"_test =
    []()
    {
      constexpr std::byte test[3]{std::byte{0}, std::byte{1}, std::byte{2}};

      constexpr auto size = sizeof(std::byte) * 3;

      constexpr auto my_hash = reki::hash_bytes(std::span{test}, 1);

      auto std_hash = std::_Hash_bytes(test, size, 1);

      expect(eq(my_hash, std_hash));

      constexpr auto my_hash_fnv = reki::fnv_hash_bytes(std::span{test}, 1);

      auto std_hash_fnv = std::_Fnv_hash_bytes(test, size, 1);

      expect(eq(my_hash_fnv, std_hash_fnv));
    };

  "array<std::byte, 3>"_test =
    []()
    {
      constexpr std::array<std::byte, 3> test{std::byte{0}, std::byte{1},
                                              std::byte{2}};

      constexpr auto size = sizeof(decltype(test));

      constexpr auto my_hash = reki::hash_bytes(std::span{test}, 1);

      auto std_hash = std::_Hash_bytes(&test, size, 1);

      expect(eq(my_hash, std_hash));

      constexpr auto my_hash_fnv = reki::fnv_hash_bytes(std::span{test}, 1);

      auto std_hash_fnv = std::_Fnv_hash_bytes(&test, size, 1);

      expect(eq(my_hash_fnv, std_hash_fnv));
    };

  "int[3]"_test =
    []()
    {
      constexpr int test[3]{0, 1, 2};

      constexpr auto my_hash = reki::hash_bytes(std::span{test}, 1);

      expect(constant<my_hash != 0>);

      constexpr auto my_hash_fnv = reki::fnv_hash_bytes(std::span{test}, 1);

      expect(constant<my_hash_fnv != 0>);
    };

  "array<int, 3>"_test =
    []()
    {
      constexpr std::array<int, 3> test{0, 1, 2};

      constexpr auto my_hash = reki::hash_bytes(std::span{test}, 1);

      expect(constant<my_hash != 0>);

      constexpr auto my_hash_fnv = reki::fnv_hash_bytes(std::span{test}, 1);

      expect(constant<my_hash_fnv != 0>);
    };

  "tuple"_test =
    []()
    {
      constexpr std::tuple<int, double, std::byte> value{1, 2.0, std::byte{3}};

      constexpr auto my_hash = reki::hash_bytes(value, 1);

      expect(constant<my_hash != 0>);

      constexpr auto my_hash_fnv = reki::fnv_hash_bytes(value, 1);

      expect(constant<my_hash_fnv != 0>);
    };
}

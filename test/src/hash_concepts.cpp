#include "hash/hash_concepts.hpp"

#include <array>
#include <cstddef>
#include <cstdint>

#include <boost/ut.hpp>

int main()
{
  using namespace boost::ut;

  "available_as_byte"_test =
    []()
    {
      expect(constant<reki::available_as_byte<std::byte>>);

      expect(constant<reki::available_as_byte<std::int8_t>>);

      expect(constant<reki::available_as_byte<char>>);

      expect(constant<reki::available_as_byte<unsigned char>>);
    };

  "unavailable_as_byte"_test =
    []()
    {
      expect(constant<!reki::available_as_byte<bool>>);

      expect(constant<!reki::available_as_byte<int>>);

      expect(constant<!reki::available_as_byte<float>>);

      expect(constant<!reki::available_as_byte<std::string>>);
    };

  "costeval_bit_castable"_test =
    []()
    {
      expect(constant<reki::consteval_bit_castable<bool>>);

      expect(constant<reki::consteval_bit_castable<int>>);

      expect(constant<reki::consteval_bit_castable<float>>);

      expect(constant<reki::consteval_bit_castable<
                        std::array<std::size_t, 10>>>);
    };

  "!consteval_bit_castable"_test =
    []()
    {
      expect(constant<!reki::consteval_bit_castable<std::string>>);

      expect(constant<!reki::consteval_bit_castable<std::vector<std::byte>>>);
    };
}

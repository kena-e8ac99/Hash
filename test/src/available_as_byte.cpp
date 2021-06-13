#include "hash/available_as_byte.hpp"

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
}

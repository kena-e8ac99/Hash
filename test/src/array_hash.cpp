#include "hash/hash.hpp"

#include <array>
#include <functional>

#include <boost/ut.hpp>
#include <string_view>

int main()
{
  using namespace boost::ut;

  "int array"_test =
    []()
    {
      constexpr int value1[3]{0, 1, 2};

      constexpr std::array<int, 3> value2{0, 1, 2};

      constexpr auto result1 = reki::hash{}(value1);

      constexpr auto result2 = reki::hash{}(value2);

      expect(neq(result1, std::size_t{0}));

      expect(eq(result1, result2));
    };

  "string_view array"_test =
    []()
    {
      constexpr std::string_view value1[]{"Hello", "World", "!"};

      constexpr std::array<std::string_view, 3> value2{"Hello", "World", "!"};

      constexpr auto result1 = reki::hash{}(value1);

      constexpr auto result2
      = reki::hash{}(value2);

      expect(neq(result1, std::size_t{0}));

      expect(eq(result1, result2));
    };

  "two dimensional array"_test =
    []()
    {
      using c_cube = int[2][3];

      using cube = std::array<std::array<int, 3>, 2>;

      constexpr c_cube value1{{0, 1, 2}, {0, 1, 2}};

      constexpr cube   value2{ { {0, 1, 2} , {0, 1, 2} } };

      constexpr auto result1 = reki::hash{}(value1);

      constexpr auto result2 = reki::hash{}(value2);

      expect(neq(result1, std::size_t{0}));

      expect(eq(result1, result2));
    };
}

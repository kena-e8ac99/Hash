#include "hash/hash.hpp"

#include <cstdint>
#include <string_view>
#include <tuple>

#include <boost/ut.hpp>

int main()
{
  using namespace boost::ut;

  "pair<int, float>"_test =
    []()
    {
      using type = std::pair<int, float>;

      constexpr type value1{1, 1.0f};

      constexpr type value2{1, 1.0f};

      constexpr auto result1 = reki::hash<type>{}(value1);

      constexpr auto result2 = reki::hash<type>{}(value2);

      expect(neq(result1, std::size_t{0}));

      expect(eq(result1, result2));
    };

  "tuple<int, float, string_view>"_test =
    []()
    {
      using type = std::tuple<int, float, std::string_view>;

      constexpr type value1{1, 1.0, "Hello World!"};

      constexpr type value2{1, 1.0, "Hello World!"};

      constexpr auto result1 = reki::hash<type>{}(value1);

      constexpr auto result2 = reki::hash<type>{}(value2);

      expect(neq(result1, std::size_t{0}));

      expect(eq(result1, result2));
    };
}

#include "hash/hash.hpp"

#include <variant>

#include <boost/ut.hpp>

int main()
{
  using namespace boost::ut;

  "variant<int, float>"_test =
    []()
    {
      using type = std::variant<int, float>;

      constexpr type value1 = 1;

      constexpr auto result1 = reki::hash{}(value1);

      constexpr type value2 = 1.0f;

      constexpr auto result2 = reki::hash{}(value2);

      expect(eq(result1, std::hash<type>{}(value1)));

      expect(eq(result2, std::hash<type>{}(value2)));
    };

  "variant<monostate, double>"_test =
    []()
    {
      using type = std::variant<std::monostate, double>;

      constexpr type value1 = std::monostate{};

      constexpr auto result1 = reki::hash{}(value1);

      constexpr type value2 = 1.0;

      constexpr auto result2 = reki::hash{}(value2);

      expect(eq(result1, std::hash<type>{}(value1)));

      expect(eq(result2, std::hash<type>{}(value2)));
    };
}

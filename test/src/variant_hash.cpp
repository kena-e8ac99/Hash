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

      expect(eq(result1, reki::hash<int>{}(1)));

      expect(neq(result2, reki::hash<float>{}(1.0f)));

      expect(neq(result2, std::size_t{0}));
    };

  "variant<monostate, float, int array, std::string_view>"_test =
    []()
    {
      using type = std::variant<std::monostate, int, float, std::array<int, 3>,
                                std::string_view>;

      constexpr type value1 = std::monostate{};

      constexpr auto result1 = reki::hash{}(value1);

      constexpr type value2 = 1.0f;

      constexpr auto result2 = reki::hash{}(value2);

      constexpr type value3 = std::array<int, 3>{0, 1, 2};

      constexpr auto result3 = reki::hash{}(value3);

      constexpr type value4 = "Hello World!";

      constexpr auto result4 = reki::hash{}(value4);

      expect(eq(result1, std::size_t{static_cast<size_t>(-7777)}));

      expect(eq(result1, reki::hash<std::monostate>{}({})));

      expect(neq(result1, result2));

      expect(neq(result2, result3));

      expect(neq(result3, result4));

      expect(neq(result4, std::size_t{0}));
    };
}

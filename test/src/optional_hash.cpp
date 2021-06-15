#include "hash/hash.hpp"

#include <optional>

#include <boost/ut.hpp>

int main()
{
  using namespace boost::ut;

  "optional int"_test =
    []()
    {
      constexpr std::optional<int> value1{};

      constexpr std::optional<int> value2{42};

      constexpr auto result1 = reki::hash{}(value1);

      constexpr auto result2 = reki::hash{}(value2);

      expect(eq(result1, static_cast<std::size_t>(-3333)));

      expect(neq(result1, result2));
    };
}

#include "hash/hash.hpp"

#include <optional>

#include <boost/ut.hpp>

int main()
{
  using namespace boost::ut;

  "nullopt"_test =
    []()
    {
      constexpr std::optional<int> value{};

      constexpr auto result = reki::hash{}(value);

      expect(eq(result, std::hash<std::optional<int>>{}(value)));
    };

  "float"_test =
    []()
    {
      constexpr std::optional<float> value{1.0f};

      constexpr auto result = reki::hash{}(value);

      expect(eq(result, std::hash<std::optional<float>>{}(value)));
    };
}

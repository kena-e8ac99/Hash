#include "hash/hash.hpp"

#include <string>
#include <string_view>

#include <boost/ut.hpp>

using namespace std::literals;

template <typename CharT = char>
constexpr const CharT* value = "Hello World!";

template <>
constexpr const char8_t* value<char8_t> = u8"Hello World!";

template <>
constexpr const char16_t* value<char16_t> = u"Hello World!";

template <>
constexpr const char32_t* value<char32_t> = U"Hello World!";

template <>
constexpr const wchar_t* value<wchar_t> = L"Hello World!";

template <typename CharT>
constexpr void check()
{
  using namespace boost::ut;

  using string_t = std::basic_string<CharT>;

  using view_t   = std::basic_string_view<CharT>;

  string_t string{value<CharT>};

  constexpr view_t   view{value<CharT>};

  constexpr auto chars_result  = reki::hash{}(value<CharT>);

            auto string_result = reki::hash{}(string);

  constexpr auto view_result   = reki::hash{}(view);

  expect(eq(chars_result, string_result));

  expect(eq(string_result, view_result));

  expect(eq(string_result, std::hash<string_t>{}(string)));
}

int main()
{
  using namespace boost::ut;

  using namespace std::literals;

  "char*"_test = [](){ check<char>(); };

  "char8_t*"_test = [](){ check<char8_t>(); };

  "char16_t*"_test = [](){ check<char16_t>(); };

  "char32_t*"_test = [](){ check<char32_t>(); };

  "wchar_t*"_test = [](){ check<wchar_t>(); };
}

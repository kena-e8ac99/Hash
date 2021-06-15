#include "hash/hash.hpp"

#include <string>
#include <string_view>

#include <boost/ut.hpp>

int main()
{
  using namespace boost::ut;

  using namespace std::literals;

  "string"_test =
    []()
    {
      constexpr auto char_text   = "Hello World!";

      constexpr auto view_text   = "Hello World!"sv;

      constexpr auto char_result = reki::hash{}(char_text);

      constexpr auto view_result = reki::hash{}(view_text);

      expect(eq(char_result, view_result));

      expect(eq(view_result, std::hash<std::string_view>{}(view_text)));
    };

  "const char8_t* == u8string_view"_test =
    []()
    {
      constexpr auto char_text   = u8"Hello World!";

      constexpr auto view_text   = u8"Hello World!"sv;

      constexpr auto char_result = reki::hash{}(char_text);

      constexpr auto view_result = reki::hash{}(view_text);

      expect(eq(char_result, view_result));

      expect(eq(view_result, std::hash<std::u8string_view>{}(view_text)));
    };

  "const char16_t* == u16string_view"_test =
    []()
    {
      constexpr auto char_text   = u"Hello World!";

      constexpr auto view_text   = u"Hello World!"sv;

      constexpr auto char_result = reki::hash{}(char_text);

      constexpr auto view_result = reki::hash{}(view_text);

      expect(eq(char_result, view_result));
    };

  "const char32_t* == u32string_view"_test =
    []()
    {
      constexpr auto char_text   = U"Hello World!";

      constexpr auto view_text   = U"Hello World!"sv;

      constexpr auto char_result = reki::hash{}(char_text);

      constexpr auto view_result = reki::hash{}(view_text);

      expect(eq(char_result, view_result));
    };

  "const wchar_t* == wstring_view"_test =
    []()
    {
      constexpr auto char_text   = L"Hello World!";

      constexpr auto view_text   = L"Hello World!"sv;

      constexpr auto char_result = reki::hash{}(char_text);

      constexpr auto view_result = reki::hash{}(view_text);

      expect(eq(char_result, view_result));
    };
}

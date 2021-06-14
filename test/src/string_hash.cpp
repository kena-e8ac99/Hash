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
                auto text        = "Hello World!"s;

      constexpr auto char_text   = "Hello World!";

      constexpr auto view_text   = "Hello World!"sv;

                auto result      = reki::hash<std::string>{}(text);

      constexpr auto char_result = reki::hash<const char*>{}(char_text);

      constexpr auto view_result = reki::hash<std::string_view>{}(view_text);

      expect(neq(result, std::size_t{0}));

      expect(eq(result, char_result));

      expect(eq(char_result, view_result));

      expect(eq(view_result, std::hash<std::string_view>{}(view_text)));
    };

  "const char8_t* == u8string_view"_test =
    []()
    {
                auto text        = u8"Hello World!"s;

      constexpr auto char_text   = u8"Hello World!";

      constexpr auto view_text   = u8"Hello World!"sv;

                auto result      = reki::hash<std::u8string>{}(text);

      constexpr auto char_result = reki::hash<const char8_t*>{}(char_text);

      constexpr auto view_result = reki::hash<std::u8string_view>{}(view_text);

      expect(neq(result, std::size_t{0}));

      expect(eq(result, char_result));

      expect(eq(char_result, view_result));

      expect(eq(view_result, std::hash<std::u8string_view>{}(view_text)));
    };

  "const char16_t* == u16string_view"_test =
    []()
    {
                auto text        = u"Hello World!"s;

      constexpr auto char_text   = u"Hello World!";

      constexpr auto view_text   = u"Hello World!"sv;

                auto result      = reki::hash<std::u16string>{}(text);

      constexpr auto char_result = reki::hash<const char16_t*>{}(char_text);

      constexpr auto view_result = reki::hash<std::u16string_view>{}(view_text);

      expect(neq(result, std::size_t{0}));

      expect(eq(result, char_result));

      expect(eq(char_result, view_result));
    };

  "const char32_t* == u32string_view"_test =
    []()
    {
                auto text        = U"Hello World!"s;

      constexpr auto char_text   = U"Hello World!";

      constexpr auto view_text   = U"Hello World!"sv;

                auto result      = reki::hash<std::u32string>{}(text);

      constexpr auto char_result = reki::hash<const char32_t*>{}(char_text);

      constexpr auto view_result = reki::hash<std::u32string_view>{}(view_text);

      expect(neq(result, std::size_t{0}));

      expect(eq(result, char_result));

      expect(eq(char_result, view_result));
    };

  "const wchar_t* == wstring_view"_test =
    []()
    {
                auto text        = L"Hello World!"s;

      constexpr auto char_text   = L"Hello World!";

      constexpr auto view_text   = L"Hello World!"sv;

                auto result      = reki::hash<std::wstring>{}(text);

      constexpr auto char_result = reki::hash<const wchar_t*>{}(char_text);

      constexpr auto view_result = reki::hash<std::wstring_view>{}(view_text);

      expect(neq(result, std::size_t{0}));

      expect(eq(result, char_result));

      expect(eq(char_result, view_result));
    };
}

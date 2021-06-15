#include "hash/hash.hpp"

#include <string>
#include <string_view>

#include <boost/ut.hpp>

int main()
{
  using namespace boost::ut;

  using namespace std::literals;

  "char*"_test =
    []()
    {
      constexpr auto view  = "Hello World!"sv;

      constexpr auto chars = view.data();

      //std::string str{view};

      constexpr auto chars_result = reki::hash{}(chars);

      constexpr auto view_result  = reki::hash{}(view);

      //auto str_result   = reki::hash{}(str);

      expect(eq(chars_result, view_result));

      //expect(eq(view_result, str_result));

      //expect(eq(str_result, std::hash<std::string>{}(str)));
    };

  "char8_t*"_test =
    []()
    {
      constexpr auto view  = u8"Hello World!"sv;

      constexpr auto chars = view.data();

      //std::u8string str{view};

      constexpr auto chars_result = reki::hash{}(chars);

      constexpr auto view_result  = reki::hash{}(view);

      //auto str_result   = reki::hash{}(str);

      expect(eq(chars_result, view_result));

      //expect(eq(view_result, str_result));

      //expect(eq(str_result, std::hash<std::u8string>{}(str)));
    };

  "char16_t*"_test =
    []()
    {
      constexpr auto view  = u"Hello World!"sv;

      constexpr auto chars = view.data();

      //std::u16string str{view};

      constexpr auto chars_result = reki::hash{}(chars);

      constexpr auto view_result  = reki::hash{}(view);

      //auto str_result   = reki::hash{}(str);

      expect(eq(chars_result, view_result));

      //expect(eq(view_result, str_result));

      //expect(eq(str_result, std::hash<std::u16string>{}(str)));
    };

  "char32_t*"_test =
    []()
    {
      constexpr auto view  = U"Hello World!"sv;

      constexpr auto chars = view.data();

      //std::u32string str{view};

      constexpr auto chars_result = reki::hash{}(chars);

      constexpr auto view_result  = reki::hash{}(view);

      //auto str_result   = reki::hash{}(str);

      expect(eq(chars_result, view_result));

      //expect(eq(view_result, str_result));

      //expect(eq(str_result, std::hash<std::u32string>{}(str)));
    };

  "wchar_t*"_test =
    []()
    {
      constexpr auto view  = L"Hello World!"sv;

      constexpr auto chars = view.data();

      //std::wstring str{view};

      constexpr auto chars_result = reki::hash{}(chars);

      constexpr auto view_result  = reki::hash{}(view);

      //auto str_result   = reki::hash{}(str);

      expect(eq(chars_result, view_result));

      //expect(eq(view_result, str_result));

      //expect(eq(str_result, std::hash<std::wstring>{}(str)));
    };
}

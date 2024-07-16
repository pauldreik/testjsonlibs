#include <fmt/compile.h>
#include <fmt/format.h>

#include <array>
#include <cstdio>
#include <format>
#include <iostream>

template<std::size_t N>
class string
{
public:
  constexpr string()
    : m_data{}
  {
  }

  constexpr string(const char (&str)[N + 1])
    : m_data{}
  {
    for (std::size_t i = 0; i < N; ++i) {
      m_data[i] = str[i];
    }
  }

  constexpr char operator[](std::size_t i) const { return m_data[i]; }

  constexpr auto data() const { return m_data; }

  template<std::size_t N2>
  constexpr bool operator==(const string<N2>& other) const
  {
    if constexpr (N != N2) {
      return false;
    } else {
      for (std::size_t i = 0; i < N; ++i) {
        if (m_data[i] != other[i]) {
          return false;
        }
      }
      return true;
    }
  }

  char m_data[N + 1];
};

template<std::size_t N1, std::size_t N2>
constexpr string<N1 + N2>
operator+(const string<N1>& a, const string<N2>& b)
{
  string<N1 + N2> ret;
  for (std::size_t i = 0; i < N1; ++i) {
    ret.m_data[i] = a[i];
  }
  // copy also the null terminator
  for (std::size_t i = 0; i <= N2; ++i) {
    ret.m_data[N1 + i] = b[i];
  }
  return ret;
}

template<std::size_t N>
string(const char (&str)[N]) -> string<N - 1>;

template<auto Key, typename T>
struct KeyAndValue
{
  T m_value;
  constexpr const T& value() const { return m_value; }
  static constexpr auto key() { return Key; }
  constexpr static inline bool is_key_and_value = true;
};

template<typename T>
concept IsKeyAndValue = T::is_key_and_value;

template<auto Key>
struct KeyHelper
{
  [[nodiscard]] constexpr auto operator=(const int& x) const
  {
    return KeyAndValue<Key, int>{ x };
  }
};

template<string Key>
constexpr auto
operator""_key()
{
  return KeyHelper<Key>{};
}

static constexpr inline void
assemble_format_string(IsKeyAndValue auto keyvalue,
                       IsKeyAndValue auto keyvalue2)
{

  constexpr string escapedleftbrace{ "{{" };
  constexpr string escapedrightbrace{ "}}" };
  constexpr string doublequote{ "\"" };
  constexpr string colon{ ":" };
  constexpr string comma{ "," };
  constexpr string bracepair{ "{}" };
  constexpr string newline{ "\n" };

  static constexpr auto fmt =
    escapedleftbrace +
    //
    (doublequote + keyvalue.key() + doublequote + colon + bracepair) +
    //
    comma +
    //
    (doublequote + keyvalue2.key() + doublequote + colon + bracepair) +
    //
    escapedrightbrace + newline;
  if constexpr (0) {
    // 1.6 seconds
    std::puts("{\"time\":12345,\"count\":4992}");
  }
  if constexpr (0) {
    // 8-9 seconds (!)
    std::print(std::cout, fmt.data(), keyvalue.value(), keyvalue2.value());
  }
  if constexpr (0) {
    // 10 seconds (!)
    auto x = std::format(fmt.data(), keyvalue.value(), keyvalue2.value());
    std::puts(x.c_str());
  }

  if constexpr (0) {
    // 3 - 3.3 seconds
    static constexpr auto compiled = FMT_COMPILE(fmt.data());
    fmt::print(compiled, keyvalue.value(), keyvalue2.value());
  }
  if constexpr (0) {
    // 3.7 - 5.1 seconds
    static constexpr auto compiled = FMT_COMPILE(fmt.data());
    auto x = fmt::format(compiled, keyvalue.value(), keyvalue2.value());
    std::puts(x.c_str());
  }
  if constexpr (1) {
    // 2.3-3.2 seconds
    static constexpr auto compiled = FMT_COMPILE(fmt.data());
    std::array<char, 128> buf;
    auto dest =
      fmt::format_to(buf.data(), compiled, keyvalue.value(), keyvalue2.value());
    std::fwrite(buf.data(), std::distance(buf.data(), dest), 1, stdout);
  }
  if constexpr (0) {
    // 6.7-7.3 seconds
    fmt::print(fmt.data(), keyvalue.value(), keyvalue2.value());
  }
  if constexpr (0) {
    // 6.7-7.3 seconds
    fmt::print(
      "{{\"time\":{},\"count\":{}}}\n", keyvalue.value(), keyvalue2.value());
  }
  if constexpr (0) {
    // 5-6 seconds
    std::printf(
      "{\"time\":%d,\"count\":%d}\n", keyvalue.value(), keyvalue2.value());
  }
}

static void
useit()
{

  for (int i = 0; i < 10000; ++i) {
    for (int j = 0; j < 10000; ++j) {
      auto blah1 = "time"_key = i;
      auto blah2 = "count"_key = j;
      assemble_format_string(blah1, blah2);
    }
  }
}

int
main()
{
  std::cout.sync_with_stdio(false);
  constexpr string hej{ "hej" };
  constexpr string bla{ " där" };
  static_assert(hej == string{ "hej" });
  static_assert(hej != string{ "hej\0" });
  static_assert(hej + bla == string{ "hej där" });

  useit();
}

#include "daw/json/daw_json_link.h"

#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <string>

namespace {
struct MyClass1
{
  int time;
  int count;
};
}

namespace daw::json {
template<>
struct json_data_contract<MyClass1>
{
  using type =
    json_member_list<json_number<"time", int>, json_number<"count", int>>;
  static inline auto to_json_data(MyClass1 const& value)
  {
    return std::forward_as_tuple(value.time, value.count);
  }
};
} // namespace daw::json

static void
doit(int i, int j)
{
  const MyClass1 cls{ .time = i, .count = j };
  const std::string str = daw::json::to_json(cls);
  std::puts(str.c_str());
}

int
main(int argc, char** argv)
{
  for (int i = 0; i < 10000; ++i) {
    for (int j = 0; j < 10000; ++j) {
      doit(i, j);
    }
  }
}

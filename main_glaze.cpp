#include <glaze/glaze.hpp>

#include <cstdio>
#include <string>

namespace {
struct S
{
  int time;
  int count;
};

}

static void
doit(int i, int j){

  const S s{ .time = i, .count = j };
  std::array<char, 128> buf{};
  const auto blah = glz::write_json(s, buf.data());

  std::puts(buf.data());
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

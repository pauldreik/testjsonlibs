#include <boost/json.hpp>

#include <cstdio>
#include <string>

static void
doit(int i, int j){

  boost::json::object obj = { { "time", i }, { "count", j } };

  std::string str = boost::json::serialize(obj);

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

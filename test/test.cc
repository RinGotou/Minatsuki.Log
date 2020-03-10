#include "minatsuki.log.h"

using namespace minatsuki;

int main(int argc, char **argv) {
  StandardRealTimeAgent agent("C:\\workspace\\Minatsuki.log", "a+");
  StandardWriter writer(stdout);

  auto now = time(nullptr);
  writer.Write(ctime(&now));

  for (size_t i = 0; i < 1000000; i += 1) {
    agent.WriteLine("test");
  }

  now = time(nullptr);
  writer.Write(ctime(&now));

  return 0;
}
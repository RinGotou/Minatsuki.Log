#include "minatsuki.log.h"

using namespace minatsuki;

int main(int argc, char **argv) {
  StandardWriter writer(stdout);

  writer.Write("test");
  writer.Write("test2\n");

  return 0;
}
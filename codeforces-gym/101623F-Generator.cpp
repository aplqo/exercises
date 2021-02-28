#include <fstream>
#include <iterator>

#include "debug_tools/generator.h"
using namespace apdebug::generator;

int main(int, char* argv[])
{
  Init();
  const int n = GetRandom(1, atoi(argv[1]));
  std::ofstream in(argv[3]);
  in << n << "\n";
  createArray(1, atoi(argv[2]), n, std::ostream_iterator<int>(in, " "));
  return 0;
}
#include <algorithm>
#include <fstream>
#include <iterator>
#include <random>

#include "debug_tools/generator.h"
using namespace apdebug::generator;
const int maxn = 1e5, maxa = 30000;

int main(int, char* argv[])
{
  Init();
  const int n = GetRandom(1, MayBe(atoi(argv[1]), maxn));
  std::ofstream in(argv[3]);
  in << n << "\n";
  createArray(0, MayBe(atoi(argv[2]), maxa), n,
              std::ostream_iterator<int>(in, " "));
  return 0;
}
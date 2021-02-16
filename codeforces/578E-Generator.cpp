#include <algorithm>
#include <fstream>
#include <iterator>

#include "debug_tools/generator.h"
using namespace apdebug::generator;
const int maxl = 1e5;

char dat[maxl + 10];

int main(int, char* argv[])
{
  Init();
  const unsigned int n = GetRandom(1, MayBe(atoi(argv[1]), maxl));
  {
    const unsigned int h = n >> 1;
    std::fill(dat, dat + h, 'L');
    std::fill(dat + h, dat + n, 'R');
    std::shuffle(dat, dat + n, default_rnd);
  }
  std::ofstream in(argv[2]);
  in << dat;
  return 0;
}
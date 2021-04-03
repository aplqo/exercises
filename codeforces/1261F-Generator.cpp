#include <cstring>
#include <fstream>
#include <random>

#include "debug_tools/generator.h"

using namespace apdebug::generator;

void createRange(std::ofstream& in, const int n, const long long mv)
{
  std::uniform_int_distribution<long long> dis(1, mv);
  for (int i = 0; i < n; ++i) {
    long long v1 = dis(default_rnd), v2 = dis(default_rnd);
    if (v1 > v2) std::swap(v1, v2);
    in << v1 << " " << v2 << "\n";
  }
}
int main(int, char* argv[])
{
  Init();
  const int mn = atoi(argv[1]);
  const long long mv = atoll(argv[2]);
  std::ofstream in(argv[3]);
  const int na = GetRandom(1, mn), nb = GetRandom(1, mn);
  in << na << "\n";
  createRange(in, na, mv);
  in << nb << "\n";
  createRange(in, nb, mv);
  return 0;
}
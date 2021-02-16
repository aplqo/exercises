#include <cstring>
#include <fstream>
#include <random>

#include "debug_tools/generator.h"
using namespace apdebug::generator;
const int maxt = 20, maxk = 501;
const long long maxl = 1e18;

int main(int, char* argv[])
{
  Init();
  const unsigned int t = GetRandom(1, MayBe(atoi(argv[1]), maxt));
  std::uniform_int_distribution<unsigned int> disk(1,
                                                   MayBe(atoi(argv[2]), maxk));
  std::uniform_int_distribution<unsigned long long> disl(
      1ull, MayBe(atoll(argv[3]), maxl));
  std::ofstream in(argv[5]);
  in << t << " " << MayBe(atoi(argv[4]), GetRandom(2, 3)) << "\n";
  for (unsigned int i = t; i; --i) {
    unsigned long long v1 = disl(default_rnd), v2 = disl(default_rnd);
    if (v1 > v2) std::swap(v1, v2);
    in << v1 << " " << v2 << " " << disk(default_rnd) << "\n";
  }
  return 0;
}
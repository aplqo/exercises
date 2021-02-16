#include <algorithm>
#include <cstring>
#include <fstream>
#include <random>

#include "debug_tools/generator.h"
using namespace std;
using namespace apdebug::generator;
const int maxn = 1e6, maxm = 2e7, maxc = 1e6;

void createTree(ofstream& in, const unsigned int n, const unsigned int mc)
{
  uniform_int_distribution<unsigned int> disc(0, mc);
  for (unsigned int i = 2; i <= n; ++i)
    in << i << " " << GetRandom(1u, i - 1) << " " << disc(default_rnd) << "\n";
}
int main(int, char* argv[])
{
  Init();
  ofstream in(argv[4]);
  const unsigned int n = GetRandom(2, MayBe(atoi(argv[1]), maxn));
  in << n << " " << GetRandom(1, MayBe(atoi(argv[2]), maxm)) << "\n";
  createTree(in, n, MayBe(atoi(argv[3]), maxc));
  in << GetRandom(1u, n) << " " << GetRandom(1u, n) << "\n";
  return 0;
}
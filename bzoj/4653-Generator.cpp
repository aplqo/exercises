#include <algorithm>
#include <cstring>
#include <fstream>
#include <random>

#include "debug_tools/generator.h"
using namespace std;
using apdebug::generator::default_rnd;
using apdebug::generator::GetRandom;
using apdebug::generator::Init;
using apdebug::generator::MayBe;
const int maxn = 500000, maxm = 200000, maxv = 1e9;

int main(int, char* argv[])
{
  Init();
  const unsigned int n = GetRandom(1, MayBe(atoi(argv[1]), maxn));
  const unsigned int m =
      GetRandom(1u, min<unsigned int>(MayBe(atoi(argv[2]), maxm), n));
  ofstream in(argv[4]);
  in << n << " " << m << endl;
  uniform_int_distribution<unsigned int> dis(0, MayBe(atoi(argv[3]), maxv));
  for (unsigned int i = 0; i < n; ++i) {
    unsigned int l = dis(default_rnd), r = dis(default_rnd);
    in << min(l, r) << " " << max(l, r) << endl;
  }
  return 0;
}
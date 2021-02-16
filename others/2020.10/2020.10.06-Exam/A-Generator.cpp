#include <algorithm>
#include <cstring>
#include <fstream>
#include <iterator>
#include <random>

#include "debug_tools/generator.h"
using namespace std;
using namespace apdebug::generator;
const int maxn = 1e5, maxv = 1e5, maxt = 500, maxa = 1e5;

void createObject(ofstream& in, const unsigned int n, const unsigned int ma)
{
  uniform_int_distribution<unsigned int> dis(1, ma);
  for (unsigned int i = 0; i < n; ++i)
    in << dis(default_rnd) << " " << dis(default_rnd) << "\n";
}

int main(int, char* argv[])
{
  Init();
  const unsigned int t = GetRandom(1, MayBe(atoi(argv[1]), maxt));
  const unsigned int ma = MayBe(atoi(argv[4]), maxa);
  uniform_int_distribution<unsigned int> disn(1, MayBe(atoi(argv[2]), maxn)),
      disv(1, MayBe(atoi(argv[3]), maxv));
  ofstream in(argv[5]);
  in << t << "\n";
  for (unsigned int i = 0; i < t; ++i) {
    const unsigned int n = disn(default_rnd);
    in << n << " " << disv(default_rnd) << "\n";
    createObject(in, n, ma);
  }
  return 0;
}
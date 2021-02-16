#include <algorithm>
#include <cstring>
#include <fstream>
#include <random>

#include "debug_tools/generator.h"
using namespace std;
using namespace apdebug::generator;
const int maxn = 1e5, maxm = 1e5;

void createQuery(ofstream& in, const unsigned int n, const unsigned int q)
{
  uniform_int_distribution<unsigned int> dis(1, n);
  for (unsigned int i = 0; i < q; ++i) {
    const unsigned int v1 = dis(default_rnd), v2 = dis(default_rnd);
    in << min(v1, v2) << " " << max(v1, v2) << endl;
  }
}
int main(int, char* argv[])
{
  Init();
  const unsigned int n = GetRandom(1, MayBe(atoi(argv[1]), maxn));
  const unsigned int m = GetRandom(1, MayBe(atoi(argv[2]), maxm));
  ofstream in(argv[3]);
  in << n << " " << m << endl;
  createQuery(in, n, m);
  return 0;
}

#include <algorithm>
#include <cstring>
#include <fstream>
#include <numeric>
#include <random>

#include "debug_tools/generator.h"
using namespace std;
using namespace apdebug::generator;
const int maxn = 100000, maxv = 1e6;

unsigned int shf[maxn + 1];

void createShf(const unsigned int n)
{
  iota(shf, shf + 1 + n, 0);
  shuffle(shf + 1, shf + 1 + n, default_rnd);
}
void createChain(ofstream& f, const unsigned int n, const unsigned int len,
                 const unsigned int mv)
{
  uniform_int_distribution<unsigned int> dis(0, mv);
  for (unsigned int i = len + 1; i > 1; --i)
    f << shf[i - 1] << " " << shf[i] << " " << dis(default_rnd) << endl;
}
void createTree(ofstream& f, const unsigned int n, const unsigned int beg,
                const unsigned int mv)
{
  uniform_int_distribution<unsigned int> dis(0, mv);
  for (unsigned int i = beg; i <= n; ++i)
    f << shf[GetRandom(1u, i - 1)] << " " << shf[i] << " " << dis(default_rnd)
      << endl;
}
int main(int, char* argv[])
{
  Init();
  const unsigned int n = GetRandom(1, MayBe(atoi(argv[1]), maxn));
  const unsigned int mv = MayBe(atoi(argv[2]), maxv);
  ofstream in(argv[3]);
  uniform_int_distribution<unsigned int> dis(0, MayBe(atoi(argv[2]), maxv));
  in << n << endl;
  unsigned int len;
  {
    const unsigned int v1 = GetRandom(1u, n - 1), v2 = GetRandom(1u, n - 1);
    in << min(v1, v2) << " " << max(v1, v2) << endl;
    len = min(v1, v2);
  }
  createShf(n);
  createChain(in, n, len, mv);
  createTree(in, n, len + 2, mv);
  return 0;
}
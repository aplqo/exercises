#include <algorithm>
#include <cstring>
#include <fstream>
#include <random>

#include "debug_tools/generator.h"
using namespace std;
using namespace apdebug::generator;
const int maxn = 1e5, maxq = 1e5;

void createTree(ofstream& in, const unsigned int n)
{
  static unsigned int shf[maxn + 10];
  iota(shf + 1, shf + 1 + n, 1);
  shuffle(shf + 1, shf + 1 + n, default_rnd);
  for (unsigned int i = 2; i <= n; ++i)
    in << shf[GetRandom(1u, i - 1)] << " " << shf[i] << "\n";
}
void createQuery(ofstream& in, const unsigned int n, const unsigned int q)
{
  uniform_int_distribution<unsigned int> dis(1, n);
  for (unsigned int i = 0; i < q; ++i)
    in << dis(default_rnd) << " " << dis(default_rnd) << " " << dis(default_rnd)
       << " " << dis(default_rnd) << "\n";
}
int main(int, char* argv[])
{
  Init();
  const unsigned int n = GetRandom(1, MayBe(atoi(argv[1]), maxn));
  const unsigned int q = GetRandom(1, MayBe(atoi(argv[2]), maxq));
  ofstream in(argv[3]);
  in << n << "\n";
  createTree(in, n);
  in << q << "\n";
  createQuery(in, n, q);
  return 0;
}
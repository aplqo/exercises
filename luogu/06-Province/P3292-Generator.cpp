#include <algorithm>
#include <cstring>
#include <fstream>
#include <functional>
#include <iterator>
#include <random>

#include "debug_tools/generator.h"
using namespace std;
using namespace apdebug::generator;
const int maxn = 20000, maxq = 200000;
const long long maxg = 1ll << 60;

void createG(ofstream& in, const unsigned int n, const unsigned long long mg)
{
  uniform_int_distribution<unsigned long long> dis(0, mg);
  generate_n(ostream_iterator<unsigned long long>(in, " "), n,
             bind(dis, default_rnd));
  in << "\n";
}
void createTree(ofstream& in, const unsigned int n)
{
  for (unsigned int i = 2; i <= n; ++i)
    in << GetRandom(1u, i - 1) << " " << i << "\n";
}
void createQuery(ofstream& in, const unsigned int n, const unsigned int q)
{
  uniform_int_distribution<unsigned int> dis(1, n);
  for (unsigned int i = 0; i < q; ++i)
    in << dis(default_rnd) << " " << dis(default_rnd) << "\n";
}
int main(int, char* argv[])
{
  Init();
  const unsigned int n = GetRandom(2, MayBe(atoi(argv[1]), maxn));
  const unsigned int q = GetRandom(1, MayBe(atoi(argv[2]), maxq));
  ofstream in(argv[4]);
  in << n << " " << q << "\n";
  createG(in, n, MayBe(atoll(argv[3]), maxg));
  createTree(in, n);
  createQuery(in, n, q);
  return 0;
}
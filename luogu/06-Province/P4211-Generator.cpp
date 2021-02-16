#include <algorithm>
#include <fstream>
#include <random>

#include "debug_tools/generator.h"
using namespace std;
using apdebug::generator::GetRandom;
using apdebug::generator::Init;
using apdebug::generator::MayBe;
const int maxn = 50000, maxq = 50000;

mt19937_64 rnd;

void createTree(ostream& f, const unsigned int n)
{
  for (unsigned int i = 1; i < n; ++i)
    f << GetRandom(0u, i - 1) << endl;
}
void createQuery(ofstream& f, const unsigned int n, const unsigned int q)
{
  uniform_int_distribution<unsigned int> dis(0, n - 1);
  for (unsigned int i = 0; i < q; ++i) {
    const unsigned int l = dis(rnd), r = dis(rnd), z = dis(rnd);
    f << min(l, r) << " " << max(l, r) << " " << z << endl;
  }
}
int main(int argc, char* argv[])
{
  Init();
  Init(rnd);
  ofstream in(argv[3]);
  const unsigned int n = GetRandom(1, MayBe(atoi(argv[1]), maxn));
  const unsigned int q = GetRandom(1, MayBe(atoi(argv[2]), maxq));
  in << n << " " << q << endl;
  createTree(in, n);
  createQuery(in, n, q);
  return 0;
}
#include <algorithm>
#include <cstring>
#include <fstream>
#include <random>
#include <set>

#include "debug_tools/generator.h"
using namespace std;
using namespace apdebug::generator;
const int maxn = 300000, maxq = 300000, maxm = 300000;

void createTree(ofstream& in, const unsigned int n)
{
  for (unsigned int i = 2; i <= n; ++i)
    in << GetRandom(1u, i - 1) << " " << i << "\n";
}
void createQuery(ofstream& in, const unsigned int n, const unsigned int q)
{
  uniform_int_distribution<unsigned int> disp(1, n);
  unsigned int sum = maxm;
  for (unsigned int i = 0; i < q; ++i) {
    const unsigned int m = GetRandom(1u, min(n, sum - (q - i - 1)));
    set<unsigned int> seq;
    in << m << "\n";
    for (unsigned int j = 0; j < m; ++j) {
      unsigned int v;
      do {
        v = disp(default_rnd);
      } while (seq.find(v) != seq.end());
      seq.insert(v);
      in << v << " ";
    }
    in << "\n";
  }
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
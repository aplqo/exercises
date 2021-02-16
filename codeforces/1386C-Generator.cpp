#include <fstream>
#include <set>
#include <utility>

#include "debug_tools/generator.h"
using namespace std;
using namespace apdebug::generator;
const int maxn = 200000, maxm = 200000, maxq = 200000;

void createGraph(ostream& os, const unsigned int n, const unsigned int m)
{
  set<pair<unsigned int, unsigned int>> ed;
  uniform_int_distribution<unsigned int> disp(1, n);
  for (unsigned int i = m; i; --i) {
    unsigned int u, v;
    do {
      u = disp(default_rnd);
      v = disp(default_rnd);
      if (u > v) swap(u, v);
    } while (u == v || ed.find(make_pair(u, v)) != ed.end());
    os << u << " " << v << "\n";
    ed.emplace(u, v);
  }
}
void createQuery(ofstream& in, const unsigned int m, const unsigned int q)
{
  uniform_int_distribution<unsigned int> dis(1, m);
  for (unsigned int i = q; i; --i) {
    unsigned int r1 = dis(default_rnd), r2 = dis(default_rnd);
    if (r1 > r2) swap(r1, r2);
    in << r1 << " " << r2 << "\n";
  }
}

int main(int, char* argv[])
{
  Init();
  const unsigned int n = GetRandom(2, MayBe(atoi(argv[1]), maxn));
  const unsigned int m = GetRandom(
      1ull,
      min<unsigned long long>(n * (n - 1ull) / 2, MayBe(atoi(argv[2]), maxm)));
  const unsigned int q = GetRandom(1, MayBe(atoi(argv[3]), maxq));
  ofstream in(argv[4]);
  in << n << " " << m << " " << q << "\n";
  createGraph(in, n, m);
  createQuery(in, m, q);
  return 0;
}
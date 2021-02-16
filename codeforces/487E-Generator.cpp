#include <algorithm>
#include <cstring>
#include <fstream>
#include <functional>
#include <iterator>
#include <set>
#include <utility>

#include "debug_tools/generator.h"
using namespace std;
using namespace apdebug::generator;
const int maxn = 1e5, maxm = 1e5, maxq = 1e5, maxw = 1e5;

void createW(ofstream& in, const unsigned int n, const unsigned int mw)
{
  uniform_int_distribution<unsigned int> disw(1, mw);
  generate_n(ostream_iterator<unsigned int>(in, " "), n,
             bind(disw, default_rnd));
  in.put('\n');
}
bool addEdge(unsigned int from, unsigned int to)
{
  static set<pair<unsigned int, unsigned int>> eds;
  if (from > to) swap(from, to);
  const auto pe = make_pair(from, to);
  if (eds.find(pe) != eds.end()) return false;
  eds.insert(pe);
  return true;
}
void createTree(ofstream& in, const unsigned int n)
{
  for (unsigned int i = 2; i <= n; ++i) {
    const unsigned int s = GetRandom(1u, i - 1);
    in << s << " " << i << "\n";
    addEdge(s, i);
  }
}
void createGraph(ofstream& in, const unsigned int n, const unsigned int m)
{
  createTree(in, n);
  uniform_int_distribution<unsigned int> disv(1, n);
  for (unsigned int i = n - 1; i < m; ++i) {
    unsigned int a, b;
    do {
      a = disv(default_rnd);
      b = disv(default_rnd);
    } while (a == b || !addEdge(a, b));
    in << a << " " << b << "\n";
  }
}
void createQuery(ofstream& in, const unsigned int n, const unsigned int q,
                 const unsigned int mw)
{
  uniform_int_distribution<unsigned int> disv(1, n), disop(0, 1), disw(1, mw);
  for (unsigned int i = 0; i < q; ++i) {
    const unsigned int op = disop(default_rnd);
    if (op == 1)
      in << "C " << disv(default_rnd) << " " << disw(default_rnd) << "\n";
    else
      in << "A " << disv(default_rnd) << " " << disv(default_rnd) << "\n";
  }
}

unsigned long long inRange(unsigned long long v, const unsigned long long l,
                           const unsigned long long r)
{
  return max(l, min(v, r));
}
int main(int, char* argv[])
{
  Init();
  const unsigned int n = GetRandom(1, MayBe(atoi(argv[1]), maxn));
  const unsigned int m = GetRandom(
      n - 1ull,
      inRange(MayBe(atoi(argv[2]), maxm), n - 1, 1ull * n * (n - 1) / 2));
  const unsigned int q = GetRandom(1, MayBe(atoi(argv[3]), maxq));
  const unsigned int mw = MayBe(atoi(argv[4]), maxw);
  ofstream in(argv[5]);
  in << n << " " << m << " " << q << "\n";
  createW(in, n, mw);
  createGraph(in, n, m);
  createQuery(in, n, q, mw);
  return 0;
}
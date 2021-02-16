#include <algorithm>
#include <fstream>
#include <numeric>
#include <random>

#include "debug_tools/generator.h"
using namespace std;
using apdebug::generator::GetRandom;
using apdebug::generator::Init;
using apdebug::generator::MayBe;
const int maxn = 1e5, maxq = 1e5;

struct edge {
  unsigned int from, to;
  edge* pre = nullptr;
} ed[maxn * 2 + 1], *cur = ed;
edge* head[maxn + 1];
mt19937_64 rnd;
unsigned int fa[maxn + 1], shf[maxn + 1], raw[maxn + 1];

void addEdge(const unsigned int from, const unsigned int to)
{
  cur->from = from;
  cur->to = to;
  cur->pre = head[from];
  head[from] = cur;
  ++cur;
}
void dfs(const unsigned int x, const unsigned int f)
{
  fa[x] = f;
  for (edge* i = head[x]; i; i = i->pre)
    if (!fa[i->to]) dfs(i->to, x);
}
void createTree(ofstream& f, const unsigned int n)
{
  for (unsigned int i = 1; i < n; ++i) {
    const unsigned int t = GetRandom(0u, i - 1);
    addEdge(i, t);
    addEdge(t, i);
  }
  iota(shf, shf + n, 0);
  shuffle(shf, shf + n, rnd);
  for (unsigned int i = 0; i < n; ++i)
    raw[shf[i]] = i;
  dfs(raw[0], raw[0]);
  for (unsigned int i = 1; i < n; ++i)
    f << shf[fa[raw[i]]] << " ";
  f << endl;
}
void createQuery(ofstream& f, const unsigned int n, const unsigned int q)
{
  uniform_int_distribution<unsigned int> disop(0, 1), disr(0, n - 1);
  for (unsigned int i = 0; i < q; ++i)
    f << (disop(rnd) ? "uninstall" : "install") << " " << disr(rnd) << endl;
  f << endl;
}

int main(int argc, char* argv[])
{
  Init();
  Init(rnd);
  const unsigned int n = GetRandom(2, MayBe(atoi(argv[1]), maxn));
  const unsigned int q = GetRandom(1, MayBe(atoi(argv[2]), maxq));
  ofstream in(argv[3]);
  in << n << endl;
  createTree(in, n);
  in << q << endl;
  createQuery(in, n, q);
  return 0;
}
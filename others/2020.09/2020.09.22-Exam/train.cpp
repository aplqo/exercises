#ifdef APTEST
#include "debug_tools/program.h"
#endif
#include <algorithm>
#include <cctype>
#include <cstdio>
#include <iostream>
#include <iterator>
using namespace std;
const unsigned int maxn = 500000, maxm = 400000;

struct Edge {
  unsigned int to;
  const Edge* pre;
} ed[maxn * 2 + 10];
const Edge* head[maxn + 10];
class FTree {
 public:
  void increase(const unsigned int l, const unsigned int r);
  unsigned int query(unsigned int pos) const;
  unsigned int n;

 private:
  void update(unsigned int pos, const int delt);
  int val[maxn + 10] = {};
} ft;
unsigned int son[maxn + 10], fa[maxn + 10], top[maxn + 10], dep[maxn + 10],
    dfn[maxn + 10];
unsigned int vert[maxm + 10];

inline void addEdge(const unsigned int from, const unsigned int to)
{
  static Edge* cur = ed;
  new (cur) Edge{to, head[from]};
  head[from] = cur++;
  new (cur) Edge{from, head[to]};
  head[to] = cur++;
}
#define lowbit(x) ((x) & -(x))
void FTree::increase(const unsigned int l, const unsigned int r)
{
  update(l, 1);
  update(r, -1);
}
unsigned int FTree::query(unsigned int pos) const
{
  int ret = 0;
  for (; pos; pos -= lowbit(pos))
    ret += val[pos];
  return ret;
}
void FTree::update(unsigned int pos, const int delt)
{
  for (; pos <= n; pos += lowbit(pos))
    val[pos] += delt;
}
#undef lowbit

unsigned int dfsSon(const unsigned int x, const unsigned int f)
{
  dep[x] = dep[f] + 1;
  fa[x] = f;
  unsigned int ret = 1, ms = 0;
  for (const Edge* i = head[x]; i; i = i->pre) {
    const unsigned int to = i->to;
    if (to == f) continue;
    const unsigned int sz = dfsSon(to, x);
    ret += sz;
    if (sz > ms) {
      ms = sz;
      son[x] = to;
    }
  }
  return ret;
}
void dfsChain(const unsigned int x, const unsigned int tp)
{
  static unsigned int sta = 0;
  top[x] = tp;
  dfn[x] = ++sta;
  if (son[x]) dfsChain(son[x], tp);
  for (const Edge* i = head[x]; i; i = i->pre)
    if (!dfn[i->to]) dfsChain(i->to, i->to);
}
static void init(const unsigned int n)
{
  dfsSon(1, 0);
  dfsChain(1, 1);
  ft.n = n;
}
unsigned int lca(unsigned int a, unsigned int b)
{
  while (top[a] != top[b]) {
    if (dep[top[a]] > dep[top[b]]) swap(a, b);
    b = fa[top[b]];
  }
  return dep[a] < dep[b] ? a : b;
}
inline unsigned int distance(const unsigned int a, const unsigned int b)
{
  return dep[a] + dep[b] - 2 * dep[lca(a, b)];
}
void clear(unsigned int a, unsigned int b)
{
  while (top[a] != top[b]) {
    if (dep[top[a]] > dep[top[b]]) swap(a, b);
    ft.increase(dfn[top[b]], dfn[b] + 1);
    b = fa[top[b]];
  }
  if (dep[a] > dep[b]) swap(a, b);
  ft.increase(dfn[a], dfn[b] + 1);
}

unsigned long long GetDist(const unsigned int beg, const unsigned int m)
{
  unsigned int x = beg;
  unsigned long long ret = 0;
  for (const unsigned int* i = vert; i < vert + m; ++i) {
    if (ft.query(dfn[*i])) continue;
    ret += distance(x, *i);
    clear(x, *i);
    x = *i;
  }
  return ret;
}
int main()
{
#ifndef APTEST
  ios_base::sync_with_stdio(false);
#endif
  unsigned int n, m, a;
  cin >> n >> m >> a;
  for (unsigned int i = 1; i < n; ++i) {
    unsigned int x, y;
    cin >> x >> y;
    addEdge(x, y);
  }
  init(n);
  copy_n(istream_iterator<unsigned int>(cin), m, vert);
  cout << GetDist(a, m) << "\n";
  return 0;
}
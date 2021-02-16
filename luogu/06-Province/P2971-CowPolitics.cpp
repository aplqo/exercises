#ifdef APTEST
#include "debug_tools/program.h"
#endif
#include <algorithm>
#include <iostream>
#include <iterator>
using namespace std;
const unsigned int maxn = 200000, maxk = maxn / 2;

struct Edge {
  unsigned int to;
  const Edge* pre;
} ed[maxn + 10];
const Edge* head[maxn + 1];
unsigned int son[maxn + 1], top[maxn + 1], fa[maxn + 1], dep[maxn + 1];
unsigned int typ[maxn + 1], maxD[maxk + 1], dist[maxk + 1];

inline void addEdge(const unsigned int from, const unsigned int to)
{
  static Edge* cur = ed;
  new (cur) Edge{to, head[from]};
  head[from] = cur++;
}
unsigned int dfsSon(const unsigned int x, const unsigned int f)
{
  fa[x] = f;
  dep[x] = dep[f] + 1;
  if (dep[x] > dep[maxD[typ[x]]]) maxD[typ[x]] = x;
  unsigned int ret = 1, ms = 0;
  for (const Edge* i = head[x]; i; i = i->pre) {
    const unsigned int to = i->to;
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
  top[x] = tp;
  if (son[x]) dfsChain(son[x], tp);
  for (const Edge* i = head[x]; i; i = i->pre)
    if (!top[i->to]) dfsChain(i->to, i->to);
}
unsigned int lca(unsigned int a, unsigned int b)
{
  while (top[a] != top[b]) {
    if (dep[top[a]] > dep[top[b]]) swap(a, b);
    b = fa[top[b]];
  }
  return dep[a] < dep[b] ? a : b;
}
unsigned int distance(const unsigned int a, const unsigned int b)
{
  return dep[a] + dep[b] - 2 * dep[lca(a, b)];
}
void solve(const unsigned int n)
{
  for (unsigned int i = 1; i <= n; ++i)
    dist[typ[i]] = max(dist[typ[i]], distance(i, maxD[typ[i]]));
}
int main()
{
#ifndef APTEST
  ios_base::sync_with_stdio(false);
#endif
  unsigned int n, k, root;
  cin >> n >> k;
  for (unsigned int i = 1; i <= n; ++i) {
    cin >> typ[i] >> fa[i];
    if (fa[i])
      addEdge(fa[i], i);
    else
      root = i;
  }
  dfsSon(root, 0);
  dfsChain(root, root);
  solve(n);
  copy(dist + 1, dist + 1 + k, ostream_iterator<unsigned int>(cout, "\n"));
  return 0;
}
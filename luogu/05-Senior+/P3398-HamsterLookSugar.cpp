#ifdef APTEST
#include "debug_tools/program.h"
#endif
#include <algorithm>
#include <iostream>
using namespace std;
const unsigned int maxn = 1e5;

struct Edge {
  unsigned int to;
  const Edge* pre;
} ed[maxn * 2 + 10];
const Edge* head[maxn + 10];
unsigned int dep[maxn + 10], top[maxn + 10], fa[maxn + 10], son[maxn + 10];
unsigned int discover[maxn + 10], finish[maxn + 10];

inline void addEdge(const unsigned int from, const unsigned int to)
{
  static Edge* cur = ed;
  new (cur) Edge{to, head[from]};
  head[from] = cur++;
  new (cur) Edge{from, head[to]};
  head[to] = cur++;
}
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
  discover[x] = ++sta;
  top[x] = tp;
  if (son[x]) dfsChain(son[x], tp);
  for (const Edge* i = head[x]; i; i = i->pre)
    if (!top[i->to]) dfsChain(i->to, i->to);
  finish[x] = ++sta;
}
inline bool isInSubTree(const unsigned int x, const unsigned int rt)
{
  return discover[rt] <= discover[x] && finish[x] <= finish[rt];
}
unsigned int lca(unsigned int a, unsigned int b)
{
  while (top[a] != top[b]) {
    if (dep[top[a]] > dep[top[b]]) swap(a, b);
    b = fa[top[b]];
  }
  return dep[a] < dep[b] ? a : b;
}
bool isIntersect(const unsigned int a1, const unsigned int a2,
                 const unsigned int b1, const unsigned int b2)
{
  const unsigned int lc1 = lca(a1, a2), lc2 = lca(b1, b2);
  return (isInSubTree(a1, lc2) ^ isInSubTree(a2, lc2)) ||
         (isInSubTree(b1, lc1) ^ isInSubTree(b2, lc1)) || lc1 == lc2;
}
int main()
{
#ifndef APTEST
  ios_base::sync_with_stdio(false);
#endif
  unsigned int n, q;
  cin >> n >> q;
  for (unsigned int i = 1; i < n; ++i) {
    unsigned int u, v;
    cin >> u >> v;
    addEdge(u, v);
  }
  dfsSon(1, 0);
  dfsChain(1, 1);
  for (unsigned int i = 0; i < q; ++i) {
    unsigned int a, b, c, d;
    cin >> a >> b >> c >> d;
    cout << (isIntersect(a, b, c, d) ? "Y" : "N") << "\n";
  }
  return 0;
}
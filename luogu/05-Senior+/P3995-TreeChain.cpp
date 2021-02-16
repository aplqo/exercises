#ifdef APTEST
#include "debug_tools/program.h"
#endif
#include <algorithm>
#include <climits>
#include <iostream>
using namespace std;
const unsigned int maxn = 1e5;

struct edge {
  unsigned int to;
  edge* pre;
} ed[maxn * 2 + 1];
edge* head[maxn + 1];
int val[maxn + 1];
unsigned int ans[maxn + 1];
unsigned int dep[maxn + 1], son[maxn + 1], fa[maxn + 1], top[maxn + 1];

void addEdge(const unsigned int from, const unsigned int to)
{
  static edge* cur = ed;
  cur->to = to;
  cur->pre = head[from];
  head[from] = cur;
  ++cur;
}

unsigned int dfsSon(const unsigned int x, const unsigned int f)
{
  fa[x] = f;
  dep[x] = dep[f] + 1;
  unsigned int ret = 1, s = 0;
  for (edge* i = head[x]; i; i = i->pre) {
    const unsigned int to = i->to;
    if (to == f) continue;
    const unsigned int sz = dfsSon(to, x);
    ret += sz;
    if (sz > s) {
      s = sz;
      son[x] = to;
    }
  }
  return ret;
}
void dfsChain(const unsigned int x, const unsigned int tp)
{
  top[x] = tp;
  if (son[x]) dfsChain(son[x], tp);
  for (edge* i = head[x]; i; i = i->pre)
    if (!top[i->to]) dfsChain(i->to, i->to);
}
inline unsigned int lca(unsigned int x, unsigned int y)
{
  while (top[x] != top[y]) {
    if (dep[top[x]] > dep[top[y]])
      x = fa[top[x]];
    else
      y = fa[top[y]];
  }
  return dep[x] > dep[y] ? y : x;
}
int dfs(const unsigned int x, const unsigned int f)
{
  for (edge* i = head[x]; i; i = i->pre)
    if (i->to != f) val[x] += dfs(i->to, x);
  return val[x];
}
void dfsAns(const unsigned int x, const unsigned int f)
{
  int md = INT_MIN;
  for (edge* i = head[x]; i; i = i->pre) {
    const unsigned int to = i->to;
    if (to == f) continue;
    dfsAns(to, x);
    val[x] += val[to];
    if (val[to] > md) {
      md = val[to];
      ans[x] = to;
    }
  }
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
    addEdge(v, u);
  }
  dfsSon(1, 0);
  dfsChain(1, 1);
  fill(val, val + 1 + n, 1);
  for (unsigned int i = 0; i < q; ++i) {
    unsigned int u, v;
    cin >> u >> v;
    const unsigned int l = lca(u, v);
    if (fa[u] != l) {
      ++val[u];
      --val[l];
    }
    if (fa[v] != l) {
      ++val[v];
      --val[l];
    }
  }
  dfs(1, 0);
  dfsAns(1, 0);
  for (unsigned int i = 1; i <= n; ++i)
    cout << ans[i] << endl;
  return 0;
}
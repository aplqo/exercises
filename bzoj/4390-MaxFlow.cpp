#ifdef APTEST
#include "debug_tools/program.h"
#endif
#include <algorithm>
#include <iostream>
using namespace std;
const unsigned int maxn = 50000;

struct Edge {
  unsigned int to;
  const Edge* pre;
} ed[maxn * 2 + 10];
const Edge* head[maxn + 10];
int val[maxn + 10];

void addEdge(const unsigned int from, const unsigned int to)
{
  static Edge* cur = ed;
  new (cur) Edge{to, head[from]};
  head[from] = cur++;
  new (cur) Edge{from, head[to]};
  head[to] = cur++;
}
namespace LCA {
unsigned int fa[maxn + 10], dep[maxn + 10], son[maxn + 10], top[maxn + 10];
unsigned int dfsSon(const unsigned int x, const unsigned int f)
{
  dep[x] = dep[f] + 1;
  fa[x] = f;
  unsigned int ms = 0, ret = 1;
  for (const Edge* i = head[x]; i; i = i->pre) {
    const unsigned int to = i->to;
    if (to == f) continue;
    const unsigned int sz = dfsSon(to, x);
    ret += sz;
    if (sz > ms) {
      son[x] = to;
      ms = sz;
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
static void init(const unsigned int p)
{
  dfsSon(p, 0);
  dfsChain(p, p);
}
unsigned int lca(unsigned int a, unsigned int b)
{
  while (top[a] != top[b]) {
    if (dep[top[a]] > dep[top[b]]) swap(a, b);
    b = fa[top[b]];
  }
  return dep[a] < dep[b] ? a : b;
}
}  // namespace LCA
unsigned int dfs(const unsigned int x, const unsigned int f)
{
  unsigned int ret = 0;
  for (const Edge* i = head[x]; i; i = i->pre) {
    const unsigned int to = i->to;
    if (to != f) {
      ret = max(ret, dfs(to, x));
      val[x] += val[to];
    }
  }
  return max<unsigned int>(ret, val[x]);
}

int main()
{
#ifndef APTEST
  ios_base::sync_with_stdio(false);
#endif
  unsigned int n, k;
  cin >> n >> k;
  for (unsigned int i = 1; i < n; ++i) {
    unsigned int x, y;
    cin >> x >> y;
    addEdge(x, y);
  }
  LCA::init(1);
  for (unsigned int i = 0; i < k; ++i) {
    unsigned int x, y;
    cin >> x >> y;
    const unsigned int l = LCA::lca(x, y);
    ++val[x];
    ++val[y];
    --val[l];
    --val[LCA::fa[l]];
  }
  cout << dfs(1, 0) << "\n";
  return 0;
}
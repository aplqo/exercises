#ifdef APTEST
#include "debug_tools/program.h"
#endif
#include <algorithm>
#include <iostream>
using namespace std;
const unsigned int maxn = 2e5, maxm = 2e5;

struct Edge {
  unsigned int to;
  const Edge* pre;
} ed[maxn * 2 + 10];
const Edge* head[maxn + 10];
struct Business {
  unsigned int u, v, lca;
  unsigned int ret;
} bus[maxm + 10];
int fLca[maxn + 10], chainLca[maxn + 10];

inline void addEdge(const unsigned int from, const unsigned int to)
{
  static Edge* cur = ed;
  new (cur) Edge{to, head[from]};
  head[from] = cur++;
  new (cur) Edge{from, head[to]};
  head[to] = cur++;
}
namespace LCA {
unsigned int son[maxn + 10], dep[maxn + 10], top[maxn + 10], fa[maxn + 10];

unsigned int dfsSon(const unsigned int x, const unsigned int f)
{
  fa[x] = f;
  dep[x] = dep[f] + 1;
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
  top[x] = tp;
  if (son[x]) dfsChain(son[x], tp);
  for (const Edge* i = head[x]; i; i = i->pre)
    if (!top[i->to]) dfsChain(i->to, i->to);
}
static void init()
{
  dfsSon(1, 0);
  dfsChain(1, 1);
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
void dfs(const unsigned int x, const unsigned int f)
{
  for (const Edge* i = head[x]; i; i = i->pre)
    if (i->to != f) {
      fLca[i->to] += fLca[x];
      dfs(i->to, x);
      chainLca[x] += chainLca[i->to];
    }
}
void answerQuery(const unsigned int m)
{
  static unsigned int cnt[maxn + 10];
  LCA::init();
  for (Business* i = bus; i < bus + m; ++i) {
    const unsigned int lc = LCA::lca(i->u, i->v);
    i->lca = lc;
    ++fLca[lc];
    ++chainLca[i->u];
    ++chainLca[i->v];
    --chainLca[lc];
    --chainLca[LCA::fa[lc]];
    ++cnt[lc];
  }
  dfs(1, 0);
  for (Business* i = bus; i < bus + m; ++i)
    i->ret = fLca[i->u] + fLca[i->v] - fLca[i->lca] - fLca[LCA::fa[i->lca]] +
             chainLca[i->lca] - cnt[i->lca] - 1;
}

int main()
{
#ifndef APTEST
  ios_base::sync_with_stdio(false);
#endif
  unsigned int n, m;
  cin >> n >> m;
  for (unsigned int i = 1; i < n; ++i) {
    unsigned int u, v;
    cin >> u >> v;
    addEdge(u, v);
  }
  for (Business* i = bus; i < bus + m; ++i)
    cin >> i->u >> i->v;
  answerQuery(m);
  for (const Business* i = bus; i < bus + m; ++i)
    cout << i->ret << "\n";
  return 0;
}
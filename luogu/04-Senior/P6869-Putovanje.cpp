#ifdef APTEST
#include "debug_tools/program.h"
#endif
#include <algorithm>
#include <iostream>
using namespace std;
const unsigned int maxn = 200000;

struct Edge {
  unsigned int id, to;
  const Edge* pre;
} ed[maxn * 2 + 10];
const Edge *head[maxn + 10], *fromEd[maxn + 10];
unsigned long long one[maxn + 10], multi[maxn + 10];
int cnt[maxn + 10];

inline void addEdge(const unsigned int from, const unsigned int to,
                    const unsigned int c1, const unsigned int c2)
{
  static Edge* cur = ed;
  static unsigned int sta = 0;
  new (cur) Edge{sta, to, head[from]};
  head[from] = cur++;
  new (cur) Edge{sta, from, head[to]};
  head[to] = cur++;
  one[sta] = c1;
  multi[sta] = c2;
  ++sta;
}
namespace HLD {
unsigned int son[maxn + 10], top[maxn + 10], fa[maxn + 10], dep[maxn + 10];

unsigned int dfsSon(const unsigned int x, const unsigned int f)
{
  fa[x] = f;
  dep[x] = dep[f] + 1;
  unsigned int ret = 1, ms = 0;
  for (const Edge* i = head[x]; i; i = i->pre) {
    const unsigned int to = i->to;
    if (to == f) continue;
    fromEd[to] = i;
    const unsigned int sz = dfsSon(to, x);
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
}  // namespace HLD
void dfs(const unsigned int x, const unsigned int f)
{
  for (const Edge* i = head[x]; i; i = i->pre)
    if (i->to != f) {
      dfs(i->to, x);
      cnt[x] += cnt[i->to];
    }
}
void markEdge(const unsigned int n)
{
  HLD::init();
  for (unsigned int i = 2; i <= n; ++i) {
    ++cnt[i];
    ++cnt[i - 1];
    cnt[HLD::lca(i, i - 1)] -= 2;
  }
  dfs(1, 0);
}
unsigned long long solve(const unsigned int n)
{
  markEdge(n);
  unsigned long long ret = 0;
  for (unsigned int i = 2; i <= n; ++i)
    ret += min(cnt[i] * (one[fromEd[i]->id]), multi[fromEd[i]->id]);
  return ret;
}

int main()
{
#ifndef APTEST
  ios_base::sync_with_stdio(false);
#endif
  unsigned int n;
  cin >> n;
  for (unsigned int i = 1; i < n; ++i) {
    unsigned int a, b, c, d;
    cin >> a >> b >> c >> d;
    addEdge(a, b, c, d);
  }
  cout << solve(n) << "\n";
  return 0;
}
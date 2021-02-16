#ifdef APTEST
#include "debug_tools/program.h"
#endif
#include <algorithm>
#include <cstring>
#include <functional>
#include <iostream>
#include <iterator>
#include <vector>
using namespace std;
const unsigned int maxn = 3e5;

struct edge {
  unsigned int to;
  edge* pre;
} ed[maxn * 2 + 10];
edge* head[maxn + 1];
unsigned int fa[maxn + 1], top[maxn + 1], son[maxn + 1];
int dep[maxn + 1];
struct player {
  unsigned int s, t;
  unsigned int l;
} p[maxn + 1];
int w[maxn + 1];
vector<int> start[maxn + 1], stop[maxn + 1];
unsigned int ans[maxn + 1];
unsigned int cnt[maxn * 4 + 1], *off = cnt + maxn * 2;

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
  unsigned int ret = 1, s = 0;
  dep[x] = dep[f] + 1;
  fa[x] = f;
  for (edge* i = head[x]; i; i = i->pre) {
    const unsigned int to = i->to;
    if (to == f) continue;
    const unsigned int sz = dfsSon(to, x);
    ret += sz;
    if (sz > s) {
      son[x] = to;
      s = sz;
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
unsigned int lca(unsigned int x, unsigned int y)
{
  while (top[x] != top[y]) {
    if (dep[top[x]] > dep[top[y]])
      x = fa[top[x]];
    else
      y = fa[top[y]];
  }
  return dep[x] > dep[y] ? y : x;
}
template <class f1>
void dfs(const unsigned int x, const unsigned int f, const f1 pred)
{
  const static auto index = [](const int p) -> unsigned int& {
    return *(off + p);
  };
  unsigned int& p = index(pred(x));
  const unsigned int bef = p;
  for (auto i : start[x])
    ++index(i);
  for (auto i : stop[x])
    --index(i);
  for (edge* i = head[x]; i; i = i->pre)
    if (i->to != f) dfs(i->to, x, pred);
  ans[x] += p - bef;
}
void solve(const unsigned int n, const unsigned int m)
{
  for (const auto* i = p; i < p + m; ++i) {
    const int ds = dep[i->s];
    start[i->s].push_back(ds);
    stop[fa[i->l]].push_back(ds);
  }
  dfs(1, 0, [](const unsigned int x) { return dep[x] + w[x]; });

  memset(cnt, 0, sizeof(cnt));
  for_each(start, start + n + 1, mem_fn(&vector<int>::clear));
  for_each(stop, stop + 1 + n, mem_fn(&vector<int>::clear));
  for (const auto* i = p; i < p + m; ++i) {
    const int v = dep[i->s] - 2 * dep[i->l];
    start[i->t].push_back(v);
    stop[i->l].push_back(v);
  }
  dfs(1, 0, [](const unsigned int x) { return w[x] - dep[x]; });
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
    addEdge(v, u);
  }
  copy_n(istream_iterator<unsigned int>(cin), n, w + 1);
  dfsSon(1, 0);
  dfsChain(1, 1);
  for (auto* i = p; i < p + m; ++i) {
    cin >> i->s >> i->t;
    i->l = lca(i->s, i->t);
  }
  solve(n, m);
  copy(ans + 1, ans + 1 + n, ostream_iterator<unsigned int>(cout, " "));
  cout << endl;
  return 0;
}
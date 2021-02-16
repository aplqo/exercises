#ifdef APTEST
#include "debug_tools/program.h"
#endif
#include <algorithm>
#include <iostream>
#include <iterator>
using namespace std;
const unsigned int maxn = 300000;

struct edge {
  unsigned int to;
  const edge* pre;
} ed[maxn * 2];
unsigned int son[maxn + 1], fa[maxn + 1], top[maxn + 1], dep[maxn + 1];
const edge* head[maxn + 1];
unsigned int a[maxn + 1];
long long val[maxn + 1];

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
  unsigned int ret = 1, ms = 0;
  dep[x] = dep[f] + 1;
  fa[x] = f;
  for (const edge* i = head[x]; i; i = i->pre) {
    const unsigned int to = i->to;
    if (to == f) continue;
    const unsigned int siz = dfsSon(to, x);
    ret += siz;
    if (siz > ms) {
      ms = siz;
      son[x] = to;
    }
  }
  return ret;
}
void dfsChain(const unsigned int x, const unsigned int tp)
{
  top[x] = tp;
  if (son[x]) dfsChain(son[x], tp);
  for (const edge* i = head[x]; i; i = i->pre)
    if (!top[i->to]) dfsChain(i->to, i->to);
}
unsigned int lca(unsigned int a, unsigned int b)
{
  while (top[a] != top[b]) {
    if (dep[top[a]] > dep[top[b]])
      a = fa[top[a]];
    else
      b = fa[top[b]];
  }
  return dep[a] > dep[b] ? b : a;
}
void dfs(const unsigned int x, const unsigned int f)
{
  for (const edge* i = head[x]; i; i = i->pre)
    if (i->to != f) {
      dfs(i->to, x);
      val[x] += val[i->to];
    }
}

int main()
{
#ifndef APTEST
  ios_base::sync_with_stdio(false);
#endif
  unsigned int n;
  cin >> n;
  copy_n(istream_iterator<unsigned int>(cin), n, a);
  for (unsigned int i = 1; i < n; ++i) {
    unsigned int x, y;
    cin >> x >> y;
    addEdge(x, y);
    addEdge(y, x);
  }
  dfsSon(1, 0);
  dfsChain(1, 1);
  for (unsigned int i = 1; i < n; ++i) {
    const unsigned int l = lca(a[i], a[i - 1]);
    ++val[fa[a[i]]];
    ++val[a[i - 1]];
    --val[l];
    --val[fa[l]];
  }
  dfs(1, 0);
  copy_n(val + 1, n, ostream_iterator<long long>(cout, "\n"));
  return 0;
}
#ifdef APTEST
#include "debug_tools/program.h"
#endif
#include <algorithm>
#include <climits>
#include <iostream>
#include <iterator>
using namespace std;
const unsigned int maxn = 100000, maxm = 100000;
const unsigned int maxv = maxm, maxe = maxn * 2;

struct Edge {
  unsigned int from, to;
  const Edge* pre;
} ed[maxe + 1], *cur = ed;
const Edge* head[maxv + 1];
unsigned int tim[maxv + 1];
unsigned int dfn[maxv + 1], low[maxv + 1], scc[maxv + 1], sen;
unsigned int degree[maxv + 1], siz[maxv + 1];

inline void addEdge(const unsigned int from, const unsigned int to)
{
  new (cur) Edge{from, to, head[from]};
  head[from] = cur++;
}
void tarjan(const unsigned int x)
{
  static unsigned int stk[maxv + 1], *top = stk;
  static unsigned int sta = 0;
  dfn[x] = low[x] = ++sta;
  *(top++) = x;
  for (const Edge* i = head[x]; i; i = i->pre) {
    const unsigned int to = i->to;
    if (!dfn[to]) {
      tarjan(to);
      low[x] = min(low[x], low[to]);
    }
    else if (!scc[to])
      low[x] = min(low[x], dfn[to]);
  }
  if (dfn[x] == low[x]) {
    ++sen;
    while (*(top - 1) != x) {
      scc[*(top - 1)] = sen;
      ++siz[sen];
      --top;
    }
    scc[x] = sen;
    ++siz[sen];
    --top;
  }
}
static void buildGraph(const unsigned int n, const unsigned int h)
{
  for (unsigned int i = 0; i < n; ++i) {
    unsigned int u, v;
    cin >> u >> v;
    if ((tim[u] + 1) % h == tim[v]) addEdge(u, v);
    if ((tim[v] + 1) % h == tim[u]) addEdge(v, u);
  }
}
void buildDAG()
{
  for (const Edge* i = ed; i < cur; ++i)
    if (scc[i->from] != scc[i->to]) ++degree[scc[i->from]];
}
unsigned int findScc()
{
  unsigned int ret = 0, ms = UINT_MAX;
  for (unsigned int i = 1; i <= sen; ++i)
    if (siz[i] < ms && !degree[i]) {
      ret = i;
      ms = siz[i];
    }
  return ret;
}
int main()
{
#ifndef APTEST
  ios_base::sync_with_stdio(false);
#endif
  unsigned int n, m, h;
  cin >> m >> n >> h;
  copy_n(istream_iterator<unsigned int>(cin), m, tim + 1);
  buildGraph(n, h);
  for (unsigned int i = 1; i <= m; ++i)
    if (!dfn[i]) tarjan(i);
  buildDAG();
  const unsigned int p = findScc();
  cout << siz[p] << "\n";
  for (unsigned int i = 1; i <= m; ++i)
    if (scc[i] == p) cout << i << " ";
  cout << "\n";
  return 0;
}
#ifdef APTEST
#include "debug_tools/program.h"
#endif
#include <algorithm>
#include <climits>
#include <functional>
#include <iostream>
#include <iterator>
#include <type_traits>
using namespace std;
const unsigned int maxn = 100000, maxm = 500000;
constexpr unsigned int inf = UINT_MAX / 2;

enum { Min = 0, Max = 1 };
class Graph {
 public:
  struct Edge {
    unsigned int from, to;
    const Edge* pre;
  } ed[maxm * 2 + 10], *cur = ed;
  const Edge* head[maxn + 10] = {};

  inline void addEdge(const unsigned int from, const unsigned int to)
  {
    new (cur) Edge{from, to, head[from]};
    head[from] = cur++;
  }
} orig;
Graph dag, rdag;
unsigned int val[maxn + 1];
unsigned int dfn[maxn + 1], low[maxn + 1], scc[maxn + 1];
unsigned int mv[maxn + 1][2], sen = 0;
bool reach[maxn + 1][2];

void tarjan(const unsigned int x)
{
  static unsigned int sta = 0;
  static unsigned int stk[maxn + 1], *top = stk;
  dfn[x] = low[x] = ++sta;
  *(top++) = x;
  for (auto i = orig.head[x]; i; i = i->pre) {
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
    mv[sen][Min] = inf;
    mv[sen][Max] = 0;
    while (*(top - 1) != x) {
      mv[sen][Min] = min(mv[sen][Min], val[*(top - 1)]);
      mv[sen][Max] = max(mv[sen][Max], val[*(top - 1)]);
      scc[*(top - 1)] = sen;
      --top;
    }
    mv[sen][Min] = min(mv[sen][Min], val[x]);
    mv[sen][Max] = max(mv[sen][Max], val[x]);
    scc[x] = sen;
    --top;
  }
}
void buildGraph()
{
  for (auto i = orig.ed; i < orig.cur; ++i)
    if (scc[i->from] != scc[i->to]) {
      dag.addEdge(scc[i->from], scc[i->to]);
      rdag.addEdge(scc[i->to], scc[i->from]);
    }
}
template <unsigned int typ>
void topological(const Graph& g, const unsigned int beg)
{
  static conditional_t<typ, plus<unsigned int>, minus<unsigned int>> nxt;
  static conditional_t<typ, greater<unsigned int>, less<unsigned int>> cmp;
  reach[beg][typ] = true;
  for (unsigned int i = beg; i <= sen && i > 0; i = nxt(i, 1)) {
    if (!reach[i][typ]) continue;
    for (auto j = g.head[i]; j; j = j->pre) {
      mv[j->to][typ] = min(mv[j->to][typ], mv[i][typ], cmp);
      reach[j->to][typ] = true;
    }
  }
}
unsigned int findMax()
{
  unsigned int ret = 0;
  for (unsigned int i = 1; i <= sen; ++i)
    if (reach[i][0] && reach[i][1]) ret = max(ret, mv[i][Max] - mv[i][Min]);
  return ret;
}

int main()
{
#ifndef APTEST
  ios_base::sync_with_stdio(false);
#endif
  unsigned int n, m;
  cin >> n >> m;
  copy_n(istream_iterator<unsigned int>(cin), n, val + 1);
  for (unsigned int i = 0; i < m; ++i) {
    unsigned int x, y, z;
    cin >> x >> y >> z;
    orig.addEdge(x, y);
    if (z == 2) orig.addEdge(y, x);
  }
  for (unsigned int i = 1; i <= n; ++i)
    if (!dfn[i]) tarjan(i);
  buildGraph();
  topological<Max>(rdag, scc[n]);
  topological<Min>(dag, scc[1]);
  cout << findMax() << "\n";
  return 0;
}

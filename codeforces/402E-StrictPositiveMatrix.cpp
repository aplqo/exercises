#ifdef APTEST
#include "debug_tools/program.h"
#endif
#include <algorithm>
#include <iostream>
using namespace std;
const unsigned int maxn = 2000;
const unsigned int maxv = maxn, maxe = maxn * maxn;

struct Edge {
  unsigned int to;
  const Edge* pre;
} ed[maxe + 1];
const Edge* head[maxv + 1];
unsigned int dfn[maxv + 1], low[maxv + 1];
unsigned int scc[maxv + 1], sen = 0;
unsigned int siz[maxv + 1];

inline void addEdge(const unsigned int from, const unsigned int to)
{
  static Edge* cur = ed;
  new (cur) Edge{to, head[from]};
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
      scc[*(top--)] = sen;
      ++siz[sen];
    }
    scc[x] = sen;
    ++siz[sen];
  }
}

int main()
{
#ifndef APTEST
  ios_base::sync_with_stdio(false);
#endif
  unsigned int n;
  cin >> n;
  for (unsigned int i = 0; i < n; ++i)
    for (unsigned int j = 0; j < n; ++j) {
      unsigned int p;
      cin >> p;
      if (p) addEdge(i, j);
    }
  tarjan(1);
  if (count(dfn, dfn + n, 0)) {
    cout << "NO"
         << "\n";
    return 0;
  }
  cout << (sen > 1 ? "NO" : "YES") << "\n";
  return 0;
}
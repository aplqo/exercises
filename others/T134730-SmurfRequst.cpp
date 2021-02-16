#ifdef APTEST
#include "debug_tools/program.h"
#endif
#include <algorithm>
#include <iostream>
using namespace std;
const int maxn = 1000;

struct edge {
  unsigned int to;
  edge* pre = nullptr;
} ed[maxn * maxn + 10];
edge* head[maxn + 10];

int blk[maxn + 10][2];
bool ans[maxn + 10][maxn + 10];
bool mat[maxn + 10][maxn + 10], col[maxn + 10];
unsigned int vis[maxn + 10];

inline void AddEdge(const unsigned int from, const unsigned int to)
{
  static edge* cur = ed;
  cur->to = to;
  cur->pre = head[from];
  head[from] = cur;
  ++cur;
}
inline void CreateGraph(const unsigned int n)
{
  for (unsigned int i = 1; i <= n; ++i)
    for (unsigned int j = 1; j <= n; ++j)
      if (!mat[i][j] && i != j) AddEdge(i, j);
}
bool dfs(const unsigned int x, const unsigned int sta)
{
  vis[x] = sta;
  for (edge* i = head[x]; i; i = i->pre) {
    if (vis[i->to] && col[i->to] == col[x])
      return false;
    else if (vis[i->to])
      continue;
    col[i->to] = !col[x];
    if (!dfs(i->to, sta)) return false;
  }
  return true;
}
inline int dp(const unsigned int n, const unsigned int s)
{
  for (unsigned int i = 1; i <= n; ++i)
    ++blk[vis[i]][col[i]];
  ans[0][0] = true;
  for (unsigned int o = 1; o <= s; ++o)
    for (int v = 1; v <= n; ++v)
      ans[o][v] = (v >= blk[o][1] && ans[o - 1][v - blk[o][1]]) ||
                  (v >= blk[o][0] && ans[o - 1][v - blk[o][0]]);
  int ret = 0;
  for (int i = 0; i <= n; ++i)
    if (ans[s][i]) ret = max(ret, min(i, static_cast<int>(n) - i));
  return ret;
}

int main()
{
#ifndef APTEST
  ios_base::sync_with_stdio(false);
#endif
  unsigned int n, m;
  cin >> n >> m;
  for (unsigned int i = 0; i < m; ++i) {
    unsigned int u, v;
    cin >> u >> v;
    mat[u][v] = true;
    mat[v][u] = true;
  }
  CreateGraph(n);
  unsigned int s = 0;
  for (unsigned int i = 1; i <= n; ++i)
    if (!vis[i] && !dfs(i, ++s)) {
      cout << "-1" << endl;
      return 0;
    }
  const unsigned int t = dp(n, s);
  const auto cliq = [](const unsigned int v) { return v * (v - 1) >> 1; };
  cout << (cliq(t) + cliq(n - t)) << endl;
  return 0;
}
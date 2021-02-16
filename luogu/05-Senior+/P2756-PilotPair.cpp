#ifdef APTEST
#include "debug_tools/program.h"
#endif
#include <algorithm>
#include <iostream>
using namespace std;
const int maxn = 100, maxe = maxn * maxn * 2, maxv = maxn * 2;

struct edge {
  unsigned int to;
  edge* pre = nullptr;
} ed[maxe * 2 + 10];
edge* head[maxv + 10];
unsigned int mat[maxv + 10];
bool vis[maxv + 10];

inline void AddEdge(const unsigned int from, const unsigned int to)
{
  static edge* cur = ed;
  cur->to = to;
  cur->pre = head[from];
  head[from] = cur;
  ++cur;
}
bool dfs(const unsigned int x)
{
  for (edge* i = head[x]; i; i = i->pre) {
    const unsigned int to = i->to;
    if (vis[to]) continue;
    vis[to] = true;
    if (!mat[to] || dfs(mat[to])) {
      mat[to] = x;
      return true;
    }
  }
  return false;
}

int main()
{
#ifndef APTEST
  ios_base::sync_with_stdio(false);
#endif
  unsigned int n, m;
  cin >> m >> n;
  while (true) {
    int u, v;
    cin >> u >> v;
    if (u == -1 && v == -1) break;
    AddEdge(u, v + m);
    AddEdge(v + m, u);
  }
  unsigned int ans = 0;
  for (unsigned int i = 1; i <= m; ++i) {
    if (dfs(i)) ++ans;
    fill(vis, vis + n + m + 1, false);
  }
  cout << ans << endl;
  for (unsigned int i = m + 1; i <= n + m; ++i)
    if (mat[i]) cout << mat[i] << " " << i - m << endl;
  return 0;
}
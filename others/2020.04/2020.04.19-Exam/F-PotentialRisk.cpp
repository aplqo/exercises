/*Luogu team T129715 potential risk*/
#ifdef APTEST
#include "debug_tools/program.h"
#endif
#include <algorithm>
#include <iostream>
using namespace std;
using num = long long;
const int maxn = 300000, maxl = 32;

struct edge {
  unsigned int to;
  num val;
  edge* pre = nullptr;
} ed[maxn * 2 + 10];
edge* head[maxn + 10];
unsigned int lg[maxn + 10];
num fa[maxn + 10][maxl + 10], dep[maxn + 10], dis[maxn + 10];

inline void AddEdge(const unsigned int from, const unsigned int to,
                    const num val)
{
  static edge* cur = ed;
  cur->to = to;
  cur->val = val;
  cur->pre = head[from];
  head[from] = cur;
  ++cur;
}
void GetLog(const unsigned int n)
{
  for (unsigned int i = 2; i <= n; ++i)
    lg[i] = lg[i >> 1] + 1;
}
void init(const unsigned int x, const unsigned int f)
{
  fa[x][0] = f;
  dep[x] = dep[f] + 1;
  for (unsigned int i = 1; i <= lg[dep[x]]; ++i)
    fa[x][i] = fa[fa[x][i - 1]][i - 1];
  for (edge* i = head[x]; i; i = i->pre) {
    if (dep[i->to]) continue;
    init(i->to, x);
    dis[i->to] = dis[x] + i->val;
  }
}
unsigned int lca(unsigned int x, unsigned int y)
{
  if (dep[x] != dep[y]) {
    if (dep[x] > dep[y]) swap(x, y);
    unsigned int d = dep[y] - dep[x], t = 1;
    for (unsigned int i = 0; t; ++i, t <<= 1)
      if (d & t) {
        y = fa[y][i];
        d ^= t;
      }
  }
  if (x == y) return x;
  for (unsigned int i = lg[dep[x]] + 1; i;) {
    unsigned int m = i - 1;
    if (fa[x][m] == fa[y][m])
      --i;
    else {
      x = fa[x][m];
      y = fa[y][m];
    }
  }
  return fa[x][0];
}
int main()
{
#ifndef APTEST
  ios_base::sync_with_stdio(false);
#endif
  unsigned int n;
  num k;
  cin >> n >> k;
  GetLog(n);
  for (unsigned int i = 1; i < n; ++i) {
    unsigned int u, v;
    num t;
    cin >> u >> v >> t;
    AddEdge(u, v, t);
    AddEdge(v, u, t);
  }
  init(1, 0);
  unsigned long long ans = 0;
  for (unsigned int i = 1; i <= n; ++i)
    for (unsigned int j = i; j <= n; ++j) {
      unsigned int l = lca(i, j);
      if (dis[i] + dis[j] - 2 * dis[l] <= k) ++ans;
    }
  cout << ans << endl;
  return 0;
}
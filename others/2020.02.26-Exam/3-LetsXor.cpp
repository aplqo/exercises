#ifdef APTEST
#include "debug_tools/program.h"
#endif
#include <algorithm>
#include <iostream>
using namespace std;
using num_t = unsigned int;
const int maxn = 100000, maxl = 20;

struct edge {
  unsigned int to;
  num_t val;
  edge* pre = nullptr;
} ed[maxn * 2 + 10];
edge* head[maxn + 10];
bool vis[maxn + 10];
unsigned int fa[maxn + 10][maxl + 10], dep[maxn + 10], lg[maxn + 10];
num_t sum[maxn + 10][maxl + 10];

inline void AddEdge(const unsigned int from, const unsigned int to,
                    const num_t val)
{
  static edge* cur = ed;
  cur->to = to;
  cur->val = val;
  cur->pre = head[from];
  head[from] = cur;
  ++cur;
}
inline void GetLog(const unsigned int n)
{
  for (unsigned int i = 2; i <= n; ++i)
    lg[i] = lg[i >> 1] + 1;
}
void init(const unsigned int x, const unsigned int f)
{
  vis[x] = true;
  fa[x][0] = f;
  dep[x] = dep[f] + 1;
  for (unsigned int i = 1; i <= lg[dep[x]]; ++i) {
    fa[x][i] = fa[fa[x][i - 1]][i - 1];
    sum[x][i] = sum[fa[x][i - 1]][i - 1] ^ sum[x][i - 1];
  }
  for (edge* i = head[x]; i; i = i->pre)
    if (!vis[i->to]) {
      sum[i->to][0] = i->val;
      init(i->to, x);
    }
}
num_t lca(unsigned int a, unsigned int b)
{
  num_t ret = 0;
  if (dep[a] != dep[b]) {
    if (dep[a] > dep[b]) swap(a, b);
    unsigned int d = dep[b] - dep[a], t = 1;
    for (unsigned int i = 0; d; ++i, t <<= 1)
      if (d & t) {
        ret ^= sum[b][i];
        b = fa[b][i];
        d ^= t;
      }
  }
  if (a == b) return ret;
  for (unsigned int i = lg[dep[a]]; i;) {
    unsigned int m = i - 1;
    if (fa[a][m] == fa[b][m])
      --i;
    else {
      ret ^= sum[a][m] ^ sum[b][m];
      a = fa[a][m];
      b = fa[b][m];
    }
  }
  ret ^= sum[a][0] ^ sum[b][0];
  return ret;
}
int main()
{
#ifndef APTEST
  ios_base::sync_with_stdio(false);
#endif
  unsigned int n, m;
  cin >> n;
  GetLog(n);
  for (unsigned int i = 0; i < n - 1; ++i) {
    unsigned int a, b;
    num_t w;
    cin >> a >> b >> w;
    AddEdge(a, b, w);
    AddEdge(b, a, w);
  }
  init(1, 0);
  cin >> m;
  for (unsigned int i = 0; i < m; ++i) {
    unsigned int u, v;
    cin >> u >> v;
    cout << lca(u, v) << endl;
  }
  return 0;
}

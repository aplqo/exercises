#ifdef APTEST
#include "debug_tools/program.h"
#endif
#include <algorithm>
#include <iostream>
#include <utility>
using namespace std;
const unsigned int maxn = 1e5, maxm = 2e5, maxl = 17;

struct edge {
  unsigned int to;
  edge* pre = nullptr;
} ed[maxn * 2];
edge* head[maxn + 1];
int cnt[maxn + 1];
unsigned int lg[maxn + 1];
unsigned int dep[maxn + 1], fa[maxn + 1][maxl + 1];

void addEdge(const unsigned int from, const unsigned int to)
{
  static edge* cur = ed;
  cur->to = to;
  cur->pre = head[from];
  head[from] = cur;
  ++cur;
}
void getLog(const unsigned int n)
{
  for (unsigned int i = 2; i <= n; ++i)
    lg[i] = lg[i >> 1] + 1;
}
void init(const unsigned int x, const unsigned int f)
{
  fa[x][0] = f;
  dep[x] = dep[f] + 1;
  for (unsigned int i = 1; i <= lg[dep[x]] + 1; ++i)
    fa[x][i] = fa[fa[x][i - 1]][i - 1];
  for (edge* i = head[x]; i; i = i->pre)
    if (i->to != f) init(i->to, x);
}
unsigned int lca(unsigned int a, unsigned int b)
{
  if (dep[a] != dep[b]) {
    if (dep[a] > dep[b]) swap(a, b);
    unsigned int d = dep[b] - dep[a], t = 1;
    for (unsigned int i = 0; t; ++i, t <<= 1)
      if (d & t) {
        b = fa[b][i];
        d ^= t;
      }
  }
  if (a == b) return a;
  for (unsigned int i = lg[dep[a]] + 1; i;) {
    const unsigned int m = i - 1;
    if (fa[a][m] == fa[b][m])
      --i;
    else {
      a = fa[a][m];
      b = fa[b][m];
    }
  }
  return fa[a][0];
}
int dfs(const unsigned int x, const unsigned int f)
{
  for (edge* i = head[x]; i; i = i->pre)
    if (i->to != f) cnt[x] += dfs(i->to, x);
  return cnt[x];
}
inline unsigned int count(const unsigned int n, const unsigned int m)
{
  return count(cnt + 2, cnt + 1 + n, 0) * m + count(cnt + 2, cnt + 1 + n, 1);
}

int main()
{
#ifndef APTEST
  ios_base::sync_with_stdio(false);
#endif
  unsigned int n, m;
  cin >> n >> m;
  for (unsigned int i = 1; i < n; ++i) {
    unsigned int x, y;
    cin >> x >> y;
    addEdge(x, y);
    addEdge(y, x);
  }
  getLog(n);
  init(1, 0);
  for (unsigned int i = 0; i < m; ++i) {
    unsigned int x, y;
    cin >> x >> y;
    ++cnt[x];
    ++cnt[y];
    cnt[lca(x, y)] -= 2;
  }
  dfs(1, 0);
  cout << count(n, m) << endl;
  return 0;
}
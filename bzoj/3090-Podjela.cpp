#ifdef APTEST
#include "debug_tools/program.h"
#endif
#include <algorithm>
#include <climits>
#include <iostream>
using namespace std;
const unsigned int maxn = 2000, maxs = maxn;
constexpr int ninf = INT_MIN / 2;

struct Edge {
  unsigned int to;
  const Edge* pre;
} ed[maxn * 2 + 10];
const Edge* head[maxn + 1];
unsigned int n;
int f[maxn + 1][maxs * 3 + 10];

void addEdge(const unsigned int from, const unsigned int to)
{
  static Edge* cur = ed;
  cur->to = to;
  cur->pre = head[from];
  head[from] = cur++;
}
void dfs(const unsigned int x, const unsigned int fa)
{
  static unsigned int siz[maxn + 1];
  siz[x] = 1;
  for (const Edge* i = head[x]; i; i = i->pre)
    if (i->to != fa) {
      dfs(i->to, x);
      siz[x] += siz[i->to];
    }
  for (const Edge* i = head[x]; i; i = i->pre) {
    if (i->to == fa) continue;
    for (unsigned int j = siz[x] - 1; j > 0; --j) {
      int cv = ninf;
      for (unsigned int k = 0; k <= min(siz[i->to] - 1, j); ++k) {
        if (j - 1 >= k) cv = max(cv, f[x][j - k - 1] + f[i->to][k]);
        if (f[i->to][k] >= 0) cv = max(cv, f[x][j - k]);
      }
      f[x][j] = cv;
    }
    f[x][0] = f[i->to][0] >= 0 ? f[x][0] : ninf;
  }
}

int main()
{
#ifndef APTEST
  ios_base::sync_with_stdio(false);
#endif
  unsigned int n;
  int x;
  cin >> n >> x;
  for (unsigned int i = 1; i <= n; ++i) {
    unsigned int v;
    cin >> v;
    f[i][0] = x - v;
  }
  for (unsigned int i = 1; i < n; ++i) {
    unsigned int u, v;
    cin >> u >> v;
    addEdge(u, v);
    addEdge(v, u);
  }
  for (unsigned int i = 1; i <= n; ++i)
    fill(f[i] + 1, f[i] + 1 + n, ninf);
  dfs(1, 0);
  for (unsigned int i = 0; i <= n; ++i)
    if (f[1][i] >= 0) {
      cout << i << '\n';
      break;
    }
  return 0;
}
#ifdef APTEST
#include "debug_tools/program.h"
#endif
#include <algorithm>
#include <climits>
#include <iostream>
#include <iterator>
using namespace std;
const unsigned int maxn = 500000, maxd = 20;
constexpr unsigned int inf = UINT_MAX / 2;

struct Edge {
  unsigned int to;
  const Edge* pre;
} ed[maxn * 2 + 1];
const Edge* head[maxn + 1];
unsigned int f[maxn + 1][maxd + 10], g[maxn + 1][maxd + 10];
bool key[maxn + 1];
unsigned int d, w[maxn + 1];

inline void addEdge(const unsigned int from, const unsigned int to)
{
  static Edge* cur = ed;
  cur->to = to;
  cur->pre = head[from];
  head[from] = cur++;
}
void dfs(const unsigned int x, const unsigned int fa)
{
  g[x][0] = key[x] ? w[x] : 0;
  fill(g[x] + 1, g[x] + 1 + d, w[x]);
  f[x][d + 1] = g[x][d + 1] = inf;
  bool leaf = true;
  for (const Edge* i = head[x]; i; i = i->pre) {
    if (i->to == fa) continue;
    dfs(i->to, x);
    leaf = false;
    for (unsigned int j = 0; j <= d; ++j)
      g[x][j] = min(g[x][j] + f[i->to][j], f[x][j + 1] + g[i->to][j + 1]);
    for (int j = d - 1; j >= 0; --j)
      g[x][j] = min(g[x][j], g[x][j + 1]);
    for (unsigned int j = 1; j <= d; ++j)
      f[x][j] += f[i->to][j - 1];
    f[x][0] = g[x][0];
    for (unsigned int j = 1; j <= d; ++j)
      f[x][j] = min(f[x][j], f[x][j - 1]);
  }
  if (leaf) f[x][0] = g[x][0];
}

int main()
{
#ifndef APTEST
  ios_base::sync_with_stdio(false);
#endif
  unsigned int n, m;
  cin >> n >> d;
  copy_n(istream_iterator<unsigned int>(cin), n, w + 1);
  cin >> m;
  for (unsigned int i = 0; i < m; ++i) {
    unsigned int t;
    cin >> t;
    key[t] = true;
  }
  for (unsigned int i = 1; i < n; ++i) {
    unsigned int x, y;
    cin >> x >> y;
    addEdge(x, y);
    addEdge(y, x);
  }
  dfs(1, 0);
  cout << g[1][0] << endl;
  return 0;
}
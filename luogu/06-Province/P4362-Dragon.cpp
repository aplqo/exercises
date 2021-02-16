#ifdef APTEST
#include "debug_tools/program.h"
#endif
#include <algorithm>
#include <climits>
#include <iostream>
using namespace std;
const unsigned int maxn = 300;
constexpr unsigned long long inf = ULLONG_MAX / 2;

struct Edge {
  unsigned int to, val;
  const Edge* pre;
} ed[maxn * 2 + 10];
const Edge* head[maxn + 1];
unsigned long long f[maxn + 1][maxn + 1][2];
unsigned int siz[maxn + 1];

inline void addEdge(const unsigned int from, const unsigned int to,
                    const unsigned int val)
{
  static Edge* cur = ed;
  new (cur) Edge{to, val, head[from]};
  head[from] = cur++;
  new (cur) Edge{from, val, head[to]};
  head[to] = cur++;
}
static void init(const unsigned int n)
{
  for (unsigned int i = 1; i <= n; ++i)
    for (unsigned int j = 0; j <= n; ++j)
      f[i][j][0] = f[i][j][1] = inf;
}
template <bool only>
void dfs(const unsigned int x, const unsigned int father)
{
  siz[x] = 1;
  f[x][1][1] = 0;
  f[x][0][0] = 0;
  bool leaf = true;
  for (const Edge* i = head[x]; i; i = i->pre) {
    const unsigned int to = i->to;
    if (to == father) continue;
    dfs<only>(to, x);
    leaf = false;
    siz[x] += siz[i->to];

    for (int j = siz[x]; j >= 0; --j) {
      const int mk = min<int>(siz[to], j);
      unsigned long long mv[2] = {inf, inf};
      for (int k = 0; k <= mk; ++k) {
        if constexpr (only)
          mv[0] = min(mv[0],
                      f[x][j - k][0] + min(f[to][k][0] + i->val, f[to][k][1]));
        else
          mv[0] = min(mv[0], f[x][j - k][0] + min(f[to][k][0], f[to][k][1]));
        mv[1] =
            min(mv[1], f[x][j - k][1] + min(f[to][k][0], f[to][k][1] + i->val));
      }
      f[x][j][0] = mv[0];
      f[x][j][1] = mv[1];
    }
  }
  if (leaf) f[x][1][1] = f[x][0][0] = 0;
}

int main()
{
#ifndef APTEST
  ios_base::sync_with_stdio(false);
#endif
  unsigned int n, m, k;
  cin >> n >> m >> k;
  if (n < k + m - 1) {
    cout << "-1\n";
    return 0;
  }
  for (unsigned int i = 1; i < n; ++i) {
    unsigned int u, v, w;
    cin >> u >> v >> w;
    addEdge(u, v, w);
  }
  init(n);
  if (m == 2)
    dfs<true>(1, 0);
  else
    dfs<false>(1, 0);
  cout << f[1][k][1] << "\n";
  return 0;
}
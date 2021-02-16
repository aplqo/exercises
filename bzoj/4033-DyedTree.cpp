#ifdef APTEST
#include "debug_tools/program.h"
#endif
#include <algorithm>
#include <climits>
#include <iostream>
using namespace std;
const unsigned int maxn = 2000;
constexpr long long ninf = LLONG_MIN / 2;

struct Edge {
  unsigned int to;
  int val;
  const Edge* pre;
} ed[maxn * 2 + 10];
const Edge* head[maxn + 1];
long long f[maxn + 1][maxn + 1];
unsigned int siz[maxn + 1], k, n;

inline void addEdge(const unsigned int from, const unsigned int to,
                    const int val)
{
  static Edge* cur = ed;
  cur->to = to;
  cur->val = val;
  cur->pre = head[from];
  head[from] = cur++;
}
void dfs(const unsigned int x, const unsigned int fa, const long long dis)
{
  static long long g[maxn + 1][maxn + 1];
  fill(g[x] + 1, g[x] + k + 1, ninf);
  fill(f[x], f[x] + k + 1, ninf);
  for (const Edge* i = head[x]; i; i = i->pre) {
    if (i->to == fa) continue;
    dfs(i->to, x, i->val);
    for (unsigned int j = min(k, siz[x] + siz[i->to]); j > 0; --j) {
      const unsigned int mt = min(siz[x], j);
      long long cv = ninf;
      for (unsigned int t = 0; t <= mt; ++t)
        cv = max(cv, g[x][t] + f[i->to][j - t]);
      g[x][j] = cv;
    }
    siz[x] += siz[i->to];
    g[x][0] += f[i->to][0];
  }
  ++siz[x];
  for (long long i = 0; i <= min(k, siz[x]); ++i) {
    f[x][i] = max(g[x][i], i > 0 ? g[x][i - 1] : 0) +
              (i * (k - i) + (siz[x] - i) * (n + i - siz[x] - k)) * dis;
  }
}

int main()
{
#ifndef APTEST
  ios_base::sync_with_stdio(false);
#endif
  cin >> n >> k;
  for (unsigned int i = 1; i < n; ++i) {
    unsigned int f, t, dis;
    cin >> f >> t >> dis;
    addEdge(f, t, dis);
    addEdge(t, f, dis);
  }
  dfs(1, 0, 0);
  cout << f[1][k] << endl;
  return 0;
}

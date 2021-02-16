#ifdef APTEST
#include "debug_tools/program.h"
#endif
#include <algorithm>
#include <climits>
#include <iostream>
using namespace std;
const unsigned int maxn = 51, maxm = 2000, maxc = 100;

struct Edge {
  unsigned int to, val;
  const Edge* pre;
} ed[maxn + 1];
const Edge* head[maxn + 1];
bool typ[maxn + 1];
unsigned int degree[maxn + 1];
unsigned int c[maxn + 1], lim[maxn + 1], s[maxn + 1];
unsigned int f[maxn + 1][maxm + 1][maxc + 1], m;

void addEdge(const unsigned int from, const unsigned int to,
             const unsigned int val)
{
  static Edge* cur = ed;
  cur->to = to;
  cur->val = val;
  cur->pre = head[from];
  head[from] = cur++;
  ++degree[to];
}
void dfs(const unsigned int x)
{
  static unsigned int g[maxn + 1][maxm + 1][maxc + 1];
  if (typ[x]) lim[x] = UINT_MAX;
  for (const Edge* i = head[x]; i; i = i->pre) {
    dfs(i->to);
    lim[x] = min(lim[x], lim[i->to] / i->val);
    c[x] += c[i->to] * i->val;
  }
  for (const Edge* i = head[x]; i; i = i->pre)
    for (int j = m; j >= 0; --j)
      for (unsigned int c = 0; c <= lim[x]; ++c) {
        unsigned int cv = 0;
        for (int k = 0; k <= j; ++k)
          cv = max(cv, g[x][j - k][c] + f[i->to][k][c * i->val]);
        g[x][j][c] = cv;
      }
  for (unsigned int i = 0; i <= m; ++i)
    for (unsigned int j = 0; j <= lim[x]; ++j)
      for (unsigned int k = 0; k <= lim[x] - j && k * c[x] <= i; ++k)
        f[x][i][j] = max(f[x][i][j], g[x][i - k * c[x]][j + k] + k * s[x]);
}

int main()
{
#ifndef APTEST
  ios_base::sync_with_stdio(false);
#endif
  unsigned int n;
  cin >> n >> m;
  for (unsigned int i = 1; i <= n; ++i) {
    char tc;
    cin >> s[i] >> tc;
    typ[i] = tc == 'A';
    if (tc == 'B')
      cin >> c[i] >> lim[i];
    else {
      unsigned int c;
      cin >> c;
      for (unsigned int j = 0; j < c; ++j) {
        unsigned int v, d;
        cin >> v >> d;
        addEdge(i, v, d);
      }
    }
  }
  const unsigned int root = find(degree + 1, degree + 1 + n, 0) - degree;
  dfs(root);
  const unsigned int mc = *max_element(lim, lim + 1 + n);
  unsigned int ans = 0;
  for (unsigned int i = 1; i <= n; ++i)
    for (unsigned int j = 0; j <= m; ++j)
      for (unsigned int t = 0; t <= mc; ++t)
        ans = max(ans, f[i][j][t]);
  cout << ans << endl;
  return 0;
}
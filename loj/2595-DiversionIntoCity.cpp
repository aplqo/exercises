#ifdef APTEST
#include "debug_tools/program.h"
#endif
#include <algorithm>
#include <climits>
#include <iostream>
#include <iterator>
using namespace std;
const unsigned int maxn = 500, maxm = 500;
const int dx[] = {0, 0, -1, 1}, dy[] = {1, -1, 0, 0};
constexpr unsigned int inf = UINT_MAX / 2;

struct Range {
  Range() = default;
  Range(const unsigned int l, const unsigned int r) : l(l), r(r){};
  inline Range operator+(const Range b) const
  {
    return Range(min(l, b.l), max(r, b.r));
  }
  unsigned int l, r;
};
unsigned int n, m;
unsigned int h[maxn + 1][maxm + 1];
bool vis[maxn + 1][maxm + 1];
Range rng[maxn + 1][maxm + 1];

Range dfs(const int x, const int y)
{
  if (vis[x][y]) return rng[x][y];
  rng[x][y] = x == n ? Range(y, y) : Range(inf, 0);
  vis[x][y] = true;
  for (unsigned int i = 0; i < 4; ++i) {
    const int nx = x + dx[i], ny = y + dy[i];
    if (nx > 0 && nx <= n && ny > 0 && ny <= m && h[nx][ny] < h[x][y])
      rng[x][y] = rng[x][y] + dfs(nx, ny);
  }
  return rng[x][y];
}
unsigned int minCover(const unsigned int m)
{
  static unsigned int left[maxm + 1], cnt[maxm + 1];
  fill(left + 1, left + 1 + m, inf);
  fill(cnt + 1, cnt + 1 + m, inf);
  for (const Range* i = rng[1] + 1; i < rng[1] + m + 1; ++i)
    left[i->r] = min(left[i->r], i->l);
  for (unsigned int i = 1; i <= m; ++i)
    for (unsigned int j = left[i] - 1; j < i; ++j)
      cnt[i] = min(cnt[i], cnt[j] + 1);
  return cnt[m];
}
int main()
{
#ifndef APTEST
  ios_base::sync_with_stdio(false);
#endif
  cin >> n >> m;
  for (unsigned int i = 1; i <= n; ++i)
    copy_n(istream_iterator<unsigned int>(cin), m, h[i] + 1);
  for (unsigned int i = 1; i <= m; ++i)
    dfs(1, i);
  const unsigned int v = count(vis[n] + 1, vis[n] + 1 + m, false);
  if (v)
    cout << "0\n" << v << "\n";
  else
    cout << "1\n" << minCover(m) << "\n";
  return 0;
}
#ifdef APTEST
#include "debug_tools/program.h"
#endif
#include <algorithm>
#include <climits>
#include <iostream>
#include <iterator>
using namespace std;
const unsigned int maxn = 1500;
constexpr int dx[] = {1, 1, -1, -1}, dy[] = {-1, 1, -1, 1};

enum { DL = 0, DR = 1, UL = 2, UR = 3 };
unsigned int sum[maxn + 10][maxn + 10];
unsigned int squ[4][maxn + 10][maxn + 10];
int n, m;

static void initSum(const unsigned int n, const unsigned int m)
{
  for (unsigned int i = 1; i <= n; ++i)
    for (unsigned int j = 1; j <= m; ++j)
      sum[i][j] += sum[i - 1][j] + sum[i][j - 1] - sum[i - 1][j - 1];
}
inline unsigned int square(int x1, int y1, int x2, int y2)
{
  if (x1 > x2) swap(x1, x2);
  if (y1 > y2) swap(y1, y2);
  if (x1 < 1 || y1 < 1 || x2 > n || y2 > m)
    return 0;
  else
    return sum[x2][y2] + sum[x1 - 1][y1 - 1] - sum[x1 - 1][y2] -
           sum[x2][y1 - 1];
}
template <int typ>
void dp(const int sx, const int sy, const int ex, const int ey, const int k)
{
  unsigned int(*const res)[maxn + 10] = squ[typ];
  constexpr int sdx = dx[typ], sdy = dy[typ];
  for (int i = sx; i != ex + sdx; i += sdx)
    for (int j = sy; j != ey + sdy; j += sdy)
      res[i][j] = max({res[i - sdx][j], res[i][j - sdy],
                       square(i - (k - 1) * sdx, j - (k - 1) * sdy, i, j)});
}
unsigned int getMax(const int n, const int m, const int k)
{
  dp<DL>(1, m, n, 1, k);
  dp<DR>(1, 1, n, m, k);
  dp<UL>(n, m, 1, 1, k);
  dp<UR>(n, 1, 1, m, k);
  unsigned int ret = 0;
  for (int i = 1; i <= n; ++i)
    for (int j = 1; j <= m; ++j) {
      ret = max({ret,
                 squ[DR][i][j] + max(squ[DL][n][j + 1] + squ[UR][i + 1][j],
                                     squ[UL][i + 1][1] + squ[DL][i][j + 1]),
                 squ[DL][i][j] + max(squ[UL][i + 1][1] + squ[DR][i][j - 1],
                                     squ[UL][i + 1][j] + squ[UR][1][j - 1]),
                 squ[UR][i][j] + max(squ[DR][i - 1][m] + squ[UL][i][j + 1],
                                     squ[DL][n][j + 1] + squ[DR][i - 1][j]),
                 squ[UL][i][j] + max(squ[DR][i - 1][m] + squ[UR][i][j - 1],
                                     squ[DR][n][j - 1] + squ[DL][i - 1][j])});

      if (j - k >= k && j + k - 1 <= m - k + 1 && i - k >= 0)
        ret = max(ret, squ[DR][n][j - k] + squ[UL][1][j + 1] +
                           square(i + 1 - k, j + 1 - k, i, j));
      if (i - k >= k && i + k - 1 <= n - k + 1 && j - k >= 0)
        ret = max(ret, squ[DR][i - k][m] + squ[UL][i + 1][1] +
                           square(i + 1 - k, j + 1 - k, i, j));
    }
  return ret;
}

int main()
{
#ifndef APTEST
  ios_base::sync_with_stdio(false);
#endif
  unsigned int k;
  cin >> n >> m >> k;
  for (int i = 1; i <= n; ++i)
    copy_n(istream_iterator<unsigned int>(cin), m, sum[i] + 1);
  initSum(n, m);
  cout << getMax(n, m, k) << "\n";
  return 0;
}
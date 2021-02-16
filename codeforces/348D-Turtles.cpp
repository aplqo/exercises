#include <cstdio>
#include <cstring>
#include <utility>
using namespace std;
typedef pair<unsigned int, unsigned int> point;
const unsigned int maxn = 3000;
const unsigned long long mod = 1000000007;

bool fail[maxn + 3][maxn + 3];
unsigned long long ans[maxn + 3][maxn + 3];
unsigned int n, m;

unsigned long long dp(unsigned long long f[maxn + 3][maxn + 3], const point beg,
                      const point en)
{
  f[beg.first][beg.second] = 1;
  for (unsigned int i = 1; i <= n; ++i)
    for (unsigned int j = 1; j <= m; ++j) {
      if (fail[i][j]) {
        f[i][j] = 0;
        continue;
      }
      f[i + 1][j] = (f[i + 1][j] + f[i][j]) % mod;
      f[i][j + 1] = (f[i][j + 1] + f[i][j]) % mod;
    }
  return f[en.first][en.second];
}
unsigned long long dp(const unsigned int bx, const unsigned int by,
                      const unsigned int ex, const unsigned int ey)
{
  memset(ans, 0, sizeof(ans));
  return dp(ans, point(bx, by), point(ex, ey));
}
unsigned long long solve(const unsigned int bx1, const unsigned int by1,
                         const unsigned int bx2, const unsigned int by2)
{
  const unsigned long long va =
      (dp(bx1, by1, n - 1, m) * dp(bx2, by2, n, m - 1)) % mod;
  const unsigned long long ta =
      (dp(bx1, by1, n, m - 1) * dp(bx2, by2, n - 1, m)) % mod;
  return (va + mod - ta) % mod;
}
int main()
{
  scanf("%u%u", &n, &m);
  for (unsigned int i = 1; i <= n; ++i) {
    static char str[maxn + 1];
    scanf("%s", str);
    for (unsigned int j = 0; j < m; ++j)
      fail[i][j + 1] = str[j] == '#';
  }
  if (fail[2][1] || fail[1][2] || fail[n - 1][m] || fail[n][m - 1] ||
      fail[n][m] || fail[1][1]) {
    printf("%u\n", 0);
    return 0;
  }
  if ((n <= 2 && m == 1) || (n == 1 && m <= 2)) {
    printf("%u\n", 1);
    return 0;
  }
  printf("%lld\n", solve(1, 2, 2, 1));
  return 0;
}

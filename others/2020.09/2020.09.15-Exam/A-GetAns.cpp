#include <algorithm>
#include <climits>
#include <fstream>
using namespace std;
const unsigned int maxn = 500, maxm = 500, maxk = maxn;
constexpr unsigned long long inf = ULLONG_MAX / 2;

unsigned int p[maxn + 1], q[maxn + 1];
unsigned long long f[maxn + 10][maxm + 10][maxk + 1];

void dp(const unsigned int n)
{
  for (unsigned int i = 0; i <= n; ++i)
    for (unsigned int j = 1; j <= n + 1; ++j)
      fill(f[i][j], f[i][j] + n + 1, inf);
  for (unsigned int i = 1; i <= n; ++i) {
    for (unsigned int j = 1; j <= i; ++j) {
      f[i][j][0] = min(f[i - 1][j][0], f[i - 1][j - 1][0] + p[i]);
      for (unsigned int k = 1; k <= n; ++k)
        f[i][j][k] = min({f[i - 1][j][k], f[i - 1][j - 1][k] + p[i],
                          f[i - 1][j - 1][k - 1] + q[i]});
    }
    for (int j = i; j >= 0; --j)
      for (unsigned int k = 0; k <= n; ++k)
        f[i][j][k] = min(f[i][j][k], f[i][j + 1][k]);
  }
}
unsigned int maxBuy(const unsigned int n, const unsigned long long m,
                    const unsigned int k)
{
  unsigned int ret = 0;
  for (unsigned int i = 0; i <= n; ++i)
    if (f[n][i][k] <= m) ret = i;
  return ret;
}

int main(int, char* argv[])
{
  ifstream in(argv[1]);
  ofstream ans(argv[2]);
  unsigned int n, k;
  unsigned long long m;
  in >> n >> k >> m;
  for (unsigned int i = 1; i <= n; ++i)
    in >> p[i] >> q[i];
  dp(n);
  ans << maxBuy(n, m, k) << "\n";
  return 0;
}
#include <algorithm>
#include <cmath>
#include <iostream>
#include <sstream>
using namespace std;
const unsigned int maxn = 1000, maxm = 1000;
constexpr unsigned long long mod = 1e9 + 7;

unsigned long long f[maxn * 2 + 10][maxm + 10][2];

static void dp(const unsigned int n, const unsigned int m)
{
  f[1][0][0] = f[1][1][1] = 1;
  for (unsigned int i = 2; i <= n + m; ++i) {
    f[i][0][0] = f[i - 1][0][1];
    f[i][0][1] = f[i - 1][0][0];
    for (unsigned int j = 1; j <= i; ++j) {
      f[i][j][0] =
          (f[i - 1][j - 1][0] + f[i - 1][j - 1][1] + f[i - 1][j][1]) % mod;
      f[i][j][1] = f[i - 1][j][0];
    }
  }
}
static void print(const unsigned int n, const unsigned int m)
{
  unsigned int lg = 0;
  for (unsigned int i = 1; i <= n; ++i)
    for (unsigned int j = 1; j <= m; ++j)
      lg = max(log10(f[i + j][j][0]) + 1 + log10(f[i + j][j][1]) + 1 + 3,
               1.0 * lg);
  for (unsigned int i = 1; i <= n; ++i) {
    for (unsigned int j = 1; j <= m; ++j) {
      ostringstream os;
      os << "(" << f[i + j][j][0] << "," << f[i + j][j][1] << ")";
      cout.width(lg);
      cout << os.str() << " ";
    }
    cout << "\n";
  }
}
int main()
{
  unsigned int n, m;
  cin >> n >> m;
  dp(n, m);
  print(n, m);
  return 0;
}

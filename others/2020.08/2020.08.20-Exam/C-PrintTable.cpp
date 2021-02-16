#include <algorithm>
#include <cstring>
#include <iostream>
#include <iterator>
using namespace std;
const unsigned int maxn = 64;
constexpr unsigned long long mod = 1e9 + 7;

unsigned long long f[maxn + 1][maxn + 1][maxn + 1];

void dp(const unsigned int x)
{
  for (unsigned int i = 0; i <= x; ++i)
    for (unsigned int j = 0; j <= i; ++j)
      fill(f[i][j] + i, f[i][j] + x + 1, 1);
  for (unsigned int i = 1; i <= x; ++i)
    for (unsigned int l = 0; l <= x; ++l)
      for (unsigned int r = l; r <= i; ++r)
        for (unsigned int k = l; k <= r; ++k)
          f[i][l][r] =
              (f[i][l][r] + (f[i - 1][l][k] * f[i - 1][k][r]) % mod) % mod;
}
int main()
{
  dp(64);
  while (true) {
    unsigned int x, l, r;
    cout << "T3 print>" << flush;
    cin >> x >> l >> r;
    if (cin.fail()) break;
    cout << f[x][l][r] << endl;
  }
  return 0;
}

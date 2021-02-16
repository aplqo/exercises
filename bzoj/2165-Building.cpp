#ifdef APTEST
#include "debug_tools/program.h"
#endif
#include <algorithm>
#include <climits>
#include <iostream>
#include <limits>
using namespace std;
const unsigned int maxn = 100, maxl = 64;
typedef __int128_t Floor;
constexpr Floor ninf =
    -(__int128_t(1) << 126) /
    __int128_t(maxl);  // numeric_limits<Floor>::lowest() / maxl;

unsigned int n;
struct Matrix {
  void reset()
  {
    for (unsigned int i = 0; i <= n; ++i)
      fill(val[i], val[i] + n + 1, ninf);
  }
  Matrix operator*(const Matrix& a) const
  {
    Matrix ret;
    ret.reset();
    for (unsigned int i = 1; i <= n; ++i)
      for (unsigned int j = 1; j <= n; ++j)
        for (unsigned int k = 1; k <= n; ++k)
          ret.val[i][k] = std::max(ret.val[i][k], a.val[i][j] + val[j][k]);
    return ret;
  }
  Floor max(const unsigned int i) const
  {
    return *max_element(val[i], val[i] + n + 1);
  }

  Floor val[maxn + 1][maxn + 1];
};
Matrix po[maxl + 10];

static unsigned int initPow(const unsigned long long m)
{
  for (unsigned int i = 1; i <= maxl; ++i) {
    po[i] = po[i - 1] * po[i - 1];
    if (po[i].max(1) >= m) return i;
  }
  return maxl;
}
unsigned long long binarySearch(const unsigned long long m,
                                const unsigned int ml)
{
  Matrix cm{};
  unsigned long long ret = 0;
  for (unsigned long long i = ml; i;) {
    const unsigned int h = i - 1;
    const Matrix t = cm * po[h];
    if (t.max(1) < m) {
      cm = t;
      ret += 1ull << h;
    }
    else
      --i;
  }
  return ret + 1;
}
void solve()
{
  unsigned long long m;
  cin >> n >> m;
  for (unsigned int i = 1; i <= n; ++i)
    for (unsigned int j = 1; j <= n; ++j) {
      unsigned long long t;
      cin >> t;
      po[0].val[i][j] = t ? t : ninf;
    }
  cout << binarySearch(m, initPow(m)) << "\n";
}

int main()
{
#ifndef APTEST
  ios_base::sync_with_stdio(false);
#endif
  unsigned int t;
  cin >> t;
  for (unsigned int i = 0; i < t; ++i)
    solve();
  return 0;
}
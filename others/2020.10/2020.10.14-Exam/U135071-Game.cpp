#ifdef APTEST
#include "debug_tools/program.h"
#endif
#include <algorithm>
#include <cmath>
#include <cstring>
#include <iomanip>
#include <iostream>
#include <iterator>
using namespace std;
const unsigned int maxn = 50, maxm = 20, maxs = 1 << maxm;
typedef long double real;

#define lowbit(x) ((x) & -(x))
unsigned int same[maxs + 10];
unsigned long long str[maxn + 10][maxm + 10], lg[maxs + 10];

static void initLog(const unsigned int m)
{
  for (unsigned int i = 0; i < m; ++i)
    lg[1 << i] = i;
}
static void findSame(const unsigned int n, const unsigned int m)
{
  static unsigned long long pos[maxs], sameStr[maxs];
  const unsigned long long msk = (1ull << m) - 1;
  for (unsigned int i = 0; i < n; ++i) {
    memset(pos, 0, sizeof(pos));
    for (unsigned int j = 0; j < n; ++j)
      for (unsigned int k = 0; k < m; ++k)
        if (str[i][k] == str[j][k]) pos[k] |= 1ull << j;
    ++same[0];
    sameStr[0] = (1ull << n) - 1;
    for (unsigned int j = 1; j <= msk; ++j) {
      const unsigned int lb = lowbit(j);
      sameStr[j] = sameStr[j ^ lb] & pos[lg[lb]];
      if (sameStr[j] != 1ull << i) ++same[j];
    }
  }
}
real dp(const unsigned int m)
{
  static real ex[maxs + 10];
  const unsigned int msk = (1 << m) - 1;
  for (int j = msk - 1; j >= 0; --j) {
    if (!same[j]) continue;
    unsigned int cnt = 0;
    for (unsigned int i = (~j) & msk; i; i -= lowbit(i)) {
      const unsigned int nv = j + lowbit(i);
      ex[j] += ex[nv] * same[nv];
      ++cnt;
    }
    ex[j] = ex[j] / (cnt * same[j]) + 1;
  }
  return ex[0];
}
int main()
{
#ifndef APTEST
  ios_base::sync_with_stdio(false);
#endif
  unsigned int n, m;
  cin >> n >> m;
  for (unsigned int i = 0; i < n; ++i)
    copy_n(istream_iterator<unsigned int>(cin), m, str[i]);
  initLog(m);
  findSame(n, m);
  cout << fixed << setprecision(5) << dp(m) << "\n";
  return 0;
}
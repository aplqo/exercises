#ifdef APTEST
#include "debug_tools/program.h"
#endif
#include <algorithm>
#include <climits>
#include <cstring>
#include <iostream>
#include <iterator>
using namespace std;
const unsigned int maxn = 500000, maxl = 60;
constexpr unsigned long long ullIinf = ULLONG_MAX;

unsigned long long a[maxn + 10];

unsigned long long dp(const unsigned int n, const unsigned int m)
{
  unsigned long long ret = 0;
  for (int i = maxl; i >= 0; --i) {
    const unsigned long long msk = 1ull << i, lmsk = ullIinf - (msk - 1);
    unsigned int cnt = 1, lst = 0;
    for (unsigned int j = 1; j < n + 1; ++j)
      if (!(a[j] & msk) && ((a[j] & lmsk) | ret) == ret) {
        ++cnt;
        lst = j;
      }
    {
      const unsigned long long ls = a[n] ^ a[lst];
      if (cnt >= m && !(ls & msk) && ((ls & lmsk) | ret) == ret)
        continue;
      else
        ret |= msk;
    }
  }
  return ret;
}

int main(int, char* argv[])
{
#ifndef APTEST
  ios_base::sync_with_stdio(false);
#endif
  unsigned int n, m;
  cin >> n >> m;
  for (unsigned int i = 1; i <= n; ++i) {
    unsigned long long v;
    cin >> v;
    a[i] = a[i - 1] ^ v;
  }
  a[n + 1] = a[n];
  cout << dp(n, m) << "\n";
  return 0;
}

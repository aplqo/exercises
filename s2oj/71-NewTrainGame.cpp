#ifdef APTEST
#include "debug_tools/program.h"
#endif
#include <algorithm>
#include <climits>
#include <iostream>
#include <iterator>
using namespace std;
const unsigned int maxn = 1e6, maxk = 1e6;
constexpr long long ninf = LLONG_MIN / 2;

unsigned int t[maxn + 10];
long long d[maxn + 10];

long long dp(const unsigned int n)
{
  static long long f[maxn + 10], typ[maxk + 10];
  fill(typ, typ + 1 + maxk, ninf);
  long long mxv = 0;
  for (unsigned int i = 1; i <= n; ++i) {
    f[i] = max(d[i] + typ[t[i]], mxv);
    typ[t[i]] = max(typ[t[i]], mxv - d[i - 1]);
    mxv = max(mxv, f[i]);
  }
  return mxv;
}
int main()
{
#ifndef APTEST
  ios_base::sync_with_stdio(false);
#endif
  unsigned int n, k;
  cin >> n >> k;
  copy_n(istream_iterator<unsigned int>(cin), n, t + 1);
  for (unsigned int i = 1; i <= n; ++i) {
    long long cd;
    cin >> cd;
    d[i] = d[i - 1] + cd;
  }
  cout << dp(n) << "\n";
  return 0;
}
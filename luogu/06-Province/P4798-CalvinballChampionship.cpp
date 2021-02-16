#ifdef APTEST
#include "debug_tools/program.h"
#endif
#include <algorithm>
#include <iostream>
#include <iterator>
#include <numeric>
using namespace std;
const unsigned int maxn = 10000;
const unsigned int mod = 1000007;

unsigned int y[maxn + 10];

unsigned long long count(const unsigned int n)
{
  static unsigned long long f[maxn + 10];
  static unsigned int mx[maxn + 10];
  fill(f, f + 1 + n, 1);
  partial_sum(y, y + n, mx,
              [](unsigned int a, unsigned int b) { return max(a, b); });
  unsigned long long ret = 1;
  for (int i = n - 2; i >= 0; --i) {
    ret = (ret + ((y[i + 1] - 1) * f[mx[i]]) % mod) % mod;
    for (unsigned int j = 0; j <= n; ++j)
      f[j] = ((f[j] * j) % mod + f[j + 1]) % mod;
  }
  return ret;
}

int main()
{
#ifndef APTEST
  ios_base::sync_with_stdio(false);
#endif
  unsigned int n;
  cin >> n;
  copy_n(istream_iterator<unsigned int>(cin), n, y);
  cout << count(n) << "\n";
  return 0;
}
#ifdef APTEST
#include "debug_tools/program.h"
#endif
#include <algorithm>
#include <iostream>
#include <iterator>
using namespace std;
const unsigned int maxn = 211985, maxa = 233333;
const unsigned long long mod = 1000000007;

unsigned int a[maxn + 1];
unsigned long long dp(const unsigned int n)
{
  static unsigned long long lst[maxa + 1], f[maxn + 1];
  lst[a[n - 1]] = 1;
  for (int i = n - 2; i >= 0; --i) {
    for (unsigned int j = a[i]; j; j = (j - 1) & a[i])
      f[i] = (f[i] + lst[j]) % mod;
    lst[a[i]] = (f[i] + 1) % mod;
  }
  unsigned long long ret = 0;
  for (unsigned int i = 0; i < n; ++i)
    ret = (ret + f[i]) % mod;
  return ret;
}
int main()
{
#ifndef APTEST
  ios_base::sync_with_stdio(false);
#endif
  unsigned int n;
  cin >> n;
  copy_n(istream_iterator<unsigned int>(cin), n, a);
  cout << dp(n) << "\n";
  return 0;
}
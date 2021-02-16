#ifdef APTEST
#include "debug_tools/program.h"
#endif
#include <iostream>
using namespace std;
const unsigned int maxn = 1e6;

unsigned int xorSum(const unsigned int n)
{
  static unsigned int xsum[maxn + 10];
  unsigned int ret = 0;
  for (unsigned int i = 1; i <= n; ++i)
    xsum[i] = xsum[i - 1] ^ i;
  for (unsigned int i = 1; i <= n; ++i) {
    if ((n / i) & 0x01) ret ^= xsum[i - 1];
    ret ^= xsum[n % i];
  }
  return ret;
}
int main()
{
#ifndef APTEST
  ios_base::sync_with_stdio(false);
#endif
  unsigned int n, acc = 0;
  cin >> n;
  for (unsigned int i = 0; i < n; ++i) {
    unsigned int v;
    cin >> v;
    acc ^= v;
  }
  cout << (acc ^ xorSum(n)) << "\n";
  return 0;
}

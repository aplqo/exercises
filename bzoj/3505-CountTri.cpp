#ifdef APTEST
#include "debug_tools/program.h"
#endif
#include <algorithm>
#include <iostream>
using namespace std;

unsigned long long gcd(unsigned long long a, unsigned long long b)
{
  if (a < b) swap(a, b);
  while (b) {
    const unsigned long long t = a;
    a = b;
    b = t % b;
  }
  return a;
}
unsigned long long count(const unsigned int n, const unsigned int m)
{
  unsigned long long ret = 0;
  for (unsigned int i = 0; i <= n; ++i)
    for (unsigned int j = 0; j <= m; ++j)
      if (i || j)
        ret += (!i || !j ? 1 : 2) * (n + 1 - i) * (m + 1 - j) * (gcd(i, j) - 1);
  return ret;
}
unsigned long long c(const unsigned long long v)
{
  return (v * (v - 1) / 2) * (v - 2) / 3;
}

int main()
{
#ifndef APTEST
  ios_base::sync_with_stdio(false);
#endif
  unsigned int n, m;
  cin >> n >> m;
  cout << c((n + 1) * (m + 1)) - count(n, m) << endl;
  return 0;
}
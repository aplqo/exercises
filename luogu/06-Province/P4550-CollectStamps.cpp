#ifdef APTEST
#include "debug_tools/program.h"
#endif
#include <iomanip>
#include <iostream>
using namespace std;
using real = double;
const unsigned int maxn = 10000;

static real dp(const unsigned int n)
{
  static real x[maxn + 1], x2[maxn + 1];
  for (unsigned int i = 1; i <= n; ++i) {
    x[i] = x[i - 1] + real(n) / i;
    x2[i] = x2[i - 1] + 2 * x[i - 1] + 2.0 * (n - i) / i * x[i] + real(n) / i;
  }
  return (x2[n] + x[n]) / 2;
}

int main()
{
#ifndef APTEST
  ios_base::sync_with_stdio(false);
#endif
  unsigned int n;
  cin >> n;
  cout << fixed << setprecision(2) << dp(n) << "\n";
  return 0;
}
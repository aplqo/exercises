#ifdef APTEST
#include "debug_tools/program.h"
#endif
#include <algorithm>
#include <functional>
#include <iostream>
using namespace std;
const unsigned int maxn = 100000;
typedef double real;

real a[maxn + 10], b[maxn + 10];

real solve(const unsigned int n)
{
  sort(a + 1, a + 1 + n, greater<real>());
  sort(b + 1, b + 1 + n, greater<real>());
  real suma = 0, sumb = 0, ret = 0;
  unsigned int cntb = 0;
  for (unsigned int i = 1; i <= n; ++i) {
    suma += a[i];
    while (cntb <= n && min(suma - cntb, sumb - i) <
                            min(suma - cntb - 1, sumb - i + b[cntb + 1]))
      sumb += b[++cntb];
    ret = max(ret, min(suma - cntb, sumb - i));
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
  for (unsigned int i = 1; i <= n; ++i) {
    cin >> a[i] >> b[i];
    --a[i];
    --b[i];
  }
  cout.precision(4);
  cout << fixed << solve(n) << "\n";
  return 0;
}
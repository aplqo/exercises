#ifdef APTEST
#include "debug_tools/program.h"
#endif
#include <algorithm>
#include <cmath>
#include <iostream>
using namespace std;
using num_t = unsigned long long;
const int maxp = 60000;

num_t ans = 1, nans = 1;
num_t primes[maxp + 10], *pcur = primes;
num_t mindiv[maxp + 10];
num_t n;

inline void eular(const num_t n)
{
  for (num_t i = 2; i <= n; ++i) {
    if (!mindiv[i]) {
      *(pcur++) = i;
      mindiv[i] = i;
    }
    for (num_t *p = primes; p < pcur && *p * i <= n; ++p)
      mindiv[*p * i] = *p;
  }
}
void dfs(const num_t x = 1, const num_t *p = primes, const unsigned int c = 32,
         const unsigned int ndiv = 1)
{
  num_t b = *p;
  if (ndiv > nans) {
    nans = ndiv;
    ans = x;
  }
  else if (ndiv == nans)
    ans = min(ans, x);
  if (p >= pcur) return;
  for (unsigned int i = 1; i <= c && x * b <= n; ++i, b *= *p)
    dfs(x * b, p + 1, i, ndiv * (i + 1));
}
int main()
{
#ifndef APTEST
  ios_base::sync_with_stdio(false);
#endif
  eular(maxp);
  while (cin) {
    cin >> n;
    if (cin.fail()) break;
    nans = 1;
    ans = 1;
    dfs();
    cout << ans << endl;
  }
  return 0;
}
#ifdef APTEST
#include "debug_tools/program.h"
#endif
#include <iostream>
using namespace std;

unsigned long long multiply(unsigned long long a, unsigned long long b,
                            const unsigned long long mod)
{
  unsigned long long ret = 0;
  for (; b; b >>= 1) {
    if (b & 0x01) ret = (ret + a) % mod;
    a = (a + a) % mod;
  }
  return ret;
}
unsigned long long quickPow(unsigned long long a, unsigned long long e,
                            const unsigned long long mod)
{
  unsigned long long ret = 1;
  for (; e; e >>= 1) {
    if (e & 0x01) ret = multiply(ret, a, mod);
    a = multiply(a, a, mod);
  }
  return ret;
}
void exgcd(const long long a, const long long b, long long& x, long long& y)
{
  if (!b) {
    x = 1;
    y = 0;
    return;
  }
  exgcd(b, a % b, x, y);
  const long long t = x;
  x = y;
  y = t - (a / b) * y;
}

inline unsigned long long inverse(const unsigned long long x,
                                  const long long mod)
{
  long long a, b;
  exgcd(x, mod, a, b);
  return (a % mod + mod) % mod;
}
inline pair<long long, long long> solveEquation(const unsigned long long a,
                                                const unsigned long long b)
{
  long long x, y;
  exgcd(a, b, x, y);
  return make_pair(x, y);
}
unsigned long long solve(unsigned long long c1, unsigned long long c2,
                         const unsigned long long e1,
                         const unsigned long long e2,
                         const unsigned long long n)
{
  auto [s, t] = solveEquation(e1, e2);
  if (s < 0) {
    swap(s, t);
    swap(c1, c2);
  }
  return multiply(quickPow(c1, s, n), quickPow(inverse(c2, n), -t, n), n);
}

int main()
{
#ifndef APTEST
  ios_base::sync_with_stdio(false);
#endif
  unsigned int t;
  cin >> t;
  for (unsigned int i = 0; i < t; ++i) {
    unsigned long long c1, c2, e1, e2, n;
    cin >> c1 >> c2 >> e1 >> e2 >> n;
    cout << solve(c1, c2, e1, e2, n) << "\n";
  }
  return 0;
}
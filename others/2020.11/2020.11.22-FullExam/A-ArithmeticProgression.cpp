#ifdef APTEST
#include "debug_tools/program.h"
#endif
#include <algorithm>
#include <cmath>
#include <iostream>
using namespace std;

long long exgcd(const long long a, const long long b, long long& x,
                long long& y)
{
  if (!b) {
    x = 1;
    y = 0;
    return a;
  }
  const long long g = exgcd(b, a % b, x, y);
  const long long t = x;
  x = y;
  y = t - (a / b) * y;
  return g;
}
pair<long long, long long> jumpRange(const long long l, const long long r,
                                     const long long x, long long dx)
{
  const bool pos = dx >= 0;
  if (!pos) dx = -dx;
  long long rl, rr;
  if (x <= l) {
    rl = (l - x + dx - 1) / dx;
    rr = (r - x) / dx;
  }
  else if (x >= r) {
    rl = -(x - l) / dx;
    rr = -(x - r + dx - 1) / dx;
  }
  else {
    rl = (x - l) / dx;
    rr = (r - x) / dx;
  }
  return pos ? make_pair(rl, rr) : make_pair(-rr, -rl);
}
unsigned long long count(const int a1, const int d1, const int a2, const int d2,
                         const long long c, const long long d)
{
  const static auto solveEqu = [](int a, int d, const long long v) {
    return (v - a) / d + 1;
  };
  long long x, y;
  const long long g = exgcd(d1, d2, x, y);
  const long long t = a2 - a1 + d1 - d2;
  if (t % g) return 0;
  y = -y;
  {
    const long long r = t / g;
    x *= r;
    y *= r;
  }
  const auto [xl, xr] =
      jumpRange(solveEqu(a1, d1, c), solveEqu(a1, d1, d), x, d2 / g);
  const auto [yl, yr] =
      jumpRange(solveEqu(a2, d2, c), solveEqu(a2, d2, d), y, d1 / g);
  return max(0ll, max(xr, yr) + 1 - min(xl, yl));
}

int main()
{
#ifndef APTEST
  ios_base::sync_with_stdio(false);
#endif
  unsigned int a1, a2, b1, b2;
  unsigned long long c, d;
  cin >> a1 >> a2 >> b1 >> b2 >> c >> d;
  cout << count(a1, a2 - a1, b1, b2 - b1, c, d) << "\n";
  return 0;
}
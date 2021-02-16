#ifdef APTEST
#include "debug_tools/program.h"
#endif
#include <algorithm>
#include <iostream>
using namespace std;
const unsigned int maxn = 2000, maxh = 1e5, maxw = 1e5;
constexpr unsigned long long mod = 1e9 + 7;

class Number {
 public:
  Number() = default;
  Number(const unsigned long long v) : val(v % mod){};
#define decl(x)                                                   \
  friend inline Number operator x(const Number l, const Number r) \
  {                                                               \
    return (l.val + mod) x r.val;                                 \
  }                                                               \
  inline Number& operator x##=(const Number r) { return *this = *this x r; }
  decl(+);
  decl(-);
  decl(*);
#undef decl
  friend inline ostream& operator<<(ostream& os, const Number v)
  {
    return os << v.val;
  }

 private:
  unsigned long long val = 0;
};
struct Point {
  unsigned int x, y;
} p[maxn + 10];
Number fac[maxh + maxw + 10], ifac[maxh + maxw + 10];

Number quickPow(Number v, unsigned long long e)
{
  Number ret = 1;
  for (; e; e >>= 1) {
    if (e & 0x01) ret *= v;
    v *= v;
  }
  return ret;
}
static void GetFactor(const unsigned int n)
{
  fac[0] = ifac[0] = 1;
  for (unsigned int i = 1; i <= n; ++i) {
    fac[i] = fac[i - 1] * i;
    ifac[i] = quickPow(fac[i], mod - 2);
  }
}
inline Number C(const unsigned int n, const unsigned int m)
{
  return n >= m ? fac[n] * ifac[n - m] * ifac[m] : 0;
}

inline Number Way(const Point a, const Point b)
{
  if (a.x > b.x || a.y > b.y) return 0;
  return C(b.x - a.x + b.y - a.y, b.x - a.x);
}
Number count(const unsigned int n, const unsigned int h, const unsigned int w)
{
  static Number f[maxn + 10];
  sort(p, p + n, [](const Point a, const Point b) {
    return a.x != b.x ? a.x < b.x : a.y < b.y;
  });
  f[0] = Way(Point{1, 1}, p[0]);
  p[n] = Point{h, w};
  for (unsigned int i = 1; i <= n; ++i) {
    f[i] = Way(Point{1, 1}, p[i]);
    for (unsigned int j = 0; j < i; ++j)
      f[i] -= f[j] * Way(p[j], p[i]);
  }
  return f[n];
}

int main()
{
#ifndef APTEST
  ios_base::sync_with_stdio(false);
#endif
  unsigned int h, w, n;
  cin >> h >> w >> n;
  GetFactor(h + w);
  for (Point* i = p; i < p + n; ++i)
    cin >> i->x >> i->y;
  cout << count(n, h, w) << "\n";
  return 0;
}
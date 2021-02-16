#ifdef APTEST
#include "debug_tools/program.h"
#endif
#include <algorithm>
#include <iostream>
#include <iterator>
using namespace std;
const unsigned int maxn = 3000, maxq = 3000;
const unsigned long long mod = 1e9 + 7;

class Number {
 public:
  Number() = default;
  Number(const unsigned long long v) : val(v % mod){};
#define decl(x)                                                   \
  inline friend Number operator x(const Number l, const Number r) \
  {                                                               \
    return (l.val + mod) x r.val;                                 \
  }                                                               \
  inline void operator x##=(const Number r)                       \
  {                                                               \
    val = ((val + mod) x r.val) % mod;                            \
  }
  decl(+);
  decl(-);
  decl(*);
#undef decl
  inline friend ostream& operator<<(ostream& os, const Number v)
  {
    return os << v.val;
  }

 private:
  unsigned long long val = 0;
};
struct Operation {
  unsigned int x, y;
} op[maxq + 10];
unsigned int a[maxn + 10];

Number quickPow(Number a, unsigned long long e)
{
  Number ret = 1;
  for (; e; e >>= 1) {
    if (e & 0x01) ret *= a;
    a *= a;
  }
  return ret;
}
Number dp(const unsigned int n, const unsigned int q)
{
  static Number f[maxn + 10][maxn + 10];
  static const Number zero = 0, one = 1;
  const Number inv2 = quickPow(2, mod - 2);
  for (unsigned int i = 0; i < n; ++i)
    for (unsigned int j = 0; j < n; ++j)
      f[i][j] = a[i] > a[j] ? one : zero;
  for (const Operation* i = op; i < op + q; ++i) {
    for (unsigned int j = 0; j < n; ++j)
      if (j != i->x && j != i->y) {
        f[j][i->x] = f[j][i->y] = inv2 * (f[j][i->x] + f[j][i->y]);
        f[i->x][j] = f[i->y][j] = inv2 * (f[i->x][j] + f[i->y][j]);
      }
    f[i->x][i->x] = f[i->y][i->y] = zero;
    f[i->x][i->y] = f[i->y][i->x] = inv2 * (f[i->x][i->y] + f[i->y][i->x]);
  }
  Number ret = 0;
  for (unsigned int i = 0; i < n; ++i)
    for (unsigned int j = i + 1; j < n; ++j)
      ret += f[i][j];
  return ret * quickPow(2, q);
}

int main()
{
#ifndef APTEST
  ios::sync_with_stdio(false);
#endif
  unsigned int n, q;
  cin >> n >> q;
  copy_n(istream_iterator<unsigned int>(cin), n, a);
  for (Operation* i = op; i < op + q; ++i) {
    cin >> i->x >> i->y;
    --(i->x);
    --(i->y);
    if (i->x > i->y) swap(i->x, i->y);
  }
  cout << dp(n, q) << "\n";
  return 0;
}
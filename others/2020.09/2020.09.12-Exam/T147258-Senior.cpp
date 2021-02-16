#ifdef APTEST
#include "debug_tools/program.h"
#endif
#include <iostream>
using namespace std;
const unsigned int maxn = 10000000;
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
  decl(*);
  decl(-);
#undef decl
  friend inline ostream& operator<<(ostream& os, const Number v)
  {
    return os << v.val;
  }

 private:
  unsigned long long val = 0;
};
Number quickPow(Number a, unsigned long long e)
{
  Number ret = 1;
  for (; e; e >>= 1) {
    if (e & 0x01) ret *= a;
    a *= a;
  }
  return ret;
}

Number fac[maxn * 2 + 10] = {1, 1}, ifac[maxn * 2 + 10] = {1, 1};

static void getInverse(const unsigned int n)
{
  for (unsigned int i = 2; i <= n; ++i)
    ifac[i] = mod - ifac[mod % i] * (mod / i);
}
static void getFactor(const unsigned int n)
{
  getInverse(n);
  for (unsigned int i = 2; i <= n; ++i) {
    fac[i] = fac[i - 1] * i;
    ifac[i] *= ifac[i - 1];
  }
}
inline Number C(const unsigned int n, const unsigned int m)
{
  return n >= m ? fac[n] * ifac[n - m] * ifac[m] : 0;
}
Number count(const unsigned int n, unsigned int x, unsigned int y)
{
  const static auto countPath = [](unsigned int x, unsigned int y) {
    return C(x + y, x) - C(x + y, x - 1);
  };
  if (x > y) swap(x, y);
  return countPath(x - 1, y - 1) * countPath(n - y, n - x);
}

int main()
{
#ifndef APTEST
  ios_base::sync_with_stdio(false);
#endif
  unsigned int t;
  cin >> t;
  getFactor(maxn * 2);
  for (unsigned int i = 0; i < t; ++i) {
    unsigned int n, x, y;
    cin >> n >> x >> y;
    cout << count(n, x, y) << "\n";
  }
  return 0;
}

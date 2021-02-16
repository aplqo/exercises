#ifdef APTEST
#include "debug_tools/program.h"
#endif
#include <algorithm>
#include <iostream>
using namespace std;
const unsigned int mod = 10007;

class Number {
 public:
  Number() = default;
  Number(const unsigned int i) : val(i % mod){};
#define decl(x)                                                   \
  friend inline Number operator x(const Number l, const Number r) \
  {                                                               \
    return (l.val + mod) x r.val;                                 \
  }                                                               \
  inline Number operator x##=(const Number r) { return *this = *this x r; }
  decl(+);
  decl(*);
#undef decl
  friend inline ostream& operator<<(ostream& os, const Number v)
  {
    return os << v.val;
  }

 private:
  unsigned int val = 0;
};
Number fac[mod + 1], ifac[mod + 1];

Number quickPow(Number a, unsigned int e)
{
  Number ret = 1;
  for (; e; e >>= 1) {
    if (e & 0x01) ret *= a;
    a *= a;
  }
  return ret;
}
static void initFactor()
{
  fac[0] = ifac[0] = 1;
  for (unsigned int i = 1; i < mod; ++i) {
    fac[i] = fac[i - 1] * i;
    ifac[i] = quickPow(fac[i], mod - 2);
  }
}
inline Number C(const unsigned int n, const unsigned int m)
{
  return n >= m ? fac[n] * ifac[m] * ifac[n - m] : 0;
}
Number com(unsigned long long n, unsigned long long m)
{
  Number ret = 1;
  while (n && m) {
    ret *= C(n % mod, m % mod);
    n /= mod;
    m /= mod;
  }
  return ret;
}
int main()
{
#ifndef APTEST
  ios_base::sync_with_stdio(false);
#endif
  unsigned int t;
  cin >> t;
  initFactor();
  for (unsigned int i = 0; i < t; ++i) {
    unsigned long long n, m;
    cin >> n >> m;
    cout << com(n, m) << "\n";
  }
  return 0;
}
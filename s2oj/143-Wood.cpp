#ifdef APTEST
#include "debug_tools/program.h"
#endif
#include <algorithm>
#include <iostream>
#include <iterator>
using namespace std;
const unsigned int maxn = 300000;
constexpr unsigned long long mod = 1e9 + 7;

class Number {
 public:
  Number(const unsigned long long v = 0) : val(v % mod){};
#define decl(x)                                                   \
  inline friend Number operator x(const Number l, const Number r) \
  {                                                               \
    return (l.val + mod) x r.val;                                 \
  }                                                               \
  inline void operator x##=(const Number r) { *this = *this x r; }
  decl(+);
  decl(*);
  decl(-);
#undef decl
  inline friend ostream& operator<<(ostream& os, const Number v)
  {
    return os << v.val;
  }

 private:
  unsigned long long val;
};
Number inv[maxn + 10];
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
static void initInv(const unsigned int n)
{
  inv[1] = 1;
  for (unsigned int i = 2; i <= n; ++i)
    inv[i] = inv[i - 1] + quickPow(i, mod - 2);
}
Number factor(const unsigned int n)
{
  Number ret = 1;
  for (unsigned int i = 2; i <= n; ++i)
    ret *= i;
  return ret;
}
Number count(const unsigned int n)
{
  const Number fp = factor(n);
  Number ret = 0;
  for (unsigned int i = 1; i <= n; ++i)
    ret += a[i] * fp * (inv[n + 1 - i] + inv[i] - 1);
  return ret;
}

int main()
{
#ifndef APTEST
  ios_base::sync_with_stdio(false);
#endif
  unsigned int n;
  cin >> n;
  copy_n(istream_iterator<unsigned int>(cin), n, a + 1);
  initInv(n);
  cout << count(n) << "\n";
  return 0;
}
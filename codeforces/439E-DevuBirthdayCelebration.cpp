#ifdef APTEST
#include "debug_tools/program.h"
#endif
#include <cstdio>
#include <iostream>
#include <vector>
using namespace std;
const unsigned int maxf = 1e5, maxq = 1e5;
constexpr unsigned long long mod = 1e9 + 7;

char outbuf[maxq * 15 + 1000], *outpos = outbuf;

class Number {
 public:
  Number() = default;
  Number(unsigned long long v) : val(v % mod) {}
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
  friend inline void writeNumber(const Number v)
  {
    outpos += sprintf(outpos, "%llu\n", v.val);
  }

 private:
  unsigned long long val = 0;
};
inline Number quickPow(Number a, unsigned long long e)
{
  Number ret = 1;
  for (; e; e >>= 1) {
    if (e & 0x01) ret *= a;
    a *= a;
  }
  return ret;
}
vector<unsigned int> factors[maxf + 10];
Number fac[maxf + 10], ifac[maxf + 10];

static void getProduct(const unsigned n)
{
  fac[0] = ifac[0] = 1;
  for (unsigned int i = 1; i <= n; ++i) {
    fac[i] = fac[i - 1] * i;
    ifac[i] = quickPow(fac[i], mod - 2);
  }
}
inline Number C(const unsigned int n, const unsigned int m)
{
  return n >= m ? fac[n] * ifac[m] * ifac[n - m] : 0;
}
static void getFactor(const unsigned int n)
{
  for (unsigned int i = 1; i <= n; ++i)
    for (unsigned int j = i * 2; j <= n; j += i)
      factors[j].push_back(i);
}
Number dp(const unsigned int people, const unsigned int sweet)
{
  static Number mem[maxf + 10];
  static unsigned int vis[maxf + 10];
  if (!sweet || !people || sweet < people || (people == 1 && sweet != 1))
    return 0;
  if (vis[sweet] == people) return mem[sweet];
  vis[sweet] = people;
  Number acc = 0;
  for (unsigned int j : factors[sweet])
    acc += dp(people, j);
  mem[sweet] = C(sweet - 1, people - 1) - acc;
  return mem[sweet];
}

int main()
{
#ifndef APTEST
  ios_base::sync_with_stdio(false);
#endif
  unsigned int q;
  cin >> q;
  getFactor(maxf + 1);
  getProduct(maxf + 1);
  for (unsigned int i = 0; i < q; ++i) {
    unsigned int people, sweet;
    cin >> sweet >> people;
    writeNumber(dp(people, sweet));
  }
  cout.write(outbuf, outpos - outbuf);
  return 0;
}
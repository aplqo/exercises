#ifdef APTEST
#include "debug_tools/program.h"
#endif
#include <iostream>
using namespace std;
constexpr unsigned long long mod = 1e9 + 7;
const unsigned int maxk = 1000;

class Number {
 public:
  Number() = default;
  Number(const unsigned long long val) : val(val % mod){};
#define decl(x)                                                   \
  friend inline Number operator x(const Number l, const Number r) \
  {                                                               \
    return l.val x r.val;                                         \
  }                                                               \
  inline Number& operator x##=(const Number r) { return *this = *this x r; }
  decl(+);
  decl(*);
#undef decl
  friend inline ostream& operator<<(ostream& os, const Number v)
  {
    return os << v.val;
  }
  friend inline istream& operator>>(istream& is, Number& v)
  {
    return is >> v.val;
  }

 private:
  unsigned long long val = 0;
};
inline Number quickPow(Number v, unsigned long long e)
{
  Number ret = 1;
  for (; e; e >>= 1) {
    if (e & 0x01) ret *= v;
    v *= v;
  }
  return ret;
}
unsigned int k;
Number a, b, ia, ib, iab;

Number dfs(const unsigned int ca, const unsigned int cab)
{
  static Number mem[maxk + 10][maxk + 10];
  static bool vis[maxk + 10][maxk + 10];
  if (ca + cab >= k) return ca + cab + a * ib;
  if (vis[ca][cab]) return mem[ca][cab];
  vis[ca][cab] = true;
  return mem[ca][cab] = (a * dfs(ca + 1, cab) + b * dfs(ca, cab + ca)) * iab;
}
int main()
{
#ifndef APTEST
  ios_base::sync_with_stdio(false);
#endif
  cin >> k >> a >> b;
  ia = quickPow(a, mod - 2);
  ib = quickPow(b, mod - 2);
  iab = quickPow(a + b, mod - 2);
  cout << dfs(1, 0) << "\n";
  return 0;
}
#ifdef APTEST
#include "debug_tools/program.h"
#endif
#include <algorithm>
#include <iostream>
#include <iterator>
using namespace std;
const unsigned int maxn = 100000;
constexpr unsigned long long mod = 1e9 + 7;

class Number {
 public:
  constexpr Number() = default;
  constexpr Number(const unsigned long long v) : val(v % mod){};
#define decl(x)                                                    \
  inline friend Number operator x(const Number l, const Number r); \
  inline Number& operator x##=(const Number r) { return *this = *this x r; }
  decl(+);
  decl(*);
#undef decl
  inline friend ostream& operator<<(ostream& os, const Number v);

 private:
  unsigned long long val = 0;
};
#define impl(x)                                            \
  inline Number operator x(const Number l, const Number r) \
  {                                                        \
    return l.val x r.val;                                  \
  }
impl(+);
impl(*);
#undef impl
inline ostream& operator<<(ostream& os, const Number v) { return os << v.val; }

Number fac[maxn * 2 + 1] = {1, 1}, ifac[maxn * 2 + 1] = {1, 1};
unsigned int l[maxn + 1], t[maxn + 1];

Number quickPow(Number v, unsigned long long e)
{
  Number ret = 1;
  for (unsigned long long i = 1; e; i <<= 1) {
    if (e & i) {
      ret *= v;
      e ^= i;
    }
    v *= v;
  }
  return ret;
}
static void init(const unsigned int n)
{
  for (unsigned int i = 2; i <= n; ++i) {
    fac[i] = fac[i - 1] * i;
    ifac[i] = quickPow(fac[i], mod - 2);
  }
}
Number calc(const unsigned int n, const unsigned int a, const unsigned int b)
{
  const static auto c = [](const unsigned int n, const unsigned int m) {
    return fac[n + m] * ifac[n] * ifac[m];
  };
  const Number cb = quickPow(b, n - 1), ca = quickPow(a, n - 1);

  Number ret = 0;
  Number pa = 1, pb = 1;
  for (unsigned int i = n; i > 1; --i) {
    ret += c(n - i, n - 2) * (pa * cb * t[i] + ca * pb * l[i]);
    pa *= a;
    pb *= b;
  }
  return ret;
}
int main()
{
#ifndef APTEST
  ios_base::sync_with_stdio(false);
#endif
  unsigned int n, a, b;
  cin >> n >> a >> b;
  copy_n(istream_iterator<unsigned int>(cin), n, l + 1);
  copy_n(istream_iterator<unsigned int>(cin), n, t + 1);
  init(2 * n);
  cout << calc(n, a, b) << endl;
  return 0;
}
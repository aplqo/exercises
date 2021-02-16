/*luogu team T133430: Jump*/
#ifdef APTEST
#include "debug_tools/program.h"
#endif
#include <algorithm>
#include <iostream>
using namespace std;
constexpr unsigned long long mod = 1e9 + 7;

class Number {
 public:
  Number() = default;
  Number(unsigned long long i) : val(i % mod) {}

#define alop(o)                                                    \
  friend inline Number operator o(const Number l, const Number r); \
  inline Number& operator o##=(const Number r) { return *this = *this o r; }
  alop(+);
  alop(*);
#undef alop
  friend inline Number factor(const Number i);
  friend inline ostream& operator<<(ostream& os, const Number v);

 private:
  unsigned long long val = 0;
};
#define impl(o)                                            \
  inline Number operator o(const Number l, const Number r) \
  {                                                        \
    return l.val o r.val;                                  \
  }
impl(+);
impl(*);
#undef impl
inline Number operator""_N(const unsigned long long i) { return i; }
inline Number operator^(Number a, unsigned long long e)
{
  Number ret = 1;
  for (unsigned long long i = 1; e; i <<= 1) {
    if (e & i) {
      ret *= a;
      e ^= i;
    }
    a *= a;
  }
  return ret;
}
inline Number operator/(const Number l, const Number r)
{
  Number inv = r ^ (mod - 2);
  return l * inv;
}
inline ostream& operator<<(ostream& os, const Number v) { return os << v.val; }
inline Number factor(const unsigned long long a, const unsigned int b)
{
  Number ret = 1;
  for (unsigned long long i = b; i > a; --i)
    ret *= i;
  return ret;
}

Number c(unsigned long long m, unsigned long long n)
{
  const static auto co = [](const unsigned long long j, unsigned long long i) {
    i = min(i, j - i);
    return factor(j - i, j) / factor(0, i);
  };
  Number ret = 1;
  while (n && m) {
    ret *= co(m % mod, n % mod);
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
  unsigned long long n, m;
  cin >> n >> m;
  if (n < m) swap(n, m);
  cout << c(n + m + 1, n + 1) + n << endl;
  return 0;
}
#ifdef APTEST
#include "debug_tools/program.h"
#endif
#include <cstring>
#include <iostream>
using namespace std;
const unsigned int maxl = 1e5;
constexpr unsigned long long mod = 1e9 + 7;

class Number {
 public:
  Number() = default;
  Number(const unsigned long long v) : val(v % mod) {}
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
Number operator^(Number a, unsigned long long e)
{
  Number ret = 1;
  for (; e; e >>= 1) {
    if (e & 0x01) ret *= a;
    a *= a;
  }
  return ret;
}
inline Number operator/(const Number l, const Number r)
{
  return l * (r ^ (mod - 2));
}
char str[maxl + 10];

inline Number sum(const Number a, const unsigned int l, const unsigned int r)
{
  return ((a ^ (r + 1)) - (a ^ l)) / (a - 1);
}
Number count(const unsigned int k)
{
  const size_t siz = strlen(str);
  const Number base = sum(Number(2) ^ siz, 0, k - 1);
  Number ret = 0, acc = 1;
  for (unsigned int i = 0; i < siz; ++i, acc *= 2)
    if (str[i] == '0' || str[i] == '5') ret += acc * base;
  return ret;
}

int main()
{
#ifndef APTEST
  ios_base::sync_with_stdio(false);
#endif
  unsigned int k;
  cin >> str >> k;
  cout << count(k) << "\n";
  return 0;
}

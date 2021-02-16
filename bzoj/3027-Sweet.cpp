#ifdef APTEST
#include "debug_tools/program.h"
#endif
#include <iostream>
#include <map>
using namespace std;
const unsigned int maxn = 10;
constexpr unsigned int mod = 2004;

class Number {
 public:
  Number() = default;
  Number(const unsigned long long i) : val(i % mod){};
#define decl(x)                                                   \
  inline friend Number operator x(const Number l, const Number r) \
  {                                                               \
    return (l.val + mod) x r.val;                                 \
  }                                                               \
  inline void operator x##=(const Number r) { *this = *this x r; }
  decl(+);
  decl(-);
  decl(*);
#undef decl
  friend inline ostream& operator<<(ostream& os, const Number v)
  {
    return os << v.val;
  }

 private:
  unsigned int val = 0;
};
unsigned int m[maxn + 10];

map<unsigned int, Number> expand(const unsigned int n)
{
  map<unsigned int, Number> ret;
  ret[0] = 1;
  for (unsigned int i = 0; i < n; ++i) {
    const unsigned int cm = m[i] + 1;
    for (auto j = prev(ret.end()); j != ret.begin(); --j)
      ret[j->first + cm] -= j->second;
    ret[0 + cm] -= ret.at(0);
  }
  return ret;
}
unsigned long long factor(const unsigned int n)
{
  unsigned long long ret = 1;
  for (unsigned int i = n; i; --i)
    ret *= i;
  return ret;
}
Number C(const unsigned int n, unsigned int m)
{
  if (n < m) return 0;
  m = max(m, n - m);
  const unsigned long long f = factor(n - m);
  unsigned long long ret = 1, cm = f * mod;
  for (unsigned int i = m + 1; i <= n; ++i)
    ret = (ret * i) % cm;
  return ret / f;
}
Number count(map<unsigned int, Number>&& mp, const unsigned int n,
             const unsigned int a, const unsigned int b)
{
  Number ret = 0;
  for (auto i : mp)
    if (i.first <= b)
      ret += i.second *
             (C(n + b - i.first, b - i.first) -
              (a >= i.first + 1 ? C(n + a - i.first - 1, a - i.first - 1) : 0));
  return ret;
}

int main()
{
#ifndef APTEST
  ios_base::sync_with_stdio(false);
#endif
  unsigned int n, a, b;
  cin >> n >> a >> b;
  for (unsigned int i = 0; i < n; ++i)
    cin >> m[i];
  cout << count(expand(n), n, a, b) << "\n";
  return 0;
}
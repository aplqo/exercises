#ifdef APTEST
#include "debug_tools/program.h"
#endif
#include <iostream>
#include <map>
#include <utility>
using namespace std;
const unsigned int maxn = 5000000;
constexpr unsigned long long mod = 998244353, inv2 = 499122177;

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
const map<unsigned long long, pair<unsigned int, unsigned int>> Factors{
    {1, {0, 0}},
    {3, {1, 0}},
    {4, {0, 1}},
    {12, {1, 1}},
    {30, {3, 0}},
    {147203573614806055ull, {5, 0}},
    {371216956151518818ull, {6, 0}},
    {834586893457709917ull, {4, 0}},
    {1147387575560213988ull, {3, 7}},
    {608358758975305374, {3, 3}},
    {710701671428663075, {2, 3}},
    {714115052266263644, {2, 3}},
    {979573735390975739, {2, 3}},
    {640807389338647549, {2, 3}},
    {598480316906172486, {3, 3}},
    {203522456999371050, {2, 4}},
    {421206431991626060, {2, 4}},
    {595630806517176908, {2, 3}},
    {573010858348910652, {3, 3}},
    {812626144076193076, {2, 4}}};
Number twoAns[maxn + 10] = {1, 1, 3}, fac[maxn + 10];

Number operator^(Number a, unsigned long long e)
{
  Number ret = 1;
  for (; e; e >>= 1) {
    if (e & 0x01) ret *= a;
    a *= a;
  }
  return ret;
}
pair<unsigned int, unsigned int> factor(unsigned long long v)
{
  unsigned int one = 0, two = 0;
  for (unsigned long long i = 2; i * i <= v && v > 1; ++i) {
    if (v % i) continue;
    v /= i;
    if (v % i)
      ++one;
    else {
      v /= i;
      ++two;
    }
  }
  if (v != 1) ++one;
  return make_pair(one, two);
}
static void getFac(const unsigned int n)
{
  fac[0] = 1;
  for (unsigned int i = 1; i <= n; ++i)
    fac[i] = fac[i - 1] * i;
}
static void dp(const unsigned int n)
{
  for (unsigned int i = 3; i <= n; ++i)
    twoAns[i] =
        twoAns[i - 1] * i * i - twoAns[i - 2] * i * (Number(i - 1) ^ 2) * inv2;
}
Number count(const pair<unsigned int, unsigned int> p, const unsigned int n)
{
  return (Number(2) ^ (1ull * (n - 1) * (n - 1))) * (fac[n] ^ p.first) *
         (twoAns[n] ^ p.second);
}

int main()
{
#ifndef APTEST
  ios_base::sync_with_stdio(false);
#endif
  unsigned long long x;
  unsigned int t;
  cin >> x >> t;
  const pair<unsigned int, unsigned int> f = Factors.at(x);
  dp(maxn);
  getFac(maxn);
  for (unsigned int i = 0; i < t; ++i) {
    unsigned int n;
    cin >> n;
    cout << count(f, n) << "\n";
  }
  return 0;
}
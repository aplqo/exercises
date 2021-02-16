#ifdef APTEST
#include "debug_tools/program.h"
#endif
#include <cmath>
#include <iostream>
using namespace std;
using factor_t = unsigned long long;
const int maxn = 3e5;
const unsigned int amod = 10007;

class Number {
 public:
  Number() = default;
  Number(unsigned long long i) : val(i % mod){};
#define decl(x)                                                \
  inline friend Number operator x(const Number, const Number); \
  inline Number& operator x##=(const Number r) { return *this = *this x r; }
  decl(+);
  decl(*);
#undef decl
  friend Number operator^(Number, unsigned long long);
  inline friend ostream& operator<<(ostream&, const Number);
  inline explicit operator unsigned long long() const { return val; }

  static unsigned long long mod;

 private:
  unsigned long long val = 0;
};
unsigned long long Number::mod;
#define impl(x)                                            \
  inline Number operator x(const Number l, const Number r) \
  {                                                        \
    return l.val x r.val;                                  \
  }
impl(+);
impl(*);
#undef impl
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
inline ostream& operator<<(ostream& os, const Number v) { return os << v.val; }

unsigned long long a[maxn + 10];
unsigned int mindiv[maxn + 10], primes[maxn + 10], *pcur = primes;
factor_t fac[maxn + 10] = {0, 0};

void calcA(const unsigned int n)
{
  Number lst[3] = {a[1], a[2], a[3]};
  for (unsigned int i = 4; i <= n; ++i) {
    a[i] = static_cast<unsigned long long>(lst[0] + lst[1] + lst[2]);
    lst[(i - 1) % 3] = a[i];
  }
}
unsigned int logBase(const unsigned int b, unsigned long long v)
{
  unsigned int ret = 0;
  while (!(v % b)) {
    ++ret;
    v /= b;
  }
  return ret;
}
void eular(const unsigned int n)
{
  for (unsigned int i = 2; i <= n; ++i) {
    if (!mindiv[i]) {
      mindiv[i] = i;
      *(pcur++) = i;
      fac[i] = 2;
    }
    for (unsigned int* j = primes; j < pcur && *j < mindiv[i] && *j * i <= n;
         ++j) {
      mindiv[*j * i] = *j;
      fac[*j * i] = fac[i] * 2;
    }
    const unsigned long long mv = 1ull * mindiv[i] * i;
    if (mv <= n) {
      mindiv[mv] = mindiv[i];
      const unsigned int t = logBase(mindiv[i], i);
      fac[mv] = (fac[i] / (t + 1)) * (t + 2);
    }
  }
}
Number solve(const unsigned int n)
{
  Number::mod = amod;
  Number ans = 1;
  for (unsigned int i = 2; i <= n; ++i) {
    Number cv = Number(i) ^ (fac[i] >> 1);
    if (fac[i] & 0x01) cv *= static_cast<unsigned int>(sqrt(i));
    ans += cv ^ a[i];
  }
  return ans;
}
int main()
{
#ifndef APTEST
  ios_base::sync_with_stdio(false);
#endif
  unsigned int n;
  cin >> n >> Number::mod >> a[1] >> a[2] >> a[3];
  eular(n);
  calcA(n);
  cout << solve(n) << endl;
  return 0;
}
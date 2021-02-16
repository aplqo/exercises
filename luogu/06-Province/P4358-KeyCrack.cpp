#ifdef APTEST
#include "debug_tools/program.h"
#endif
#include <iostream>
#include <numeric>
#include <random>
using namespace std;

unsigned long long multiply(unsigned long long a, unsigned long long b,
                            const unsigned long long mod)
{
  unsigned long long ret = 0;
  for (; b; b >>= 1) {
    if (b & 0x01) ret = (ret + a) % mod;
    a = (a + a) % mod;
  }
  return ret;
}
unsigned long long quickPow(unsigned long long a, unsigned long long e,
                            const unsigned long long mod)
{
  unsigned long long ret = 1;
  for (; e; e >>= 1) {
    if (e & 0x01) ret = multiply(ret, a, mod);
    a = multiply(a, a, mod);
  }
  return ret;
}
pair<unsigned int, unsigned long long> lowbit(unsigned long long n)
{
  unsigned int ret = 0;
  while (!(n & 0x01)) {
    ++ret;
    n >>= 1;
  }
  return make_pair(ret, n);
}
bool millerRabin(const unsigned long long n)
{
  constexpr unsigned long long testA[] = {2,  3,  5,  7,  11, 13,
                                          17, 19, 23, 29, 31, 37};
  const auto [r, d] = lowbit(n);
  for (unsigned long long a : testA) {
    if (a > n - 2) break;
    unsigned long long x = quickPow(a, d, n);
    if (x == 1 || x == n - 1) continue;
    for (unsigned int i = 1; i < r; ++i) {
      x = multiply(x, x, n);
      if (x == n - 1) goto End;
    }
    return false;
  End:;
  }
  return true;
}
unsigned long long getRandom()
{
  static mt19937 rnd((random_device())());
  uniform_int_distribution<unsigned long long> dis(1);
  return dis(rnd);
}
unsigned long long pollardRho(const unsigned long long n)
{
  static const auto f = [](const unsigned long long x,
                           const unsigned long long c,
                           const unsigned long long mod) {
    return (multiply(x, x, mod) + c) % mod;
  };
  unsigned long long x = getRandom() % n, y = x, c = getRandom() % n;
  do {
    x = f(x, c, n);
    y = f(f(y, c, n), c, n);
    const unsigned long long g = gcd(x > y ? x - y : y - x, n);
    if (g != 1 && g != n) return g;
  } while (y != x);
  return 1;
}
pair<unsigned long long, unsigned long long> factor(const unsigned long long n)
{
  unsigned long long v = 1;
  while (v == 1)
    v = pollardRho(n);
  return make_pair(v, n / v);
}
void exgcd(const long long a, const long long b, long long& x, long long& y)
{
  if (!b) {
    x = 1;
    y = 0;
    return;
  }
  exgcd(b, a % b, x, y);
  const long long t = x;
  x = y;
  y = t - (a / b) * y;
}
unsigned long long inverse(const unsigned long long x, const long long mod)
{
  long long a, b;
  exgcd(x, mod, a, b);
  return (a % mod + mod) % mod;
}

unsigned long long crackPrivate(const unsigned long long publicKey,
                                const unsigned long long n)
{
  const auto [p, q] = factor(n);
  return inverse(publicKey, (p - 1) * (q - 1));
}

int main()
{
#ifndef APTEST
  ios_base::sync_with_stdio(false);
#endif
  unsigned long long e, n, c;
  cin >> e >> n >> c;
  const unsigned long long privateKey = crackPrivate(e, n);
  cout << privateKey << " " << quickPow(c, privateKey, n) << "\n";
  return 0;
}
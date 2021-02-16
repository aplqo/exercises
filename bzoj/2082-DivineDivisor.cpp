#ifdef APTEST
#include "debug_tools/program.h"
#endif
#include <algorithm>
#include <cmath>
#include <iomanip>
#include <iostream>
#include <numeric>
#include <random>
#include <sstream>
#include <string>
#include <unordered_map>
#include <utility>
using namespace std;

unordered_map<unsigned long long, unsigned int> fac;
ranlux24 rnd((random_device())());

unsigned long long multiply(__uint128_t a, unsigned long long b,
                            const unsigned long long mod)
{
  return (a * b) % mod;
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
inline pair<unsigned int, unsigned long long> lowbit(unsigned long long n)
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
  static constexpr unsigned long long testA[] = {2,  3,  5,  7,  11, 13,
                                                 17, 19, 23, 29, 31, 37};
  const auto [r, d] = lowbit(n - 1);
  for (const unsigned long long a : testA) {
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
inline unsigned long long getRandom(const unsigned int n)
{
  uniform_int_distribution<unsigned long long> dis(1, n);
  return dis(rnd);
}
unsigned long long pollardRho(const unsigned long long n)
{
  static const auto f = [](const unsigned long long x, const unsigned int c,
                           const unsigned long long m) {
    return (multiply(x, x, m) + c) % m;
  };
  const unsigned long long c = getRandom(n - 1);
  unsigned long long x = getRandom(n - 1), y = x;
  for (unsigned int i = 1; i; i <<= 1) {
    unsigned long long acc = 1;
    for (unsigned int j = 0; j < i; ++j) {
      y = f(y, c, n);
      acc = multiply(acc, (x > y ? x - y : y - x), n);
      if ((j & 0x7fu) == 0) {
        const unsigned long long g = gcd(acc, n);
        if (g > 1) return g;
      }
    }
    const unsigned long long g = gcd(acc, n);
    if (g > 1) return g;
    x = y;
  }
  return 1;
}
void factor(const unsigned long long v)
{
  if (millerRabin(v)) {
    auto it = fac.find(v);
    if (it == fac.end()) it = fac.insert({v, 0}).first;
    ++(it->second);
    return;
  }
  unsigned long long g = 1;
  while (g == 1 || g >= v)
    g = pollardRho(v);
  factor(g);
  factor(v / g);
}
inline pair<unsigned int, unsigned int> maxExp()
{
  unsigned int ret = 0, cnt = 0;
  for (const auto i : fac)
    if (i.second > ret) {
      ret = i.second;
      cnt = 1;
    }
    else if (i.second == ret)
      ++cnt;
  return make_pair(ret, cnt);
}
string toString(const long double d)
{
  ostringstream os;
  os << fixed << setprecision(0) << d;
  return os.str();
}

int main()
{
#ifndef APTEST
  ios_base::sync_with_stdio(false);
#endif
  unsigned int m;
  cin >> m;
  fac.reserve(m * 64);
  for (unsigned int i = 0; i < m; ++i) {
    unsigned long long v;
    cin >> v;
    factor(v);
  }
  const auto [k, c] = maxExp();
  string sc = toString(pow(2.0L, c));
  --sc.back();
  cout << k << "\n" << sc << "\n";
  return 0;
}
#ifdef APTEST
#include "debug_tools/program.h"
#endif
#include <algorithm>
#include <climits>
#include <cmath>
#include <iostream>
#include <map>
#include <utility>
using namespace std;

unsigned long long mod;
long long exgcd(const long long a, const long long b, long long& x,
                long long& y)
{
  if (!b) {
    x = 1;
    y = 0;
    return a;
  }
  const long long g = exgcd(b, a % b, x, y);
  const long long t = x;
  x = y;
  y = t - (a / b) * y;
  return g;
}
inline pair<unsigned long long, unsigned long long> inverse(long long y,
                                                            long long p)
{
  long long a, b;
  const unsigned long long g = exgcd(y, p, a, b);
  if (y % p == 1 || y % p == 0) return make_pair(g, 1);
  return make_pair(g, ((a + p) % p + p) % p);
}
unsigned long long quickPow(unsigned long long a, unsigned long long e)
{
  unsigned long long ret = 1 % mod;
  for (; e; e >>= 1) {
    if (e & 0x01) ret = (ret * a) % mod;
    a = (a * a) % mod;
  }
  return ret;
}
unsigned long long bsgs(unsigned long long y, unsigned long long z,
                        const unsigned long long r)
{
  const unsigned long long step = sqrt(mod);
  multimap<unsigned long long, unsigned long long> mem;
  {
    unsigned long long acc = z % mod;
    for (unsigned long long i = 0; i < step; ++i) {
      mem.insert({acc, i});
      acc = (acc * y) % mod;
    }
  }
  {
    const unsigned long long s = quickPow(y, step);
    unsigned long long acc = 1 % mod;
    for (unsigned long long i = 0; i < 2 * step; ++i) {
      {
        const auto it = mem.equal_range(acc);
        bool found = false;
        unsigned long long ret = ULLONG_MAX;
        for (auto j = it.first; j != it.second; ++j)
          if (i * step >= j->second && quickPow(y, i * step - j->second) == z) {
            ret = min(ret, i * step + r - j->second);
            found = true;
          }
        if (found) return ret;
      }
      acc = (acc * s) % mod;
    }
  }
  return ULLONG_MAX;
}
unsigned long long exbsgs(unsigned long long y, unsigned long long z)
{
  unsigned long long r = 0;
  for (pair<unsigned long long, unsigned long long> g = inverse(y, mod);
       true;) {
    if (g.first == 1 || z == 1) break;
    mod /= g.first;
    if (z % g.first) return ULLONG_MAX;
    z = (z / g.first) % mod;
    ++r;
    z = (z * inverse(y / g.first, mod).second) % mod;
    g = inverse(y, mod);
  }
  return bsgs(y, z, r);
}

int main()
{
#ifndef APTEST
  ios_base::sync_with_stdio(false);
#endif
  while (true) {
    unsigned long long a, b;
    cin >> a >> mod >> b;
    if (!a && !mod && !b) break;
    b %= mod;
    const unsigned long long v = exbsgs(a, b);
    if (v == ULLONG_MAX)
      cout << "No Solution" << endl;
    else
      cout << v << endl;
  }
  return 0;
}
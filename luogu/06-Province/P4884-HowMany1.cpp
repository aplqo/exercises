#ifdef APTEST
#include "debug_tools/program.h"
#endif
#include <cmath>
#include <iostream>
#include <map>
using namespace std;

unsigned long long mod;

unsigned long long Multiply(unsigned long long v, unsigned long long b)
{
  unsigned long long ret = 0;
  for (; b; b >>= 1) {
    if (b & 0x01) ret = (ret + v) % mod;
    v = (v + v) % mod;
  }
  return ret;
}
unsigned long long quickPow(unsigned long long v, unsigned long long b)
{
  unsigned long long ret = 1;
  for (; b; b >>= 1) {
    if (b & 0x01) ret = Multiply(ret, v);
    v = Multiply(v, v);
  }
  return ret;
}
unsigned long long bsgs(const unsigned long long k)
{
  const unsigned long long step = ceil(sqrt(mod));
  map<unsigned long long, unsigned long long> mem;
  {
    unsigned long long acc = k;
    for (unsigned int i = 0; i < step; ++i) {
      mem[acc] = i;
      acc = (acc * 10) % mod;
    }
  }
  {
    unsigned long long acc = 1;
    const unsigned long long s = quickPow(10, step);
    for (unsigned int i = 0; i < 2 * step; ++i) {
      if (auto it = mem.find(acc); it != mem.end() && i * step > it->second)
        return i * step - it->second;
      acc = Multiply(acc, s);
    }
  }
  return 0;
}
int main()
{
#ifndef APTEST
  ios_base::sync_with_stdio(false);
#endif
  unsigned long long k;
  cin >> k >> mod;
  cout << bsgs((k * 9 + 1) % mod) << endl;
  return 0;
}
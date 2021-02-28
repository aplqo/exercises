#ifdef APTEST
#include "debug_tools/judge.h"
#endif
#include <algorithm>
#include <iostream>
const int maxn = 500;
constexpr unsigned long long mod = 1e9 + 7;

int cnt[maxn + 10];

unsigned long long quickPow2(unsigned long long e)
{
  unsigned long long ret = 1, acc = 2;
  for (; e; e >>= 1) {
    if (e & 0x01) ret = (ret * acc) % mod;
    acc = (acc * acc) % mod;
  }
  return ret;
}
unsigned long long solve(const int n)
{
  unsigned long long ret = 0;
  for (const int *i = cnt; i < cnt + n; ++i)
    if (*i) ret = (ret + mod + 1 - quickPow2(*i - 1)) % mod;
  return (ret + quickPow2(n - 1) - 1) % mod;
}

int main()
{
  std::ios::sync_with_stdio(false);
  int n;
  std::cin >> n;
  for (int i = 0; i < n; ++i) {
    int f;
    std::cin >> f;
    ++cnt[f == -1 ? i : f - 1];
  }
  std::cout << solve(n) << "\n";
  return 0;
}
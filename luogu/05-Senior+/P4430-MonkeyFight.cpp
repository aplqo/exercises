#ifdef APTEST
#include "debug_tools/program.h"
#endif
#include <iostream>
using namespace std;
constexpr unsigned long long mod = 9999991;

unsigned long long factor(const unsigned int n)
{
  unsigned long long ret = 1;
  for (unsigned int i = 2; i <= n; ++i)
    ret = (ret * i) % mod;
  return ret;
}
unsigned long long quickPow(unsigned long long a, unsigned int e)
{
  unsigned long long ret = 1;
  for (; e; e >>= 1) {
    if (e & 0x01) ret = (ret * a) % mod;
    a = (a * a) % mod;
  }
  return ret;
}
int main()
{
#ifndef APTEST
  ios_base::sync_with_stdio(false);
#endif
  unsigned int n;
  cin >> n;
  cout << (quickPow(n, n - 2) * factor(n - 1)) % mod;
  return 0;
}
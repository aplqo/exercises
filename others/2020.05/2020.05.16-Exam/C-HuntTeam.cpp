/*Luogu team T133193: Treasure hunt team*/
#ifdef APTEST
#include "debug_tools/program.h"
#endif
#include <iostream>
using namespace std;
constexpr unsigned long long mod = 1e9 + 7;

unsigned long long QuickPow(unsigned long long a, unsigned int e)
{
  unsigned long long ret = 1;
  a %= mod;
  for (unsigned int i = 1; e; i <<= 1) {
    if (e & i) {
      ret = (ret * a) % mod;
      e ^= i;
    }
    a = (a * a) % mod;
  }
  return ret;
}
unsigned long long factor(const unsigned int a, const unsigned int b)
{
  unsigned long long ret = 1;
  for (unsigned int i = b; i > a; --i)
    ret = (ret * i) % mod;
  return ret;
}

int main()
{
#ifndef APTEST
  ios_base::sync_with_stdio(false);
#endif
  unsigned long long n, m;
  cin >> n >> m;
  const auto c = [](unsigned int a, unsigned int b) {
    return (factor(a - b, a) * QuickPow(factor(0, b), mod - 2)) % mod;
  };
  cout << (c(n + 1, m + 1) * ((m + 1) >> 1)) % mod << endl;
  return 0;
}
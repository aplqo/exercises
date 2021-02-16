/*Luogu team T133184: Count graphs*/
#ifdef APTEST
#include "debug_tools/program.h"
#endif
#include <iostream>
using namespace std;
const int maxn = 1000;
constexpr unsigned long long mod = 1000000009;

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
  static unsigned long long mem[maxn + 10][maxn + 10] = {};
  if (mem[a][b]) return mem[a][b];
  unsigned long long ret = 1;
  for (unsigned int i = b; i > a; --i)
    ret = (ret * i) % mod;
  return mem[a][b] = ret;
}
unsigned long long c(const unsigned int m, const unsigned int n)
{
  if (!n || n >= m) return 1;
  static unsigned long long mem[maxn + 10][maxn + 10];
  if (mem[m][n]) return mem[m][n];
  return mem[m][n] =
             (factor(m - n, m) * (QuickPow(factor(0, n), mod - 2))) % mod;
}
unsigned long long f[maxn + 10] = {1, 1, 1};

int main()
{
  unsigned int n;
  cin >> n;
  for (unsigned int i = 3; i <= n; ++i) {
    static const auto fun = [](const unsigned int i) {
      return QuickPow(2, i * (i - 1) / 2);
    };
    f[i] = fun(i);
    for (unsigned int j = 1; j < i; ++j) {
      unsigned long long cv = (c(i - 1, min(j - 1, i - j)) * f[j]) % mod;
      cv = (cv * fun(i - j)) % mod;
      f[i] = (f[i] + mod - cv) % mod;
    }
  }
  cout << f[n] << endl;
  return 0;
}
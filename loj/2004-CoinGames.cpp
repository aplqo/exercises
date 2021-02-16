#ifdef APTEST
#include "debug_tools/judge.h"
#endif
#include <algorithm>
#include <cmath>
#include <iostream>
const unsigned int maxn = 300, maxm = 300;
constexpr unsigned long long base = 3;
typedef double Real;
constexpr Real eps = 1e-8;

bool seq[maxn + 10][maxm + 10];
unsigned long long hash[maxn + 10][maxm + 10];
Real equ[maxn + 10][maxn + 10], pow2[maxm + 10];

namespace Hash {
unsigned long long powBase[maxn + 10];
static void initPow(const unsigned int n)
{
  powBase[0] = 1;
  for (unsigned int i = 1; i <= n; ++i)
    powBase[i] = powBase[i - 1] * base;
}
void getHash(const unsigned n, const bool str[], unsigned long long dest[])
{
  for (unsigned int i = 1; i <= n; ++i)
    dest[i] = dest[i - 1] * base + str[i];
}
inline unsigned long long substrHash(const unsigned long long hash[],
                                     const unsigned int l, const unsigned int r)
{
  return hash[r] - hash[l - 1] * powBase[r + 1 - l];
}
inline bool equal(const unsigned long long pre[],
                  const unsigned long long suf[], const unsigned int n,
                  const unsigned int l)
{
  return substrHash(pre, 1, l) == substrHash(suf, n - l + 1, n);
}
}  // namespace Hash

static void initPow2(const unsigned int n)
{
  pow2[0] = 1;
  for (unsigned int i = 1; i <= n; ++i)
    pow2[i] = pow2[i - 1] * Real(2.0);
}
static void init(const unsigned int n, const unsigned int m)
{
  initPow2(m);
  Hash::initPow(m);
  for (unsigned int i = 1; i <= n; ++i)
    Hash::getHash(m, seq[i], hash[i]);
}
void gaussian(const unsigned int n)
{
  for (unsigned int i = 0; i < n; ++i) {
    if (std::fabs(equ[i][i]) < eps) {
      for (unsigned int j = i + 1; j < n; ++j)
        if (std::fabs(equ[j][i]) >= eps) {
          std::swap_ranges(equ[j] + i, equ[j] + n + 1, equ[i] + i);
          break;
        }
    }
    for (unsigned int j = i + 1; j < n; ++j) {
      const Real r = equ[j][i] / equ[i][i];
      for (unsigned int k = i; k <= n; ++k)
        equ[j][k] -= equ[i][k] * r;
    }
  }
  for (unsigned int i = n - 1; i; --i) {
    equ[i][n] /= equ[i][i];
    for (unsigned int j = 0; j < i; ++j)
      equ[j][n] -= equ[j][i] * equ[i][n];
  }
}
void buildEquation(const unsigned int n, const unsigned int m)
{
  for (unsigned int i = 1; i <= n; ++i) {
    equ[i][0] = -1;
    for (unsigned int j = 1; j <= n; ++j)
      for (unsigned int k = 1; k <= m; ++k)
        if (Hash::equal(hash[i], hash[j], m, k)) equ[i][j] += pow2[k];
  }
  for (unsigned int i = 1; i <= n; ++i)
    equ[0][i] = 1;
  equ[0][n + 1] = 1;
}

int main()
{
  std::ios::sync_with_stdio(false);
  unsigned int n, m;
  std::cin >> n >> m;
  for (unsigned int i = 1; i <= n; ++i) {
    char buf[maxm + 10];
    std::cin >> (buf + 1);
    for (unsigned int j = 1; j <= m; ++j)
      seq[i][j] = buf[j] == 'H';
  }
  init(n, m);
  buildEquation(n, m);
  gaussian(n + 1);
  std::cout.precision(7);
  for (unsigned int i = 1; i <= n; ++i)
    std::cout << equ[i][n + 1] << "\n";
  return 0;
}
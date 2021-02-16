#ifdef APTEST
#include "debug_tools/program.h"
#endif
#include <algorithm>
#include <functional>
#include <iostream>
#include <utility>
#include <valarray>
using namespace std;
const int maxn = 2000, maxf = 10;

valarray<int> fact[maxn + 10], lg[maxn + 10];
valarray<int> fexp(maxf);
unsigned int flg[maxn + 10];
unsigned int mem[maxn + 10][maxn + 10];
unsigned int fac[maxf + 10];

unsigned int factor(unsigned int k)
{
  static unsigned int primes[] = {2, 3, 5, 7, 11, 13, 17, 19};
  unsigned int ret = 0;
  for (auto i : primes) {
    if (k % i) continue;
    fac[ret] = i;
    while (k % i == 0) {
      ++fexp[ret];
      k /= i;
    }
    ++ret;
  }
  fexp = fexp[slice(0, ret, 1)];
  return ret;
}
void init(const unsigned int n, const unsigned int k)
{
  const unsigned int f = factor(k);
  const auto res = [&f](valarray<int>& i) { i.resize(f); };
  for_each(lg, lg + n + 1, res);
  for_each(fact, fact + n + 1, res);
  for (unsigned int i = 2; i <= n; ++i)
    for (unsigned int j = 0; j < f; ++j)
      if (i % fac[j] == 0) lg[i][j] = lg[i / fac[j]][j] + 1;
  for (unsigned int i = 2; i <= n; ++i) {
    fact[i] = fact[i - 1] + lg[i];
    flg[i] = (fact[i] / fexp).min();
  }
}
void prepare(const unsigned int n, const unsigned int m)
{
  for (unsigned int j = 1; j <= m; ++j)
    for (unsigned int i = 1; i <= n; ++i) {
      mem[j][i] = mem[j - 1][i] + mem[j][i - 1] - mem[j - 1][i - 1];
      if (i >= j && flg[i] &&
          ((fact[i] - fact[i - j] - fact[j]) / fexp).min() > 0)
        ++mem[j][i];
    }
}
int main()
{
#ifndef APTEST
  ios_base::sync_with_stdio(false);
#endif
  unsigned int t, k;
  cin >> t >> k;
  init(maxn, k);
  prepare(maxn, maxn);
  for (unsigned int i = 0; i < t; ++i) {
    unsigned int n, m;
    cin >> n >> m;
    cout << mem[m][n] << endl;
  }
  return 0;
}

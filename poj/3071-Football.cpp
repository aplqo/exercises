#ifdef APTEST
#include "debug_tools/program.h"
#endif
#include <algorithm>
#include <cstdio>
#include <cstdlib>
#include <iostream>
using namespace std;
const unsigned int maxn = 256;
typedef double real;

real p[maxn + 1][maxn + 1], f[maxn + 1][maxn + 1];

static void init(const unsigned int n, const unsigned int s)
{
  for (unsigned int i = 0; i < n; ++i)
    fill(f[i], f[i] + s, 0);
  for (unsigned int i = 0; i < s; ++i)
    f[0][i] = p[i][i ^ 1];
}
unsigned int dp(const unsigned int n, const unsigned int s)
{
  for (unsigned int i = 1; i < n; ++i)
    for (unsigned int j = 0; j < s; ++j) {
      const unsigned int msk = 1 << i, bs = (j & ~(msk - 1)) ^ msk;
      for (unsigned int k = bs; k < bs + msk; ++k)
        f[i][j] += f[i - 1][k] * p[j][k];
      f[i][j] *= f[i - 1][j];
    }
  return max_element(f[n - 1], f[n - 1] + s) + 1 - f[n - 1];
}
void solve()
{
  int n;
  cin >> n;
  if (n < 1) exit(0);
  const unsigned int s = 1 << n;
  for (unsigned int i = 0; i < s; ++i)
    for (unsigned int j = 0; j < s; ++j)
      cin >> p[i][j];
  init(n, s);
  cout << dp(n, s) << endl;
}
int main()
{
#ifndef APTEST
  ios_base::sync_with_stdio(false);
#endif
  while (true)
    solve();
  return 0;
}
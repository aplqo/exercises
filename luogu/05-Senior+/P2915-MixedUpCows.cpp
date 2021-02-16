#ifdef APTEST
#include "debug_tools/program.h"
#endif
#include <algorithm>
#include <cmath>
#include <iostream>
#include <iterator>
using namespace std;
#define lowbit(x) ((x) & -(x))
const unsigned int maxn = 16, maxs = (1 << maxn);

unsigned long long f[maxn + 1][maxs + 1], lg[maxs + 1];
int val[maxn + 1];

void init(const unsigned int n)
{
  for (unsigned int i = 0; i < n; ++i)
    f[i][1 << i] = 1;
  for (unsigned int i = 0; i < n; ++i)
    lg[1 << i] = i;
}
unsigned long long dp(const unsigned int n, const int k)
{
  const unsigned int msk = (1 << n) - 1;
  for (unsigned int i = 0; i < (1 << n); ++i)
    for (unsigned int j = i; j; j -= lowbit(j)) {
      const unsigned int cj = lg[lowbit(j)];
      if (!f[cj][i]) continue;
      for (unsigned int v = (~i) & msk; v; v -= lowbit(v)) {
        const unsigned int pk = lowbit(v), ck = lg[pk];
        if (abs(val[cj] - val[ck]) > k) f[ck][i | pk] += f[cj][i];
      }
    }
  unsigned long long ret = 0;
  for (unsigned int i = 0; i < n; ++i)
    ret += f[i][msk];
  return ret;
}

int main()
{
#ifndef APTEST
  ios_base::sync_with_stdio(false);
#endif
  unsigned int n;
  int k;
  cin >> n >> k;
  copy_n(istream_iterator<int>(cin), n, val);
  init(n);
  cout << dp(n, k) << endl;
  return 0;
}
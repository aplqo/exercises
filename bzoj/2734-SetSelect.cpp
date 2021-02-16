#ifdef APTEST
#include "debug_tools/program.h"
#endif
#include <cstring>
#include <iostream>
using namespace std;
const unsigned int maxn = 100000;
const unsigned int maxl2 = 17, maxl3 = 11, maxs = (1 << maxl3);
constexpr unsigned long long mod = 1000000001;

bool vis[maxn + 1];
unsigned int valid[maxs + 1], *vcur = valid, len[maxl2 + 1];

static void init()
{
  const unsigned int msk = (1 << maxl3) - 1;
  for (unsigned int i = 0; i <= msk; ++i)
    if (!(i & (i << 1)) && !(i & (i >> 1))) *(vcur++) = i;
}
unsigned int setup(const unsigned int x, const unsigned int n)
{
  unsigned int cnt = 0;
  for (unsigned int t = x; t <= n; t *= 2, ++cnt) {
    len[cnt] = 0;
    for (unsigned int j = t; j <= n; j *= 3) {
      vis[j] = true;
      ++len[cnt];
    }
  }
  return cnt;
}
unsigned long long dp(const unsigned int r)
{
  static unsigned long long f[maxl2 + 1][maxs];
  memset(f, 0, sizeof(f));
  for (unsigned int* i = valid; i < vcur && *i < (1 << len[0]); ++i)
    f[0][*i] = 1;
  for (unsigned int i = 1; i < r; ++i)
    for (unsigned int* s = valid; s < vcur && *s < (1 << len[i]); ++s)
      for (unsigned int* l = valid; l < vcur && *l < (1 << len[i - 1]); ++l)
        if (!(*s & *l)) f[i][*s] = (f[i][*s] + f[i - 1][*l]) % mod;
  unsigned long long ret = 0;
  for (unsigned int* i = valid; i < vcur && *i < (1 << len[r - 1]); ++i)
    ret = (ret + f[r - 1][*i]) % mod;
  return ret;
}
int main()
{
#ifndef APTEST
  ios_base::sync_with_stdio(false);
#endif
  unsigned int n;
  unsigned long long ans = 1;
  cin >> n;
  init();
  for (unsigned int i = 1; i <= n; ++i)
    if (!vis[i]) ans = (ans * dp(setup(i, n))) % mod;
  cout << ans << endl;
  return 0;
}
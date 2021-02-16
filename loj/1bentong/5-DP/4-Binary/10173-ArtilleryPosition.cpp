#ifdef APTEST
#include "debug_tools/program.h"
#endif
#include <algorithm>
#include <iostream>
using namespace std;
#define lowbit(x) ((x) & -(x))
const unsigned int maxn = 100, maxm = 10;
constexpr unsigned int maxs = 500;

unsigned int valid[maxs + 1], cnt[maxs + 1], cur;
unsigned int f[maxn + 1][maxs + 1][maxs + 1];
unsigned int dat[maxn + 1];

inline unsigned int popcnt(unsigned int x)
{
  unsigned int ret = 0;
  for (; x; x -= lowbit(x))
    ++ret;
  return ret;
}
void init(const unsigned int m)
{
#define test(v, l, op) (((v)op(l)) & (v))
  for (unsigned int i = 0; i < (1 << m); ++i)
    if (!(test(i, 1, <<) || test(i, 2, <<) || test(i, 1, >>) ||
          test(i, 2, >>))) {
      cnt[cur] = popcnt(i);
      valid[cur++] = i;
    }
#undef test
}
unsigned int dp(const unsigned int n)
{
#define failD(s, l) (valid[s] & dat[l])
#define failV(l1, l2) (valid[l1] & valid[l2])
  for (unsigned int i = 0; i < cur; ++i) {
    if (failD(i, 0)) continue;
    for (unsigned int j = 0; j < cur; ++j)
      if (!failV(i, j) && !failD(j, 1))
        f[1][i][j] = max(f[1][i][j], cnt[i] + cnt[j]);
  }
  for (unsigned int j = 2; j < n; ++j)
    for (unsigned int i1 = 0; i1 < cur; ++i1) {
      if (failD(i1, j)) continue;
      for (unsigned int i2 = 0; i2 < cur; ++i2) {
        if (failD(i2, j - 1) || failV(i1, i2)) continue;
        for (unsigned int l = 0; l < cur; ++l)
          if (!failD(l, j - 2) && !failV(i1, l) && !failV(i2, l))
            f[j][i2][i1] = max(f[j][i2][i1], f[j - 1][l][i2] + cnt[i1]);
      }
    }
  unsigned int ret = 0;
  for (unsigned int i = 0; i < cur; ++i)
    ret = max(ret, *max_element(f[n - 1][i], f[n - 1][i] + cur));
  return ret;
}

int main()
{
#ifndef APTEST
  ios_base::sync_with_stdio(false);
#endif
  unsigned int n, m;
  cin >> n >> m;
  for (unsigned int i = 0; i < n; ++i) {
    static char buf[maxm + 10];
    cin >> buf;
    for (unsigned int j = 0; j < m; ++j)
      dat[i] = (dat[i] << 1) + (buf[j] == 'H');
  }
  init(m);
  cout << dp(n) << endl;
  return 0;
}
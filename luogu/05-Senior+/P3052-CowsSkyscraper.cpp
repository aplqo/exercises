#ifdef APTEST
#include "debug_tools/program.h"
#endif
#include <algorithm>
#include <iostream>
#include <iterator>
using namespace std;
#define lowbit(x) ((x) & -(x))
const unsigned int maxn = 18, maxs = (1 << maxn);

unsigned int w[maxn];
unsigned int lg[maxs + 1];

void getLog(const unsigned int n)
{
  for (unsigned int i = 0; i < n; ++i)
    lg[1 << i] = i;
}
unsigned int dp(const unsigned int n, const unsigned int siz)
{
  static unsigned int f[maxs + 1], rem[maxs + 1];
  const unsigned int msk = (1 << n) - 1;
  for (unsigned int i = 0; i <= msk; ++i) {
    const auto upd = [&i](const unsigned int v, const unsigned int s) {
      if (v > f[i] || (v == f[i] && s > rem[i])) {
        f[i] = v;
        rem[i] = s;
      }
    };
    for (unsigned int j = i; j; j -= lowbit(j)) {
      const unsigned int t = lowbit(j), b = lg[t];
      const unsigned int s = i - t;
      if (rem[s] >= w[b])
        upd(f[s], rem[s] - w[b]);
      else
        upd(f[s] + 1, siz - w[b]);
    }
  }
  return f[msk];
}
int main()
{
#ifndef APTEST
  ios_base::sync_with_stdio(false);
#endif
  unsigned int n, siz;
  cin >> n >> siz;
  copy_n(istream_iterator<unsigned int>(cin), n, w);
  getLog(n);
  cout << dp(n, siz) << endl;
  return 0;
}
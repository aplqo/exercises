#ifdef APTEST
#include "debug_tools/program.h"
#endif
#include <algorithm>
#include <iostream>
#include <iterator>
using namespace std;
const unsigned int maxn = 2000, maxm = 2000;

unsigned int w[maxn + 1], m;
unsigned int f[maxm + 1] = {1}, cnt[maxn + 1][maxm + 1];

void addObject(const unsigned int l, const unsigned int r)
{
  for (unsigned int i = l; i < r; ++i)
    for (unsigned int j = m; j > 0; --j)
      if (j >= w[i]) f[j] = (f[j] + f[j - w[i]]) % 10;
}
void dfs(const unsigned int l, const unsigned int r)
{
  static unsigned int tmp[16][maxm + 1], dep;
  if (r - l == 1) {
    copy(f, f + m + 1, cnt[l]);
    return;
  }
  ++dep;
  const unsigned int mid = (l + r) >> 1;
  copy(f, f + m + 1, tmp[dep]);
  addObject(mid, r);
  dfs(l, mid);

  copy(tmp[dep], tmp[dep] + m + 1, f);
  addObject(l, mid);
  dfs(mid, r);
  --dep;
}
int main()
{
#ifndef APTEST
  ios_base::sync_with_stdio(false);
#endif
  unsigned int n;
  cin >> n >> m;
  copy_n(istream_iterator<unsigned int>(cin), n, w);
  dfs(0, n);
  for (unsigned int i = 0; i < n; ++i) {
    copy(cnt[i] + 1, cnt[i] + m + 1, ostream_iterator<unsigned int>(cout));
    cout << endl;
  }
  return 0;
}

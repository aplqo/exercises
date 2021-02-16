#ifdef APTEST
#include "debug_tools/program.h"
#endif
#include <algorithm>
#include <functional>
#include <iostream>
using namespace std;
const unsigned int maxn = 1000 * 8, maxe = 1000;

unsigned int a[maxn + 1], b[maxn + 1];
unsigned int pos[maxn + 1], cur;
unsigned long long f[maxn + 1][maxe + 1], g[maxn + 1][maxe + 1];

template <class T>
void dp(const int beg, const int n, unsigned long long out[maxn + 1][maxe + 1])
{
  T nxt;
  for (int i = beg; i > 0 && i <= n; i = nxt(i, 1)) {
    for (unsigned int j = 1; j <= maxe; ++j) {
      out[i][j] = out[nxt(i, -1)][j];
      if (b[i] <= j)
        out[i][j] = max(out[i][j], out[nxt(i, -1)][j - b[i]] + a[i]);
    }
  }
}
static void init(const unsigned int n)
{
  dp<plus<int>>(1, n, f);
  dp<minus<int>>(n, n, g);
}
unsigned long long query(const unsigned int d, const unsigned int e)
{
  unsigned long long ret = 0;
  for (unsigned int i = 0; i <= e; ++i)
    ret = max(ret, f[pos[d] - 1][i] + g[pos[d + 1]][e - i]);
  return ret;
}
void addObject(const unsigned int p, const unsigned int pa,
               const unsigned int pb, unsigned int c)
{
  pos[p] = cur + 1;
  for (unsigned int i = 1; c >= i; i <<= 1) {
    ++cur;
    a[cur] = pa * i;
    b[cur] = pb * i;
    c -= i;
  }
  if (c) {
    ++cur;
    a[cur] = pa * c;
    b[cur] = pb * c;
  }
}

int main()
{
#ifndef APTEST
  ios_base::sync_with_stdio(false);
#endif
  unsigned int n, q;
  cin >> n;
  for (unsigned int i = 1; i <= n; ++i) {
    unsigned int a, b, c;
    cin >> b >> a >> c;
    addObject(i, a, b, c);
  }
  init(cur);
  cin >> q;
  for (unsigned int i = 0; i < q; ++i) {
    unsigned int d, e;
    cin >> d >> e;
    cout << query(d + 1, e) << endl;
  }
  return 0;
}

#ifdef APTEST
#include "debug_tools/program.h"
#endif
#include <algorithm>
#include <cstdio>
#include <iostream>
#include <iterator>
#include <limits>
#include <numeric>
using namespace std;
const unsigned int maxn = 100;
constexpr long double ninf = numeric_limits<long double>::lowest(), eps = 1e-3;

struct edge {
  unsigned int to;
  const edge* pre;
} ed[maxn * 2 + 1];
const edge* head[maxn + 1];
long long a[maxn + 1], b[maxn + 1];
long double f[maxn + 1][maxn + 1], tmp[maxn + 1][maxn + 1];
unsigned int num;

void addEdge(const unsigned int from, const unsigned int to)
{
  static edge* cur = ed;
  cur->to = to;
  cur->pre = head[from];
  head[from] = cur;
  ++cur;
}
void treeDp(const unsigned int x, const unsigned int fa, const double mid)
{
  for (const edge* i = head[x]; i; i = i->pre)
    if (i->to != fa) treeDp(i->to, x, mid);
  fill(tmp[0] + 1, tmp[0] + num + 1, ninf);
  unsigned int cnt = 0;
  for (const edge* i = head[x]; i; i = i->pre)
    if (i->to != fa) {
      ++cnt;
      for (unsigned int j = 1; j <= num; ++j) {
        tmp[cnt][j] = tmp[cnt - 1][j];
        for (unsigned int c = 0; c <= j; ++c)
          tmp[cnt][j] = max(tmp[cnt - 1][c] + f[i->to][j - c], tmp[cnt][j]);
      }
    }
  for (unsigned int i = 1; i <= num; ++i)
    f[x][i] = tmp[cnt][i - 1] + a[x] - mid * b[x];
}
bool test(const unsigned int n, const long double v)
{
  treeDp(1, 0, v);
  long double ret = ninf;
  for (unsigned int i = 1; i <= n; ++i)
    ret = max(ret, f[i][num]);
  return ret >= 0;
}
long double bfind(const unsigned int n)
{
  long double l = 0, r = accumulate(a + 1, a + 1 + n, 0ll);
  while (r - l >= eps) {
    const long double mid = (l + r) / 2;
    if (test(n, mid))
      l = mid + 0.01;
    else
      r = mid - 0.01;
  }
  return l;
}
int main()
{
  unsigned int n, m;
  cin >> n >> m;
  num = n - m;
  copy_n(istream_iterator<long long>(cin), n, a + 1);
  copy_n(istream_iterator<long long>(cin), n, b + 1);
  for (unsigned int i = 1; i < n; ++i) {
    unsigned int a, b;
    cin >> a >> b;
    addEdge(a, b);
    addEdge(b, a);
  }
  printf("%.1Lf\n", bfind(n));
  return 0;
}
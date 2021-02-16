#ifdef APTEST
#include "debug_tools/program.h"
#endif
#include <algorithm>
#include <climits>
#include <iostream>
#include <numeric>
using namespace std;
const unsigned int maxn = 1e5, maxl = 17;
constexpr unsigned int inf = UINT_MAX / 2;

class STTable {
 public:
  void build(const unsigned int n, const long long v[]);
  long long query(const unsigned int l, const unsigned int r) const;
  static void initLog(const unsigned int n);

 private:
  static unsigned int lg[maxn + 10];
  long long tab[maxn + 1][maxl + 1];
} tab;
unsigned int STTable::lg[maxn + 10];
struct Pill {
  long long a, b;
} p[maxn + 10];
long long h[maxn + 1], c[maxn + 1];

void STTable::build(const unsigned int n, const long long v[])
{
  for (unsigned int i = 1; i <= n; ++i)
    tab[i][0] = v[i];
  for (unsigned int i = 1; i <= lg[n]; ++i)
    for (unsigned int l = 1; l + (1 << i) - 1 <= n; ++l)
      tab[l][i] = min(tab[l][i - 1], tab[l + (1 << (i - 1))][i - 1]);
}
long long STTable::query(const unsigned int l, const unsigned int r) const
{
  const unsigned int d = lg[r + 1 - l];
  return min(tab[l][d], tab[r + 1 - (1 << d)][d]);
}
void STTable::initLog(const unsigned int n)
{
  for (unsigned int i = 2; i <= n; ++i)
    lg[i] = lg[i >> 1] + 1;
}
static unsigned int init(const unsigned int n)
{
  static long long tmp[maxn + 1];
  sort(p + 1, p + 1 + n,
       [](const Pill a, const Pill b) { return a.a - a.b > b.a - b.b; });
  unsigned int e = n + 1;
  for (unsigned int i = 1; i <= n; ++i) {
    h[i] = h[i - 1] + p[i].a - p[i].b;
    c[i] += c[i - 1];
    tmp[i] = h[i] - c[i - 1];
    if (h[i] < h[i - 1] || h[i] <= c[i]) {
      e = i;
      break;
    }
  }
  tab.initLog(n);
  tab.build(n, tmp);
  return e;
}
unsigned int binaryFind(const unsigned int e, const unsigned int beg,
                        const long long x, const long long lim)
{
  const auto test = [beg, x, lim](unsigned int m) {
    return h[m] >= x & tab.query(beg + 1, m) > lim;
  };
  unsigned int l = beg + 1, r = e - 1;
  if (e <= beg || !test(r)) return inf;
  while (l <= r) {
    const unsigned int m = (l + r) >> 1;
    if (test(m))
      r = m - 1;
    else
      l = m + 1;
  }
  return r >= e ? inf : r;
}
unsigned int solve(const unsigned int e, const long long l)
{
  unsigned int ret = inf;
  for (unsigned int i = 1; i <= e; ++i) {
    if (h[i - 1] + p[i].a >= l)
      ret = min<unsigned int>(ret, lower_bound(h, h + i, l - p[i].a) - h + 1);
    else
      ret = min(ret, binaryFind(e, i, l - p[i].b, p[i].a - p[i].b) + 1);
  }
  return ret;
}

int main()
{
#ifndef APTEST
  ios_base::sync_with_stdio(false);
#endif
  unsigned int n;
  long long l;
  cin >> n >> l;
  for (Pill* i = p + 1; i < p + 1 + n; ++i)
    cin >> i->a >> i->b;
  for (long long* i = c + 1; i < c + 1 + n; ++i)
    cin >> *i;
  const unsigned int res = solve(init(n), l);
  if (res == inf)
    cout << "-1\n";
  else
    cout << res << "\n";
  return 0;
}
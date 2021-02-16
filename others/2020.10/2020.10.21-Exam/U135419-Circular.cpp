#ifdef APTEST
#include "debug_tools/program.h"
#endif
#include <algorithm>
#include <climits>
#include <cmath>
#include <iostream>
#include <utility>
using namespace std;
const unsigned int maxn = 100000, maxl = 20;
constexpr unsigned long long inf = ULLONG_MAX;

struct Segment {
  unsigned long long l, r;
} seg[maxn * 2 + 10];
unsigned int nxt[maxn * 2 + 10][maxl + 1];

static void expand(const unsigned int n, const unsigned int m)
{
  for (Segment* i = seg; i < seg + n; ++i)
    if (i->r < i->l) i->r += m;
  Segment* cur = seg + n;
  for (const Segment* i = seg; i < seg + n; ++i) {
    cur->l = i->l + m;
    cur->r = i->r + m;
    ++cur;
  }
}
static unsigned int discretize(const unsigned int n)
{
  static unsigned long long tmp[maxn * 4 + 10];
  unsigned long long* ptr = tmp;
  for (const Segment* i = seg; i < seg + n; ++i) {
    *(ptr++) = i->l;
    *(ptr++) = i->r;
  }
  sort(tmp, ptr);
  ptr = unique(tmp, ptr);
  for (Segment* i = seg; i < seg + n; ++i) {
    i->l = lower_bound(tmp, ptr, i->l) - tmp;
    i->r = lower_bound(tmp, ptr, i->r) - tmp;
  }
  return ptr - tmp;
}
static void initNxt(const unsigned int n, const unsigned int vcnt,
                    const unsigned int lg)
{
  static unsigned int suf[maxn * 4 + 10];
  const auto pred = [](const unsigned int a, const unsigned int b) {
    return seg[a].r < seg[b].r;
  };
  fill(
      reinterpret_cast<unsigned int*>(nxt),
      reinterpret_cast<unsigned int*>(nxt) + sizeof(nxt) / sizeof(unsigned int),
      n);
  fill(suf, suf + vcnt + 2, n);
  seg[n].r = inf;
  for (unsigned int i = 0; i < n; ++i) {
    const unsigned int pl = seg[i].l;
    suf[pl] = min(suf[pl], i, pred);
  }
  for (int i = vcnt; i >= 0; --i)
    suf[i] = min(suf[i], suf[i + 1], pred);
  for (unsigned int i = 0; i < n; ++i)
    nxt[i][0] = suf[seg[i].r];
  for (unsigned int i = 1; i <= lg; ++i)
    for (unsigned int j = 0; j < n; ++j)
      nxt[j][i] = nxt[nxt[j][i - 1]][i - 1];
}
unsigned int binarySearch(unsigned int pos, const unsigned int ex,
                          const unsigned int lg)
{
  unsigned int ret = 0;
  for (unsigned int i = lg; i;) {
    const unsigned int m = i - 1;
    if (seg[nxt[pos][m]].r < ex) {
      pos = nxt[pos][m];
      ret |= 1 << m;
    }
    else
      i = m;
  }
  return ret;
}

int main()
{
#ifndef APTEST
  ios_base::sync_with_stdio(false);
#endif
  unsigned int n, m;
  cin >> m >> n;
  for (Segment* i = seg; i < seg + n; ++i)
    cin >> i->l >> i->r;
  const unsigned int lg = log2(n) + 1;
  expand(n, m);
  initNxt(n * 2, discretize(n * 2), lg);
  unsigned int ret = 0;
  for (unsigned int i = 0; i < n; ++i)
    ret = max(ret, binarySearch(i, seg[i + n].l, lg) + 1);
  cout << ret << "\n";
  return 0;
}
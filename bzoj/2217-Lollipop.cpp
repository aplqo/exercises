#ifdef APTEST
#include "debug_tools/program.h"
#endif
#include <algorithm>
#include <functional>
#include <iostream>
#include <iterator>
#include <numeric>
#include <utility>
using namespace std;
const unsigned int maxn = 1000000;

struct Range {
  unsigned int l, r;
} rg[maxn * 2 + 10];
unsigned int a[maxn + 10];

template <unsigned int part>
pair<unsigned int, Range> maxRange(unsigned int s, const unsigned int n)
{
  typedef reverse_iterator<const unsigned int*> riter;
  if ((s & 0x01) == part) return make_pair(s, Range{1u, n});
  const unsigned int p1 = find(a + 1, a + 1 + n, 1) - a;
  const unsigned int p2 = find(riter(a + n), riter(a), 1) - riter(a + n) + 1;
  if (p1 - 1 < p2)
    return make_pair(s - (p1 - 1) * 2 - 1, Range{p1 + 1, n});
  else
    return make_pair(s - p2 * 2 - 1, Range{1u, n - p2 - 1});
}
void moveRange(const unsigned int s)
{
  for (Range* i = rg + s; i > rg + 1; --i) {
    if (!i->l) continue;
    if (a[i->l] == 2)
      *(i - 2) = Range{i->l + 1, i->r};
    else if (a[i->r] == 2)
      *(i - 2) = Range{i->l, i->r - 1};
    else
      *(i - 2) = Range{i->l + 1, i->r - 1};
  }
}
static void init(const unsigned int n)
{
  const unsigned int s = accumulate(a, a + 1 + n, 0u);
  const auto v1 = maxRange<1>(s, n);
  if (v1.second.l <= v1.second.r) rg[v1.first] = v1.second;
  const auto v2 = maxRange<0>(s, n);
  if (v2.second.l <= v2.second.r) rg[v2.first] = v2.second;
  moveRange(s);
  rg[0] = Range{};
}

int main()
{
#ifndef APTEST
  ios_base::sync_with_stdio(false);
#endif
  unsigned int n, m;
  cin >> n >> m;
  {
    static char str[maxn + 10];
    cin >> (str + 1);
    transform(str + 1, str + 1 + n, a + 1,
              [](char c) { return c == 'T' ? 2 : 1; });
  }
  init(n);
  for (unsigned int i = 0; i < m; ++i) {
    unsigned int k;
    cin >> k;
    if (rg[k].l)
      cout << rg[k].l << " " << rg[k].r << "\n";
    else
      cout << "NIE\n";
  }
  return 0;
}
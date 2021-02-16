#ifdef APTEST
#include "debug_tools/program.h"
#endif
#include <algorithm>
#include <iomanip>
#include <iostream>
using namespace std;
const unsigned int maxn = 3e5;
typedef double real;

struct Range {
  int l, r;
} rng[maxn + 10];

unsigned int solve(const unsigned int n)
{
  sort(rng + 1, rng + n + 1, [](const Range& a, const Range& b) {
    return a.l != b.l ? a.l < b.l : a.r < b.r;
  });
  int ret = rng[1].r - rng[1].l;
  for (Range* i = rng + 2; i < rng + n + 1; ++i) {
    if (i->l < (i - 1)->r) {
      i->r = max(i->r, (i - 1)->r);
      i->l = min(i->l, (i - 1)->l);
    }
    ret = max(ret, i->r - i->l);
  }
  return ret;
}
int main()
{
#ifndef APTEST
  ios_base::sync_with_stdio(false);
#endif
  unsigned int n;
  cin >> n;
  for (Range* i = rng; i < rng + n; ++i) {
    int a, p;
    cin >> p >> a;
    i->l = p - a;
    i->r = p + a;
    if (p < a) {
      cout << "-1\n";
      return 0;
    }
  }
  cout << fixed << setprecision(1) << solve(n) / 2.0 << "\n";
  return 0;
}
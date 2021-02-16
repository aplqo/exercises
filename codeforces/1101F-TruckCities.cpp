#ifdef APTEST
#include "debug_tools/program.h"
#endif
#include <algorithm>
#include <iostream>
#include <iterator>
using namespace std;
const unsigned int maxn = 400, maxm = 250000;

struct Truck {
  unsigned int s, t, r;
  unsigned long long c;
} tk[maxm + 1];
unsigned int a[maxn + 1];

unsigned long long dp(const unsigned int n, const unsigned int m)
{
  static unsigned int f[maxn + 1][maxn + 1];
  static const auto val = [](const unsigned int lst, const unsigned int k,
                             const unsigned int r) {
    return max(f[lst][k - 1], a[r] - a[lst]);
  };
  const Truck* pt = tk;
  unsigned long long ret = 0;
  for (unsigned int l = 1; l <= n; ++l) {
    if (pt >= tk + m || pt->s != l) continue;
    for (unsigned int i = l; i <= n; ++i)
      f[i][0] = a[i] - a[l];
    for (unsigned int k = 1; k <= n; ++k) {
      f[l][k] = 0;
      unsigned int ptr = l;
      for (unsigned int r = l + 1; r <= n; ++r) {
        f[r][k] = val(ptr, k, r);
        while (ptr + 1 < r && val(ptr + 1, k, r) < f[r][k])
          f[r][k] = val(++ptr, k, r);
      }
    }

    while (pt < tk + m && pt->s == l) {
      ret = max(ret, pt->c * f[pt->t][pt->r]);
      ++pt;
    }
  }
  return ret;
}
int main()
{
#ifndef APTEST
  ios_base::sync_with_stdio(false);
#endif
  unsigned int n, m;
  cin >> n >> m;
  copy_n(istream_iterator<unsigned int>(cin), n, a + 1);
  for (Truck* i = tk; i < tk + m; ++i)
    cin >> i->s >> i->t >> i->c >> i->r;
  sort(tk, tk + m, [](const Truck& a, const Truck& b) { return a.s < b.s; });
  cout << dp(n, m) << "\n";
  return 0;
}
#ifdef APTEST
#include "debug_tools/program.h"
#endif
#include <algorithm>
#include <cmath>
#include <iterator>
#include <numeric>
using namespace std;
const unsigned int maxn = 1e6, maxl = 20;
using num = long long;

num a[maxn + 1];
long double q[maxn + 1];

long double dfs(const unsigned int l, const unsigned int r,
                const unsigned int d = 0)
{
  if (r == l) {
    q[l] = 2 - static_cast<long double>(1) / (num(1) << d);
    return q[l] * a[l];
  }
  const unsigned int m = (l + r) >> 1;
  return dfs(l, m, d + 1) + dfs(m + 1, r, d + 1);
}
int main()
{
  unsigned int n, m;
  num qwq;
  scanf("%u%u%Ld", &n, &m, &qwq);
  for_each(a + 1, a + 1 + n, [](num& i) { scanf("%Ld", &i); });
  if (!m) return 0;
  long double e = dfs(1, n);
  partial_sum(q + 1, q + n + 1, q + 1);
  for (unsigned int i = 0; i < m; ++i) {
    unsigned int l, r;
    num x;
    scanf("%u%u%Ld", &l, &r, &x);
    e += (q[r] - q[l - 1]) * x;
    printf("%Ld\n", static_cast<unsigned long long>(e * qwq));
  }
  return 0;
}
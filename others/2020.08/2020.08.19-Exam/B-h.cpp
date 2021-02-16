#ifdef APTEST
#include "debug_tools/program.h"
#endif
#include <algorithm>
#include <cstdio>
using namespace std;
const unsigned int maxn = 5e4;
using real = double;
constexpr real mr = 2e7, eps = 1e-5;

struct Sample {
  int p, v;
} sam[maxn + 1];
unsigned int n, k;

bool test(const real mid)
{
  static real stk[maxn + 10];
  real* top = stk;
  for (unsigned int i = 0; i < n; ++i) {
    const real nv = sam[i].p + mid * sam[i].v;
    real* const p = upper_bound(stk, top, nv);
    if (p == top)
      *(top++) = nv;
    else
      *p = nv;
  }
  return n - (top - stk) <= k;
}
real binary()
{
  real l = 0, r = mr + 10;
  while (r - l >= eps) {
    const real mid = (l + r) / 2;
    if (test(mid))
      l = mid;
    else
      r = mid;
  }
  return r;
}

int main()
{
  scanf("%d%d", &n, &k);
  for (unsigned int i = 0; i < n; ++i)
    scanf("%d%d", &sam[i].p, &sam[i].v);
  sort(sam, sam + n,
       [](const Sample& a, const Sample& b) { return a.p < b.p; });
  const real t = binary();
  if (t > mr)
    printf("Forever\n");
  else
    printf("%.4lf\n", t);
  return 0;
}
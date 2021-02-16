#ifdef APTEST
#include "debug_tools/program.h"
#endif
#include <algorithm>
#include <functional>
#include <iostream>
#include <iterator>
#include <tuple>
#include <type_traits>
#include <utility>
using namespace std;
const unsigned int maxn = 1e5, maxl = 18;

namespace RailwayTrip {
unsigned int l[maxn + 10];
unsigned int left[maxn + 10][maxl + 10], right[maxn + 10][maxl + 10];

template <bool rev>
static void initLR(const unsigned int n, unsigned int res[][maxl + 10])
{
  static unsigned int stk[maxn + 10];
  conditional_t<rev, minus<unsigned int>, plus<unsigned int>> nxt;
  unsigned int* top = stk;
  for (unsigned int i = rev ? n : 1; i && i <= n; i = nxt(i, 1)) {
    while (top > stk && l[*(top - 1)] < l[i])
      --top;
    if (top > stk)
      res[i][0] = *(top - 1);
    else
      res[i][0] = i;
    *(top++) = i;
  }
}
static void initRange(const unsigned int n)
{
  initLR<false>(n, left);
  initLR<true>(n, right);
  for (unsigned int i = 1; i <= maxl; ++i)
    for (unsigned int j = 1; j <= n; ++j) {
      left[j][i] =
          min(left[left[j][i - 1]][i - 1], left[right[j][i - 1]][i - 1]);
      right[j][i] =
          max(right[left[j][i - 1]][i - 1], right[right[j][i - 1]][i - 1]);
    }
}
tuple<unsigned int, unsigned int, unsigned int> containRange(
    const unsigned int pos, const unsigned int cov)
{
  unsigned int l = pos, r = pos;
  unsigned int ret = 0;
  for (unsigned int i = maxl; i;) {
    const unsigned int mid = i - 1;
    const unsigned int cl = min(left[l][mid], left[r][mid]),
                       cr = max(right[l][mid], right[r][mid]);
    if (cov < cl || cr < cov) {
      ret |= 1 << mid;
      l = cl;
      r = cr;
    }
    else
      i = mid;
  }
  return make_tuple(ret, l, r);
}
}  // namespace RailwayTrip

int main()
{
#ifndef APTEST
  ios_base::sync_with_stdio(false);
#endif
  using namespace RailwayTrip;
  unsigned int n, k, q;
  cin >> n >> k >> q;
  copy_n(istream_iterator<unsigned int>(cin), n, l + 1);
  initRange(n);
  for (unsigned int i = 0; i < q; ++i) {
    unsigned int a, b;
    cin >> a >> b;
    if (a > b) swap(a, b);
    const auto [v, l, r] = containRange(a, b);
    cout << v + get<0>(containRange(b, r)) << "\n";
  }
  return 0;
}
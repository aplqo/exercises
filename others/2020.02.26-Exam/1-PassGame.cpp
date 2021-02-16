#ifdef APTEST
#include "debug_tools/program.h"
#endif
#include <algorithm>
#include <climits>
#include <cstdint>
#include <iostream>
using namespace std;
using res_t = uint_fast16_t;
#define lowbit(x) ((x) & -(x))
const int maxn = 16;
constexpr res_t maxs = (res_t(1) << maxn);
constexpr int inf = INT_MAX;

int dis[maxn + 10][maxn + 10], f[maxs + 10][maxn + 10];
unsigned int lg[maxs + 10];

inline void init(const unsigned int n)
{
  res_t t = 1;
  for (unsigned int i = 0; i < n; ++i, t <<= 1) {
    fill(f[t], f[t] + n, 0);
    lg[t] = i;
  }
}
int main()
{
#ifndef APTEST
  ios_base::sync_with_stdio(false);
#endif
  unsigned int n;
  cin >> n;
  for (unsigned i = 0; i < n; ++i)
    for (unsigned int j = 0; j < n; ++j)
      cin >> dis[i][j];
  for_each(f + 1, f + (1 << n),
           [&n](int(&i)[maxn + 10]) { fill(i, i + n, inf); });
  init(n);

  for (res_t i = 1; i < res_t(1) << n; ++i) {
    for (res_t j = i; j; j -= lowbit(j)) {
      const unsigned int from = lg[lowbit(j)];
      for (res_t k = ~i & (1 << n) - 1; k; k -= lowbit(k)) {
        const unsigned int to = lg[lowbit(k)];
        f[i | lowbit(k)][to] =
            min(f[i | lowbit(k)][to], f[i][from] + dis[from][to]);
      }
    }
  }
  cout << *min_element(f[(1 << n) - 1], f[(1 << n) - 1] + n) << endl;
  return 0;
}

#ifdef APTEST
#include "debug_tools/judge.h"
#endif
#include <algorithm>
#include <iostream>
#include <iterator>
const int maxn = 1e5;

int a[maxn * 2 + 10];

int* findPartition(const int n, const int m)
{
  int *l = a, *r = a + n * 2 - 1;
  int pair = 0;
  while (l < r) {
    while (l < r && *l < m - *r)
      ++l;
    if (l < r && *l >= m - *r) {
      ++pair;
      ++l;
      --r;
    }
  }
  return a + (n - pair) * 2;
}
template <bool exceed>
int maxCost(const int* l, const int* r, const int m)
{
  int ret = 0;
  --r;
  for (; l < r; ++l, --r)
    ret = std::max(ret, exceed ? *l + *r - m : *l + *r);
  return ret;
}
int greedy(const int n, const int m)
{
  std::sort(a, a + n * 2);
  const int* p = findPartition(n, m);
  return std::max(maxCost<false>(a, p, m), maxCost<true>(p, a + n * 2, m));
}
int main()
{
  std::ios::sync_with_stdio(false);
  int n, m;
  std::cin >> n >> m;
  std::copy_n(std::istream_iterator<int>(std::cin), n * 2, a);
  std::cout << greedy(n, m) << "\n";
  return 0;
}
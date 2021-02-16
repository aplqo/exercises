#ifdef APTEST
#include "debug_tools/program.h"
#endif
#include <algorithm>
#include <iostream>
#include <numeric>
using namespace std;
using num_t = unsigned long long;
const int maxn = 1e5;

num_t sum[maxn + 10];
unsigned int n, m;

#if (Test_impl == 1)
inline bool test(const num_t val)
{
  unsigned int lst = 0;
  for (unsigned int i = 0; i < m; ++i) {
    unsigned int cur = lst + 1;
    for (; cur <= n && sum[cur] - sum[lst] < val; ++cur)
      ;
    if (cur > n) return false;
    lst = cur;
  }
  return true;
}
#else
inline bool test(const num_t val)
{
  num_t* lst = sum;
  for (unsigned int i = 0; i < m; ++i) {
    num_t* cur = lower_bound(lst + 1, sum + n + 1, *lst + val);
    if (cur == sum + n + 1) return false;
    lst = cur;
  }
  return true;
}
#endif

int main()
{
#ifndef APTEST
  ios_base::sync_with_stdio(false);
#endif
  cin >> n >> m;
  for (num_t* i = sum + 1; i < sum + n + 1; ++i)
    cin >> *i;
  partial_sum(sum + 1, sum + 1 + n, sum + 1);
  num_t l = 1, r = sum[n] / m;
  while (l <= r) {
    num_t m = (l + r) >> 1;
    if (test(m))
      l = m + 1;
    else
      r = m - 1;
  }
  cout << r << endl;
  return 0;
}
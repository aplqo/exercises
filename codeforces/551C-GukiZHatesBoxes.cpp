#ifdef APTEST
#include "debug_tools/program.h"
#endif
#include <algorithm>
#include <iostream>
#include <iterator>
#include <numeric>
using namespace std;
const unsigned int maxn = 100000;
const unsigned long long maxa = 1000000000;

unsigned int cnt[maxn + 10];

bool check(const unsigned int n, unsigned int m, const unsigned long long t)
{
  static unsigned int tmp[maxn + 10];
  copy(cnt, cnt + 1 + n, tmp);
  unsigned int* ptr = tmp + n;
  while (ptr >= tmp && !*(ptr))
    --ptr;
  for (unsigned int i = 0; i < m; ++i)
    for (unsigned long long acc = t - min<unsigned long long>(t, ptr - tmp);
         acc && ptr >= tmp;) {
      const unsigned long long delt = min<unsigned long long>(*ptr, acc);
      *ptr -= delt;
      acc -= delt;
      while (ptr >= tmp && !*ptr)
        --ptr;
    }
  return all_of(tmp + 1, tmp + 1 + n, [](unsigned int i) { return !i; });
}

unsigned long long binarySearch(const unsigned int n, const unsigned int m)
{
  long long l = 0, r = accumulate(cnt, cnt + 1 + n, 0ll) + n;
  while (l <= r) {
    const long long mid = (l + r) / 2;
    if (check(n, m, mid))
      r = mid - 1;
    else
      l = mid + 1;
  }
  return l;
}
int main()
{
#ifndef APTEST
  ios_base::sync_with_stdio(false);
#endif
  unsigned int n, m;
  cin >> n >> m;
  copy_n(istream_iterator<unsigned int>(cin), n, cnt + 1);
  cout << binarySearch(n, m) << "\n";
  return 0;
}
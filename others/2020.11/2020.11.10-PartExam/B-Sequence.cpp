#ifdef APTEST
#include "debug_tools/program.h"
#endif
#include <algorithm>
#include <cstring>
#include <iostream>
#include <iterator>
#include <numeric>
using namespace std;
const unsigned int maxn = 100000, maxt = maxn * 2;

unsigned int a[maxn + 10];

namespace FTree {
#define lowbit(x) ((x) & -(x))
int val[maxn * 2 + 10];

void change(unsigned int p, const unsigned int n, const int delt)
{
  for (; p <= n; p += lowbit(p))
    val[p] += delt;
}
unsigned int prefixSum(unsigned int p)
{
  int ret = 0;
  for (; p; p -= lowbit(p))
    ret += val[p];
  return ret;
}
unsigned int sum(const unsigned int l, const unsigned int r)
{
  return prefixSum(r) - prefixSum(l - 1);
}
void clear() { memset(val, 0, sizeof(val)); }

#undef lowbit
}  // namespace FTree

unsigned long long countMedian(
    const unsigned int n, const unsigned int k, const unsigned int l2,
    const unsigned int r2)  // [pl, pr), pl only in prefix sum
{
  static int sum[maxn + 10];
  FTree::clear();
  unsigned long long ret = 0;
  unsigned int lp = 0;
  for (unsigned int i = 1; i <= n; ++i)
    sum[i] = sum[i - 1] + (a[i] > k ? 1 : -1);
  for (unsigned int i = l2; i <= n; ++i) {
    if (i - lp > r2) FTree::change(sum[lp++] + n + 1, n * 2 + 5, -1);
    FTree::change(sum[i - l2] + n + 1, n * 2 + 5, 1);
    ret += FTree::sum(sum[i] + n + 1, n * 2 + 1);
  }
  return ret;
}

int main()
{
#ifndef APTEST
  ios_base::sync_with_stdio(false);
#endif
  unsigned int n, m;
  cin >> n;
  copy_n(istream_iterator<unsigned int>(cin), n, a + 1);
  cin >> m;
  for (unsigned int i = 0; i < m; ++i) {
    unsigned int l1, r1, l2, r2;
    cin >> l1 >> r1 >> l2 >> r2;
    cout << countMedian(n, r1, l2, r2) - countMedian(n, l1 - 1, l2, r2) << "\n";
  }
  return 0;
}
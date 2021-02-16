#ifdef APTEST
#include "debug_tools/program.h"
#endif
#include <algorithm>
#include <iostream>
#include <iterator>
using namespace std;
const unsigned int maxn = 40, maxs = (1 << (maxn / 2));

unsigned long long cost[maxn + 10];
unsigned long long plan[2][maxs + 10];
unsigned int lg[maxs + 10];

static void getLog(const unsigned int n)
{
  for (unsigned int i = 0; i <= n; ++i)
    lg[1 << i] = i;
}
#define lowbit(x) ((x) & -(x))
void enumerate(const unsigned long long* begin, const unsigned int n,
               unsigned long long res[])
{
  const unsigned int msk = 1u << n;
  for (unsigned int i = 1; i < msk; ++i) {
    const unsigned int lb = lowbit(i);
    res[i] = res[i ^ lb] + begin[lg[lb]];
  }
}
#undef lowbit

unsigned long long count(const unsigned int n, const unsigned long long lim)
{
  const unsigned int pl = n / 2, pr = n - pl;
  const unsigned int sizeL = 1u << pl, sizeR = 1u << pr;
  getLog(max(pl, pr));
  enumerate(cost, pl, plan[0]);
  enumerate(cost + pl, pr, plan[1]);
  sort(plan[0], plan[0] + sizeL);
  sort(plan[1], plan[1] + sizeR);
  const unsigned long long* ptr = plan[0] + sizeL - 1;
  unsigned long long ret = 0;
  for (const unsigned long long* i = plan[1]; i < plan[1] + sizeR; ++i) {
    while (ptr >= plan[0] && *ptr + (*i) > lim)
      --ptr;
    ret += ptr + 1 - plan[0];
  }
  return ret;
}
int main()
{
#ifndef APTEST
  ios_base::sync_with_stdio(false);
#endif
  unsigned int n;
  unsigned long long lim;
  cin >> n >> lim;
  copy_n(istream_iterator<unsigned long long>(cin), n, cost);
  cout << count(n, lim) << "\n";
  return 0;
}
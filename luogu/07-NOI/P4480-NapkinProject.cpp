#ifdef APTEST
#include "debug_tools/program.h"
#endif
#include <algorithm>
#include <cstring>
#include <deque>
#include <iostream>
#include <iterator>
#include <limits>
#include <numeric>
using namespace std;
const unsigned int maxd = 200000;
typedef unsigned long long Cost;
constexpr Cost inf = numeric_limits<Cost>::max();

Cost buyCost, slowCost, fastCost;
int slowTime, fastTime;
unsigned int need[maxd + 10], dirty[maxd + 10];

inline void move(const int before, deque<int>& dest, deque<int>& src)
{
  while (!src.empty() && src.front() <= before) {
    dest.push_back(src.front());
    src.pop_front();
  }
}
Cost use(unsigned int& lst, const Cost cst, deque<int>& rst)
{
  Cost ret = 0;
  while (lst && !rst.empty()) {
    const unsigned int from = rst.back(), tp = min(dirty[from], lst);
    lst -= tp;
    dirty[from] -= tp;
    ret += tp * cst;
    if (!dirty[from]) rst.pop_back();
  }
  return ret;
}
template <bool ignoreSlow>
Cost greedy(const int n, unsigned int buy)
{
  Cost ret = buy * buyCost;
  deque<int> near, far, notAvailable;
  for (int i = 0; i < n; ++i) {
    unsigned int lst = need[i];
    {
      const unsigned int tp = min<unsigned int>(lst, buy);
      lst -= tp;
      buy -= tp;
    }
    move(i - fastTime, near, notAvailable);
    if constexpr (!ignoreSlow) {
      move(i - slowTime, far, near);
      ret += use(lst, slowCost, far);
    }
    ret += use(lst, fastCost, near);
    if (lst) return inf;
    notAvailable.push_back(i);
    dirty[i] = need[i];
  }
  return ret;
}
Cost tenarySearch(const unsigned int n)
{
  const auto func = slowCost > fastCost ? greedy<true> : greedy<false>;
  unsigned int l = *min_element(need, need + n),
               r = accumulate(need, need + n, 0u);
  while (r - l > 2) {
    const unsigned int lm = l + (r - l) / 3, rm = l + 2 * (r - l) / 3;
    if (func(n, lm) < func(n, rm))
      r = rm;
    else
      l = lm;
  }
  return min({func(n, l), func(n, l + 1), func(n, l + 2)});
}

int main()
{
#ifndef APTEST
  ios::sync_with_stdio(false);
#endif
  unsigned int n;
  cin >> n;
  cin >> fastTime >> slowTime >> fastCost >> slowCost >> buyCost;
  if (slowTime < fastTime) {
    swap(slowTime, fastTime);
    swap(slowCost, fastCost);
  }
  copy_n(istream_iterator<unsigned int>(cin), n, need);
  cout << tenarySearch(n) << "\n";
  return 0;
}
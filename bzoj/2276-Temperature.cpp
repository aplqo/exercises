#ifdef APTEST
#include "debug_tools/program.h"
#endif
#include <algorithm>
#include <deque>
#include <iostream>
using namespace std;
const unsigned int maxn = 1e6;

struct Day {
  int l, r;
} d[maxn + 10];

unsigned int solve(const unsigned int n)
{
  unsigned int ret = 0;
  deque<Day*> q;
  for (Day* i = d; i < d + n; ++i) {
    while (!q.empty() && q.front()->l > i->r)
      q.pop_front();
    if (!q.empty()) ret = max<unsigned int>(ret, i + 1 - q.front());
    Day* lst = i;
    while (!q.empty() && q.back()->l < i->l) {
      lst = q.back();
      q.pop_back();
    }
    lst->l = i->l;
    q.push_back(lst);
  }
  return ret;
}
int main()
{
#ifndef APTEST
  ios_base::sync_with_stdio(false);
#endif
  unsigned int n;
  cin >> n;
  for (Day* i = d; i < d + n; ++i)
    cin >> i->l >> i->r;
  cout << solve(n) << "\n";
  return 0;
}

#ifdef APTEST
#include "debug_tools/program.h"
#endif
#include <algorithm>
#include <deque>
#include <functional>
#include <iostream>
#include <iterator>
#include <utility>
using namespace std;
const unsigned int maxn = 3e6;

unsigned long long a[maxn + 10];
template <template <typename> class Pred>
class MonoQueue {
 public:
  void push(const unsigned int pos, const unsigned long long val)
  {
    static Pred<unsigned long long> prd;
    while (!q.empty() && !prd(q.back().second, val))
      q.pop_back();
    q.emplace_back(pos, val);
  }
  void pop(const unsigned int pos)
  {
    while (q.front().first <= pos)
      q.pop_front();
  }
  unsigned long long front() const { return q.front().second; }

 private:
  deque<pair<unsigned int, unsigned long long>> q;
};

unsigned int solve(const unsigned int n, const unsigned long long t)
{
  MonoQueue<greater> gq;
  MonoQueue<less> lq;
  unsigned int pl = 1, ret = 1;
  for (unsigned int i = 1; i <= n; ++i) {
    gq.push(i, a[i]);
    lq.push(i, a[i]);
    while (gq.front() - lq.front() > t) {
      gq.pop(pl);
      lq.pop(pl);
      ++pl;
    }
    ret = max(ret, i + 1 - pl);
  }
  return ret;
}

int main()
{
#ifndef APTEST
  ios_base::sync_with_stdio(false);
#endif
  unsigned int n;
  unsigned long long t;
  cin >> t >> n;
  copy_n(istream_iterator<unsigned long long>(cin), n, a + 1);
  cout << solve(n, t) << "\n";
  return 0;
}
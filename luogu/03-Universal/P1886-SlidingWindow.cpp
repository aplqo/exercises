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
const unsigned int maxn = 1e6;

template <class T>
class mqueue {
 public:
  void push(const unsigned int p, const int val)
  {
    T cmp;
    while (!q.empty() && !cmp(q.back().second, val))
      q.pop_back();
    q.emplace_back(p, val);
  }
  void pop(const unsigned int p)
  {
    while (!q.empty() && q.front().first < p)
      q.pop_front();
  }
  unsigned int top() const { return q.front().second; }

 private:
  deque<pair<unsigned int, int>> q;
};
int val[maxn + 1];
int high[maxn + 1], low[maxn + 1];

void solve(const unsigned int n, const unsigned int k)
{
  mqueue<less<int>> lq;
  mqueue<greater<int>> gq;
  for (unsigned int i = 0; i < k - 1; ++i) {
    gq.push(i, val[i]);
    lq.push(i, val[i]);
  }
  for (unsigned int i = k - 1; i < n; ++i) {
    gq.push(i, val[i]);
    gq.pop(i - k + 1);
    lq.push(i, val[i]);
    lq.pop(i - k + 1);
    high[i] = gq.top();
    low[i] = lq.top();
  }
}

int main()
{
#ifndef APTEST
  ios_base::sync_with_stdio(false);
#endif
  unsigned int n, k;
  cin >> n >> k;
  copy_n(istream_iterator<int>(cin), n, val);
  solve(n, k);
  copy(low + k - 1, low + n, ostream_iterator<int>(cout, " "));
  cout << endl;
  copy(high + k - 1, high + n, ostream_iterator<int>(cout, " "));
  cout << endl;
  return 0;
}
/*Luogu team T131977: Bob*/
#ifdef APTEST
#include "debug_tools/program.h"
#endif
#include <algorithm>
#include <deque>
#include <iostream>
#include <limits>
#include <utility>
using namespace std;
using num = int;
constexpr num ninf = numeric_limits<num>::lowest();
const int maxn = 1000000;

class mqueue {
 public:
  void push(const unsigned int p, const num val)
  {
    while (!q.empty() && q.back().second < val)
      q.pop_back();
    q.emplace_back(p, val);
  }
  void pop(const unsigned int p)
  {
    while (!q.empty() && q.front().first < p)
      q.pop_front();
  }
  num top() const { return q.front().second; }

 private:
  deque<pair<unsigned int, num>> q;
} q;

num f[maxn + 10], a[maxn + 10];

num dp(const unsigned int n, const unsigned int k)
{
  for (unsigned int i = 0; i < n; ++i) {
    q.pop(i < k ? 0 : i - k);
    f[i] = a[i] + max(q.top(), 0);
    q.push(i, f[i]);
  }
  num ret = ninf;
  for (unsigned int i = 0; i < n; ++i)
    ret = max(ret, f[i]);
  return ret;
}
int main()
{
#ifndef APTEST
  ios_base::sync_with_stdio(false);
#endif
  unsigned int n, k;
  cin >> n >> k;
  for (num* i = a; i < a + n; ++i)
    cin >> *i;
  cout << dp(n, k) << endl;
  return 0;
}
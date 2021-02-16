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
const unsigned int maxn = 500000;

template <class T>
class SlideWindow {
 public:
  void push(const unsigned long long p, const long long v)
  {
    while (!q.empty() && cmp(v, q.back().second))
      q.pop_back();
    q.emplace_back(p, v);
  }
  void pop(const unsigned long long v)
  {
    while (q.front().first > v)
      q.pop_front();
  }
  long long front() const { return q.front().second; }

 private:
  T cmp;
  deque<pair<unsigned long long, long long>> q;
};
long long a[maxn + 1];

enum { Lan, Tao };
long long dp(const unsigned long long n, const unsigned long long m)
{
  static long long f[2][maxn + 10];
  SlideWindow<greater<long long>> gq;
  SlideWindow<less<long long>> lq;
  f[Lan][n] = a[n];
  f[Tao][n] = -a[n];
  lq.push(n, f[Tao][n]);
  gq.push(n, f[Lan][n]);
  for (int i = n - 1; i >= 0; --i) {
    gq.pop(i + m);
    lq.pop(i + m);
    f[Tao][i] = gq.front() - a[i];
    f[Lan][i] = lq.front() + a[i];
    gq.push(i, f[Lan][i]);
    lq.push(i, f[Tao][i]);
  }
  return f[Tao][0];
}
int main()
{
#ifndef APTEST
  ios_base::sync_with_stdio(false);
#endif
  unsigned int n, m;
  cin >> n >> m;
  copy_n(istream_iterator<long long>(cin), n, a + 1);
  cout << dp(n, m) << endl;
  return 0;
}
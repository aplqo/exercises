#ifdef APTEST
#include "debug_tools/program.h"
#endif
#include <algorithm>
#include <deque>
#include <iostream>
#include <iterator>
#include <numeric>
#include <utility>
using namespace std;
using real = double;
const unsigned int maxn = 5e4;

long long s[maxn + 1], f[maxn + 1];
long long L;

#define x_i(i) ((i) + s[i] - L - 1)
#define y_i(i) ((i) + s[i])
#define X(k) y_i(k)
#define Y(k) (f[k] + y_i(k) * y_i(k))
#define K(i) (2 * x_i(i))
#define C(i) (x_i(i) * x_i(i))

struct point {
  point(long long x, long long y) : x(x), y(y) {}
  point(unsigned int v) : point(X(v), Y(v)) {}
  long long x, y;
};
inline real slope(const point l, const point r)
{
  return static_cast<real>(l.y - r.y) / (l.x - r.x);
}
static long long solve(const unsigned int n)
{
  static unsigned int q[maxn + 1], *front = q, *back = q + 1;
  for (unsigned int i = 1; i <= n; ++i) {
    while (back - front >= 2 &&
           slope(point(*(front + 1)), point(*front)) < K(i))
      ++front;
    f[i] = Y(*front) - K(i) * X(*front) + C(i);
    while (back - front >= 2 && slope(point(*(back - 1)), point(i)) <
                                    slope(point(*(back - 2)), point(i)))
      --back;
    *(back++) = i;
  }
  return f[n];
}
int main()
{
#ifndef APTEST
  ios_base::sync_with_stdio(false);
#endif
  unsigned int n;
  cin >> n >> L;
  copy_n(istream_iterator<long long>(cin), n, s + 1);
  partial_sum(s + 1, s + n + 1, s + 1);
  cout << solve(n) << endl;
  return 0;
}
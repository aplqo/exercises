#ifdef APTEST
#include "debug_tools/program.h"
#endif
#include <algorithm>
#include <iostream>
#include <iterator>
#include <numeric>
using namespace std;
const unsigned int maxn = 1e6;
using real = long double;

#define K(i) (2 * a * s[i])
#define X(k) s[k]
#define Y(k) (f[k] + a * s[k] * s[k] - b * s[k])
#define C(i) (a * s[i] * s[i] + b * s[i] + c)

long long a, b, c;
long long s[maxn + 1], f[maxn + 1];
struct point {
  point(const long long x, const long long y) : x(x), y(y) {}
  point(const unsigned int i) : point(X(i), Y(i)) {}

  long long x, y;
};

inline real slope(const point a, const point b)
{
  return static_cast<real>(a.y - b.y) / (a.x - b.x);
}
static long long dp(const unsigned int n)
{
  static unsigned int q[maxn + 1], *front = q, *back = q + 1;
  for (unsigned int i = 1; i <= n; ++i) {
    while (back - front >= 2 && slope(*(front + 1), point(*front)) > K(i))
      ++front;
    f[i] = Y(*front) - X(*front) * K(i) + C(i);
    while (back - front >= 2 &&
           slope(*(back - 1), point(i)) > slope(*(back - 2), point(i)))
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
  cin >> n >> a >> b >> c;
  copy_n(istream_iterator<unsigned int>(cin), n, s + 1);
  partial_sum(s + 1, s + 1 + n, s + 1);
  cout << dp(n) << endl;
  return 0;
}
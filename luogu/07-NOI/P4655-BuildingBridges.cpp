#ifdef APTEST
#include "debug_tools/program.h"
#endif
#include <algorithm>
#include <climits>
#include <iostream>
#include <iterator>
#include <numeric>
using namespace std;
const unsigned int maxn = 1e5;
constexpr long long inf = LLONG_MAX / 2;

long long w[maxn + 10], h[maxn + 10], f[maxn + 10];

inline long long square(long long p) { return p * p; }
long long getCost(const unsigned int x, const unsigned int from)
{
  return f[from] + w[x - 1] - w[from] + square(h[x] - h[from]);
}
namespace Convex {
#define X(i) h[i]
#define K(i) (2 * h[i])
#define Y(i) (f[i] - w[i] + square(h[i]))

struct Vector {
  long long x, y;
  inline long long operator*(const Vector& r) const
  {
    return x * r.y - y * r.x;
  }
};
struct Point {
  Point(const long long x, const long long y) : x(x), y(y){};
  Point(const unsigned int i) : Point(X(i), Y(i)){};
  long long x, y;
  inline Vector operator-(const Point& r) const
  {
    return Vector{x - r.x, y - r.y};
  }
};
class ConvexHull {
 public:
  void insert(const unsigned int i)
  {
    const Point cp(i);
    while (back - front > 1 &&
           (cp - Point(*(back - 1))) * (cp - Point(*(back - 2))) >= 0)
      --back;
    *(back++) = i;
  }
  unsigned int top(const unsigned int i)
  {
    const Vector cv{1, K(i)};
    while (back - front > 1 && (Point(*(front + 1)) - Point(*front)) * cv >= 0)
      ++front;
    return *front;
  }
  inline void clear() { front = back = q; }

 private:
  unsigned int q[maxn + 10], *front = q, *back = q;
};
}  // namespace Convex
void cdq(unsigned int* const l, unsigned int* const r)
{
  static Convex::ConvexHull cv;
  static unsigned int tmp[maxn + 10];
  if (r - l == 1) return;
  unsigned int* const mid = l + (r - l) / 2;
  cdq(l, mid);
  cv.clear();
  for (const unsigned int* i = l; i < mid; ++i)
    cv.insert(*i);
  copy(mid, r, tmp);
  {
    unsigned int* const pv = tmp + (r - mid);
    sort(tmp, pv, [](unsigned int a, unsigned int b) { return K(a) < K(b); });
    for (const unsigned int* i = tmp; i < pv; ++i)
      f[*i] = min(f[*i], getCost(*i, cv.top(*i)));
    cdq(mid, r);
  }
  inplace_merge(l, mid, r,
                [](unsigned int a, unsigned int b) { return X(a) < X(b); });
}

int main()
{
#ifndef APTEST
  ios_base::sync_with_stdio(false);
#endif
  unsigned int n;
  cin >> n;
  copy_n(istream_iterator<long long>(cin), n, h + 1);
  for (unsigned int i = 1; i <= n; ++i) {
    long long cw;
    cin >> cw;
    w[i] = w[i - 1] + cw;
  }
  {
    static unsigned int buf[maxn + 10];
    iota(buf + 1, buf + 1 + n, 1);
    fill(f + 2, f + n + 1, inf);
    cdq(buf + 1, buf + n + 1);
  }
  cout << f[n] << "\n";
  return 0;
}
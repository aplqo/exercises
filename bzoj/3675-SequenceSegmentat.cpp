#ifdef APTEST
#include "debug_tools/program.h"
#endif
#include <algorithm>
#include <cctype>
#include <cstdio>
#include <numeric>
using namespace std;
const unsigned int maxn = 100000, maxk = 200;

template <class T>
T read()
{
  T ret = 0;
  char c = getchar();
  while (!isdigit(c))
    c = getchar();
  while (isdigit(c)) {
    ret = ret * 10 + c - '0';
    c = getchar();
  }
  return ret;
}
long long a[maxn + 1], f[maxn + 1][2];
#define X(j) (a[j])
#define K(i) (-a[i])
#define Y(j, k) (f[j][k] - a[j] * a[j])
struct Vector {
  Vector(const long long x, const long long y) : x(x), y(y){};
  inline long long operator*(const Vector r) const
  {
    return this->x * r.y - this->y * r.x;
  }
  long long x, y;
};
struct Point {
  Point(const long long x, const long long y) : x(x), y(y){};
  Point(const unsigned int v, const unsigned int k) : Point(X(v), Y(v, k)){};
  long long x, y;
  inline Vector operator-(const Point r) const
  {
    return Vector(this->x - r.x, this->y - r.y);
  };
};

long long solve(const unsigned int n, const unsigned int k)
{
  static unsigned int q[2][maxn + 1], *front[2] = {q[0], q[1]},
                                      *back[2] = {q[0] + 1, q[1]};
  for (unsigned int i = 1; i <= k; ++i) {
    const unsigned int cur = i & 0x01, lst = !cur;
    back[cur] = front[cur] = q[cur];
    for (unsigned int j = 1; j <= n; ++j) {
      while (back[lst] - front[lst] >= 2 &&
             (Point(*(front[lst] + 1), lst) - Point(*front[lst], lst)) *
                     Vector(1, K(j)) <=
                 0)
        ++front[lst];
      f[j][cur] =
          f[*front[lst]][lst] + (a[j] - a[*front[lst]]) * a[*front[lst]];
      const Point cp(j, cur);
      while (back[cur] - front[cur] >= 2 &&
             (cp - Point(*(back[cur] - 2), cur)) *
                     (cp - Point(*(back[cur] - 1), cur)) >=
                 0)
        --back[cur];
      *(back[cur]++) = j;
    }
  }
  return f[n][k & 0x01];
}
int main()
{
  const unsigned int n = read<unsigned int>();
  const auto k = read<unsigned int>() + 1;
  generate_n(a + 1, n, read<long long>);
  partial_sum(a + 1, a + 1 + n, a + 1);
  printf("%lld\n", solve(n, k));
  return 0;
}
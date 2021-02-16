#ifdef APTEST
#include "debug_tools/program.h"
#endif
#include <iostream>
#include <iterator>
#include <numeric>
using namespace std;
const unsigned int maxn = 3e5;
typedef long long Cost;

Cost f[maxn + 10], c[maxn + 10], sum[maxn + 10];
#define X(j) (sum[(j)-1])
#define Y(j) f[j]
#define K(i) (-c[i])
struct Vertex {
  Vertex(Cost x, Cost y) : x(x), y(y){};
  Cost x, y;
  inline Cost operator*(const Vertex r) const { return x * r.y - y * r.x; }
};
struct Point {
  Point(const Cost x, const Cost y) : x(x), y(y){};
  Point(const unsigned int i) : x(X(i)), y(Y(i)){};
  inline Vertex operator-(const Point b) const
  {
    return Vertex(x - b.x, y - b.y);
  }
  Cost x, y;
};

static void init(const unsigned int n)
{
  partial_sum(sum, sum + n + 2, sum);
  const reverse_iterator<Cost*> ri = reverse_iterator<Cost*>(c + n + 1);
  partial_sum(ri, ri + n + 1, ri);
}
Cost dp(const unsigned int n, const Cost s)
{
  static unsigned int q[maxn + 10], *front = q, *back = q + 1;
  q[0] = n + 1;
  for (unsigned int i = n; i; --i) {
    {
      const Vertex cv(1, K(i));
      while (back - front > 1 &&
             cv * (Point(*front) - Point(*(front + 1))) >= 0)
        ++front;
    }
    f[i] = f[*front] + (s + sum[*front - 1] - sum[i - 1]) * c[i];
    {
      const Point cp(i);
      while (back - front > 1 &&
             (cp - Point(*(back - 2))) * (cp - Point(*(back - 1))) >= 0)
        --back;
      *(back++) = i;
    }
  }
  return f[1];
}

int main()
{
#ifndef APTEST
  ios_base::sync_with_stdio(false);
#endif
  unsigned int n;
  Cost s;
  cin >> n >> s;
  for (unsigned int i = 1; i <= n; ++i)
    cin >> sum[i] >> c[i];
  init(n);
  cout << dp(n, s) << "\n";
  return 0;
}
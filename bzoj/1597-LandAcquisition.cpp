#ifdef APTEST
#include "debug_tools/program.h"
#endif
#include <algorithm>
#include <iostream>
using namespace std;
const unsigned int maxn = 5e4;

struct Land {
  long long c, r;
} lnd[maxn + 1], lar[maxn + 1];
long long f[maxn + 1];
#define K(i) (lar[i].c)
#define X(j) (-lar[j + 1].r)
#define Y(j) (f[j])
struct Vector {
  Vector(const long long x, const long long y) : x(x), y(y){};
  long long x, y;
  inline long long operator*(const Vector r) const { return x * r.y - y * r.x; }
};
struct Point {
  Point(const long long x, const long long y) : x(x), y(y){};
  Point(const unsigned int i) : Point(X(i), Y(i)){};
  Vector operator-(const Point r) const { return Vector(x - r.x, y - r.y); }
  long long x, y;
};

static unsigned int init(const unsigned int n)
{
  sort(lnd, lnd + n, [](const Land a, const Land b) {
    return a.c != b.c ? a.c < b.c : a.r > b.r;
  });
  Land* p = lar + 1;
  *p = lnd[0];
  for (unsigned int i = 1; i < n; ++i) {
    while (p > lar && lnd[i].r >= p->r && lnd[i].c >= p->c)
      --p;
    *(++p) = lnd[i];
  }
  return p - lar;
}
long long dp(const unsigned int n)
{
  static unsigned int q[maxn + 1], *front = q, *back = q + 1;
  for (unsigned int i = 1; i <= n; ++i) {
    while (back - front >= 2 &&
           (Point(*(front + 1)) - Point(*front)) * Vector(1, K(i)) >= 0)
      ++front;
    f[i] = f[*front] + lar[*front + 1].r * lar[i].c;
    const Point cp(i);
    while (back - front >= 2 &&
           (cp - Point(*(back - 1))) * (cp - Point(*(back - 2))) >= 0)
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
  cin >> n;
  for (unsigned int i = 0; i < n; ++i)
    cin >> lnd[i].r >> lnd[i].c;
  cout << dp(init(n)) << endl;
  return 0;
}
#ifdef APTEST
#include "debug_tools/program.h"
#endif
#include <algorithm>
#include <iostream>
#include <iterator>
using namespace std;
const unsigned int maxn = 1000, maxm = 1000;
constexpr unsigned long long mod = 998244353;

class Number {
 public:
  Number() = default;
  Number(const unsigned long long v) : val(v % mod){};
#define decl(x)                                                   \
  friend inline Number operator x(const Number l, const Number r) \
  {                                                               \
    return (l.val + mod) x r.val;                                 \
  }                                                               \
  inline Number& operator x##=(const Number r) { return *this = *this x r; }
  decl(+);
  decl(-);
  decl(*);
#undef decl
  friend inline Number operator/(const Number l, const Number r);
  friend inline ostream& operator<<(ostream& os, const Number v)
  {
    return os << v.val;
  }

 private:
  unsigned long long val;
};
Number quickPow(Number v, unsigned long long e)
{
  Number ret = 1;
  for (; e; e >>= 1) {
    if (e & 0x01) ret *= v;
    v *= v;
  }
  return ret;
}
inline Number operator/(const Number l, const Number r)
{
  return l * quickPow(r, mod - 2);
}
unsigned int dat[maxn + 1][maxm + 1];
Number exp[maxn + 1][maxm + 1];
struct Point {
  unsigned int x, y;

  inline unsigned int distOrig() const { return x * x + y * y; }
  inline bool operator<(const Point r) const
  {
    return dat[x][y] < dat[r.x][r.y];
  }
  template <class T>
  inline T& operator[](T dat[maxn + 1][maxm + 1]) const
  {
    return dat[x][y];
  }
} p[maxn * maxm + 1];

static void init(const unsigned int n, const unsigned int m)
{
  Point* cur = p;
  for (unsigned int i = 0; i < n; ++i)
    for (unsigned int j = 0; j < m; ++j)
      new (cur++) Point{i, j};
  sort(p, p + n * m);
}
void dp(const unsigned int cnt)
{
  static unsigned int sx[maxn * maxm + 1], sy[maxn * maxm + 1];
  static Number square[maxn * maxm + 1], se[maxn * maxm + 1];
  unsigned int i = 1;
  sx[0] = p[0].x;
  sy[0] = p[0].y;
  square[0] = p[0].distOrig();
  for (; i < cnt && p[i][dat] == p[0][dat]; ++i) {
    sx[i] = sx[i - 1] + p[i].x;
    sy[i] = sy[i - 1] + p[i].y;
    square[i] = square[i - 1] + p[i].distOrig();
  }
  for (; i < cnt; ++i) {
    const Point cp = p[i];
    const unsigned int lstp = lower_bound(p, p + i + 1, p[i]) - p - 1;
    cp[exp] = (square[lstp] - Number(2) * cp.x * sx[lstp] -
               Number(2) * cp.y * sy[lstp] + se[lstp]) /
                  (lstp + 1) +
              cp.distOrig();
    sx[i] = sx[i - 1] + cp.x;
    sy[i] = sy[i - 1] + cp.y;
    square[i] = square[i - 1] + cp.distOrig();
    se[i] = se[i - 1] + cp[exp];
  }
}
int main()
{
#ifndef APTEST
  ios_base::sync_with_stdio(false);
#endif
  unsigned int r, c;
  cin >> r >> c;
  for (unsigned int i = 0; i < r; ++i)
    copy_n(istream_iterator<unsigned int>(cin), c, dat[i]);
  init(r, c);
  dp(r * c);
  unsigned int x, y;
  cin >> x >> y;
  cout << exp[x - 1][y - 1] << "\n";
  return 0;
}
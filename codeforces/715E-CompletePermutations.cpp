#ifdef APTEST
#include "debug_tools/judge.h"
#endif
#include <algorithm>
#include <iostream>
#include <iterator>
#include <numeric>
const unsigned int maxn = 250;
constexpr unsigned long long mod = 998244353;

class Number {
 public:
#define decl(x)                                                   \
  inline friend Number operator x(const Number l, const Number r) \
  {                                                               \
    return Number{(l.val x r.val) % mod};                         \
  }                                                               \
  inline void operator x##=(const Number r) { val = (val x r.val) % mod; }
  decl(+);
  decl(*);
#undef decl
  inline friend std::ostream& operator<<(std::ostream& os, const Number v)
  {
    return os << v.val;
  }

  unsigned long long val;
};
struct ConstArrayPtr {
  const Number* ptr;
  unsigned int size;
};
struct EdgeStat {
  unsigned int cycle;
  unsigned int undToA;    // 0->a
  unsigned int aToUnd;    // a->0
  unsigned int undToUnd;  // 0->0
};
Number fac[maxn + 10], ifac[maxn + 10];
Number s[maxn + 10][maxn + 10];

namespace Disjoint {
unsigned int fa[maxn + 10], size[maxn + 10];

static void init(const unsigned int n)
{
  std::fill(size + 1, size + 1 + n, 1);
  std::iota(fa + 1, fa + 1 + n, 1);
}
unsigned int find(const unsigned int x)
{
  return fa[x] == x ? x : fa[x] = find(fa[x]);
}
bool merge(unsigned int x, unsigned int y)
{
  x = find(x);
  y = find(y);
  if (x == y) return false;
  if (size[x] < size[y]) std::swap(x, y);
  fa[y] = x;
  size[x] += size[y];
  return true;
}
}  // namespace Disjoint
static void initModularInv(const unsigned int n)
{
  ifac[0].val = ifac[1].val = 1;
  for (unsigned int i = 2; i <= n; ++i)
    ifac[i].val = mod - (ifac[mod % i] * Number{mod / i}).val;
}
static void initFactor(const unsigned int n)
{
  initModularInv(n);
  fac[0].val = 1;
  for (unsigned int i = 1; i <= n; ++i) {
    fac[i] = fac[i - 1] * Number{i};
    ifac[i] *= ifac[i - 1];
  }
}
static void initStirling(const unsigned int n)
{
  s[0][0].val = s[1][1].val = 1;
  for (unsigned int i = 2; i <= n; ++i)
    for (unsigned int j = 1; j <= i; ++j)
      s[i][j] = s[i - 1][j - 1] + s[i - 1][j] * Number{i - 1};
}
static void init(const unsigned int n)
{
  initFactor(n);
  initStirling(n);
}

inline Number C(const unsigned int n, const unsigned int m)
{
  return fac[n] * ifac[n - m] * ifac[m];
}
inline Number fallFactor(const unsigned int n, const unsigned int e)
{
  return fac[n] * ifac[n - e];
}
void convolution(const ConstArrayPtr a, const ConstArrayPtr b, Number dest[])
{
  for (unsigned int i = 0; i < a.size; ++i)
    for (unsigned int j = 0; j < b.size; ++j)
      dest[i + j] += a.ptr[i] * b.ptr[j];
}
void convolution(const ConstArrayPtr a, const ConstArrayPtr b,
                 const ConstArrayPtr c, Number dest[])
{
  static Number tmp[maxn + 10];
  convolution(a, b, tmp);
  convolution(ConstArrayPtr{tmp, a.size + b.size - 1}, c, dest);
}
EdgeStat simpilify(const unsigned int n, const unsigned int p1[],
                   const unsigned int p2[])
{
  static bool from[maxn + 10], to[maxn + 10];
  EdgeStat ret{};
  Disjoint::init(n);
  for (unsigned int i = 0; i < n; ++i)
    if (p1[i] && p2[i] && !Disjoint::merge(p1[i], p2[i])) ++ret.cycle;
  for (unsigned int i = 0; i < n; ++i)
    if (!p1[i] && !p2[i])
      ++ret.undToUnd;
    else if (!p1[i]) {
      const unsigned int actual = Disjoint::find(p2[i]);
      if (!to[actual]) {
        from[actual] = true;
        ++ret.undToA;
      }
      else {
        --ret.aToUnd;
        ++ret.undToUnd;
        to[actual] = false;
      }
    }
    else if (!p2[i]) {
      const unsigned int actual = Disjoint::find(p1[i]);
      if (!from[actual]) {
        to[actual] = true;
        ++ret.aToUnd;
      }
      else {
        --ret.undToA;
        ++ret.undToUnd;
        from[actual] = false;
      }
    }
  return ret;
}
void oneUnd(const unsigned int n, const unsigned int m, Number dest[])
{
  if (!m) {
    for (unsigned int i = 0; i <= n; ++i)
      dest[i] = s[n][i];
  }
  else {
    for (unsigned int i = 0; i <= n; ++i)
      for (unsigned int j = i; j <= n; ++j)
        dest[i] += C(n, j) * s[j][i] * fallFactor(n + m - j - 1, n - j);
  }
}
void bothUnd(const unsigned int m, Number dest[])
{
  for (unsigned int i = 0; i <= m; ++i)
    dest[i] = s[m][i] * fac[m];
}
void solve(const unsigned int n, const unsigned int p1[],
           const unsigned int p2[], Number dest[])
{
  static Number tmpa[maxn + 10], tmpb[maxn + 10], tmpc[maxn + 10];
  const EdgeStat es = simpilify(n, p1, p2);
  oneUnd(es.aToUnd, es.undToUnd, tmpa);
  oneUnd(es.undToA, es.undToUnd, tmpb);
  bothUnd(es.undToUnd, tmpc);
  convolution(ConstArrayPtr{tmpa, es.aToUnd + 1},
              ConstArrayPtr{tmpb, es.undToA + 1},
              ConstArrayPtr{tmpc, es.undToUnd + 1}, dest + es.cycle);
}

int main()
{
  std::ios::sync_with_stdio(false);
  unsigned int n;
  std::cin >> n;
  static unsigned int p1[maxn + 10], p2[maxn + 10];
  static Number dest[maxn + 10];
  std::copy_n(std::istream_iterator<unsigned int>(std::cin), n, p1);
  std::copy_n(std::istream_iterator<unsigned int>(std::cin), n, p2);
  init(n);
  solve(n, p1, p2, dest);
  for (unsigned int i = 0; i < n; ++i)
    std::cout << dest[n - i] << " ";
  return 0;
}
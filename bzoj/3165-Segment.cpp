#ifdef APTEST
#include "debug_tools/judge.h"
#endif
#include <algorithm>
#include <cassert>
#include <cmath>
#include <iostream>
using namespace std;
constexpr unsigned int modx = 39989, mody = 1e9;
const unsigned long long maxn = 1e5, maxx = 39989;
using real = double;
constexpr real eps = 1e-8;

struct Line {
  Line() : id(0){};
  Line(const unsigned int id, int x1, long long y1, int x2, long long y2)
      : id(id), k((real(y1) - y2) / (real(x1) - x2)), b(y1 - k * x1){};
  inline real eval(const int x) const { return k * x + b; }
  inline real intersect(const Line& r) const
  {
    return (r.b - this->b) / (this->k - r.k);
  }

  real k, b;
  unsigned int id;
} lin[maxn + 10];
inline bool equal(const real a, const real b) { return abs(a - b) <= eps; }
struct ValFrom {
  real val;
  unsigned int from;
  inline bool operator<(const ValFrom& r) const
  {
    return !equal(val, r.val) ? r.val - val >= eps : from > r.from;
  }
};
class SegmentTree {
 public:
  void build(const unsigned int l, const unsigned int r, SegmentTree*(&cur))
  {
    static const Line nil{};
    lv = l;
    rv = r;
    mv = (l + r) >> 1;
    val = &nil;
    if (r - l == 1) return;
    lc = ++cur;
    rc = ++cur;
    lc->build(l, mv, cur);
    rc->build(mv, r, cur);
  }
  ValFrom find(const unsigned int x) const
  {
    const ValFrom thisVal{val->eval(x), val->id};
    if (rv - lv == 1) return thisVal;
    return std::max((x < mv ? lc : rc)->find(x), thisVal);
  }
  void updateLine(const unsigned int l, const unsigned int r, const Line* line)
  {
    if (l == lv && r == rv) {
      addLine(line);
      return;
    }
    if (r <= mv)
      lc->updateLine(l, r, line);
    else if (l >= mv)
      rc->updateLine(l, r, line);
    else {
      lc->updateLine(l, mv, line);
      rc->updateLine(mv, r, line);
    }
  }

 private:
  void addLine(const Line* ln)
  {
    const real cvl = val->eval(lv), cvr = val->eval(rv - 1), nvl = ln->eval(lv),
               nvr = ln->eval(rv - 1);
    const bool el = equal(cvl, nvl), er = equal(cvr, nvr);
    if ((nvl - cvl >= eps && nvr - cvr >= eps) ||
        (el && er && val->id > ln->id))
      val = ln;
    else if (((cvl - nvl >= -eps) ^ (cvr - nvr >= -eps)) || (el ^ er)) {
      if (ln->k < val->k) std::swap(ln, val);
      const real i = val->intersect(*ln);
      if (mv - i >= eps) {
        lc->addLine(val);
        val = ln;
      }
      else
        rc->addLine(ln);
    }
  }

  SegmentTree *lc, *rc;
  unsigned int lv, mv, rv;
  const Line* val;
} SNodes[maxn * 4 + 1], *SRoot = SNodes;
ValFrom vertical[maxn + 10];

void addLine(unsigned int x0, unsigned int y0, unsigned int x1, unsigned int y1)
{
  static unsigned int cur = 0;
  if (x0 > x1) {
    swap(x0, x1);
    swap(y0, y1);
  }
  ++cur;
  if (x0 == x1)
    vertical[x0] = std::max(vertical[x0], ValFrom{std::max(y0, y1), cur});
  else {
    new (&lin[cur]) Line(cur, x0, y0, x1, y1);
    SRoot->updateLine(x0, x1 + 1, &lin[cur]);
  }
}
inline unsigned int find(const unsigned int x)
{
  return std::max(SRoot->find(x), vertical[x]).from;
}

int main()
{
  std::ios_base::sync_with_stdio(false);
  std::cin.tie(nullptr);
  std::cout.tie(nullptr);
  unsigned long long n, lst = 0;
  cin >> n;
  {
    SegmentTree* c = SNodes;
    SRoot->build(0, maxx + 1, c);
  }
  const auto decodex = [&lst](const unsigned long long x) -> unsigned int {
#ifndef NoMod
    return (x + lst + modx - 1) % modx + 1;
#else
    return x;
#endif
  };
  const auto decodey = [&lst](const unsigned long long y) -> unsigned int {
#ifndef NoMod
    return (y + lst + mody - 1) % mody + 1;
#else
    return y;
#endif
  };
  for (unsigned int i = 0; i < n; ++i) {
    unsigned int op;
    cin >> op;
    if (op == 0) {
      unsigned int x;
      cin >> x;
      cout << (lst = find(decodex(x))) << "\n";
    }
    else {
      unsigned int x0, y0, x1, y1;
      cin >> x0 >> y0 >> x1 >> y1;
      addLine(decodex(x0), decodey(y0), decodex(x1), decodey(y1));
    }
  }
  return 0;
}
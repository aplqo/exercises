#ifdef APTEST
#include "debug_tools/program.h"
#endif
#include <algorithm>
#include <iostream>
#include <iterator>
#include <limits>
#include <numeric>
#include <vector>
using namespace std;
const unsigned int maxn = 250000, maxe = 10;
typedef int Rank;
constexpr Rank inf = numeric_limits<Rank>::max(),
               ninf = numeric_limits<Rank>::min();

class SegmentTree {
 public:
  void build(const unsigned int l, const unsigned int r, SegmentTree*& cur,
             const Rank dat[]);
  Rank prefixMax(const unsigned int r) const;
  void assign(const unsigned int pos, const Rank v);
  unsigned int lower_bound(const Rank val) const;

 private:
  unsigned int lv, mv, rv;
  Rank greatest, val;
  SegmentTree *lc, *rc;
} snodes[maxn * 4 * 2 + 10];
SegmentTree* roots[2];
vector<unsigned int> tops;
Rank d[maxn + 10];

void SegmentTree::build(const unsigned int l, const unsigned int r,
                        SegmentTree*& cur, const Rank dat[])
{
  lv = l;
  rv = r;
  mv = (l + r) >> 1;
  if (r - l == 1) {
    val = greatest = dat[l];
    return;
  }
  lc = ++cur;
  lc->build(l, mv, cur, dat);
  rc = ++cur;
  rc->build(mv, r, cur, dat);
  greatest = max(lc->greatest, rc->greatest);
}
Rank SegmentTree::prefixMax(const unsigned int r) const
{
  if (rv == r) return greatest;
  return r <= mv ? lc->prefixMax(r) : max(lc->greatest, rc->prefixMax(r));
}
void SegmentTree::assign(const unsigned int pos, const Rank v)
{
  if (rv - lv == 1) {
    greatest = val = v;
    return;
  }
  (pos < mv ? lc : rc)->assign(pos, v);
  greatest = max(lc->greatest, rc->greatest);
}
unsigned int SegmentTree::lower_bound(const Rank val) const
{
  if (rv - lv == 1) return this->val >= val ? lv : rv;
  return (lc->greatest < val ? rc : lc)->lower_bound(val);
}

static void init(const unsigned int n, const unsigned int a)
{
  static Rank tmp[maxn + 10];
  tops.resize(n + 1);
  iota(tops.begin(), tops.end(), 0);
  sort(tops.begin() + 1, tops.end(),
       [](unsigned int a, unsigned int b) { return d[a] > d[b]; });
  tops.resize(min(n, maxe) + 1);
  {
    SegmentTree* c = snodes;
    roots[0] = c;
    reverse_copy(d + 1, d + a, tmp + 1);
    roots[0]->build(1, a, c, tmp);
    roots[1] = ++c;
    roots[1]->build(1, n + 1 - a, c, d + a);
  }
}
inline unsigned int distance(unsigned int a, unsigned int b)
{
  return a > b ? a - b : b - a;
}
unsigned int minBefore(const unsigned int pos, const unsigned int a)
{
  const bool typ = pos > a;
  if (pos == a) return 0;
  const unsigned int idx = distance(pos, a);
  return idx + roots[!typ]->lower_bound(roots[typ]->prefixMax(idx + 1)) - 1;
}
void enhance(const unsigned int pos, const unsigned int e, const unsigned int a)
{
  Rank lst = d[tops[e]];
  const auto oldSize = tops.size();
  if (const auto it = find(tops.begin(), tops.end(), pos); it != tops.end())
    tops.erase(it);
  tops.insert(tops.begin() + e, pos);
  tops.resize(oldSize);
  for (auto it = tops.begin() + e; it != tops.begin(); --it) {
    if (*it != a) roots[*it > a]->assign(distance(*it, a), ++lst);
    d[*it] = lst;
  }
}

int main()
{
#ifndef APTEST
  ios_base::sync_with_stdio(false);
#endif
  unsigned int n, a, q;
  cin >> n >> a;
  copy_n(istream_iterator<Rank>(cin), n, d + 1);
  init(n, a);
  cin >> q;
  for (unsigned int i = 0; i < q; ++i) {
    char op;
    cin >> op;
    if (op == 'F') {
      unsigned int p;
      cin >> p;
      cout << minBefore(p, a) << "\n";
    }
    else {
      unsigned int p, e;
      cin >> p >> e;
      enhance(p, e, a);
    }
  }
  return 0;
}
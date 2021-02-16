#ifdef APTEST
#include "debug_tools/program.h"
#endif
#include <algorithm>
#include <climits>
#include <iostream>
using namespace std;
const unsigned int maxn = 1e5;
typedef double real;

class SegmentTree {
 public:
  void build(const unsigned int l, const unsigned int r, SegmentTree *&(cur));
  void assign(const unsigned int pos, const real val);
  unsigned int lis() const;

 private:
  inline void pushup();
  unsigned int binary(const real mnv) const;

  unsigned int lv, mv, rv;
  SegmentTree *lc, *rc;
  real selectMin, allMax;
  unsigned int size;
} snodes[maxn * 4 + 10], *root = snodes;
struct Modify {
  unsigned int pos, val;
  real k;
} mod[maxn + 10];

void SegmentTree::build(const unsigned int l, const unsigned int r,
                        SegmentTree *&cur)
{
  lv = l;
  rv = r;
  mv = (l + r) >> 1;
  if (r - l == 1) {
    size = selectMin = allMax = 0;
    return;
  }
  lc = ++cur;
  lc->build(l, mv, cur);
  rc = ++cur;
  rc->build(mv, r, cur);
  pushup();
}
void SegmentTree::assign(const unsigned int pos, const real val)
{
  if (rv - lv == 1) {
    size = val > 0;
    selectMin = allMax = val;
    return;
  }
  (pos < mv ? lc : rc)->assign(pos, val);
  pushup();
}
unsigned int SegmentTree::lis() const { return size; }
inline void SegmentTree::pushup()
{
  selectMin = lc->selectMin;
  if (rc->allMax <= lc->allMax) {
    allMax = lc->allMax;
    size = lc->size;
    return;
  }
  else {
    allMax = rc->allMax;
    size = lc->size + rc->binary(lc->allMax);
  }
}
unsigned int SegmentTree::binary(const real mnv) const
{
  if (mnv >= allMax) return 0;
  if (mnv < selectMin) return size;
  if (lc->allMax <= mnv)
    return rc->binary(mnv);
  else
    return (size - lc->size) + lc->binary(mnv);
}

static unsigned int discretize(const unsigned int m)
{
  static unsigned int tmp[maxn + 19];
  unsigned int *cur = tmp;
  for (const Modify *i = mod; i < mod + m; ++i)
    *(cur++) = i->pos;
  sort(tmp, cur);
  cur = unique(tmp, cur);
  for (Modify *i = mod; i < mod + m; ++i) {
    i->k = real(i->val) / i->pos;
    i->pos = lower_bound(tmp, cur, i->pos) - tmp;
  }
  return cur - tmp;
}

int main()
{
#ifndef APTEST
  ios_base::sync_with_stdio(false);
#endif
  unsigned int n, m;
  cin >> n >> m;
  for (Modify *i = mod; i < mod + m; ++i)
    cin >> i->pos >> i->val;
  {
    SegmentTree *c = snodes;
    root->build(0, discretize(m), c);
  }
  for (const Modify *i = mod; i < mod + m; ++i) {
    root->assign(i->pos, i->k);
    cout << root->lis() << "\n";
  }
  return 0;
}

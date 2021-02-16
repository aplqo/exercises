#ifdef APTEST
#include "debug_tools/program.h"
#endif
#include <algorithm>
#include <iostream>
using namespace std;
const unsigned int maxn = 1e4;

struct Point {
  unsigned int x, y;
  int val;
} p[maxn + 10];
class SegmentTree {
 public:
  void init(const unsigned int l, const unsigned int r);
  void update(const unsigned int l, const unsigned int r, const int delt);
  int max(const unsigned int l, const unsigned int r);

  static inline SegmentTree* access(SegmentTree*(&p), const unsigned int l,
                                    const unsigned int r);
  static inline SegmentTree* maybe(SegmentTree* p);
  static SegmentTree* cur;

  SegmentTree *lc, *rc;
  unsigned int lv, mv, rv;
  int val, laz;

 private:
  void push(const int laz);
  void pushdown();

} nodes[maxn * 64 + 10], *root = nodes;
SegmentTree* SegmentTree::cur = nodes;

void SegmentTree::init(const unsigned int l, const unsigned int r)
{
  lv = l;
  rv = r;
  mv = (l + r) >> 1;
  lc = rc = nullptr;
  val = laz = 0;
}
void SegmentTree::update(const unsigned int l, const unsigned int r,
                         const int delt)
{
  if (l == lv && r == rv) {
    push(delt);
    return;
  }
  pushdown();
  if (r <= mv)
    access(lc, lv, mv)->update(l, r, delt);
  else if (l >= mv)
    access(rc, mv, rv)->update(l, r, delt);
  else {
    access(lc, lv, mv)->update(l, mv, delt);
    access(rc, mv, rv)->update(mv, r, delt);
  }
  val = std::max(maybe(lc)->val, maybe(rc)->val);
}
int SegmentTree::max(const unsigned int l, const unsigned int r)
{
  if ((l == lv && r == rv) || (!lc && !rc)) return val;
  pushdown();
  if (r <= mv)
    return maybe(lc)->max(l, r);
  else if (l >= mv)
    return maybe(rc)->max(l, r);
  else
    return std::max(maybe(lc)->max(l, mv), maybe(rc)->max(mv, r));
}
void SegmentTree::push(const int laz)
{
  this->val += laz;
  this->laz += laz;
}
void SegmentTree::pushdown()
{
  if (laz == 0) return;
  access(lc, lv, mv)->push(laz);
  access(rc, mv, rv)->push(laz);
  laz = 0;
}
inline SegmentTree* SegmentTree::access(SegmentTree*(&p), const unsigned int l,
                                        const unsigned int r)
{
  if (p) return p;
  p = ++cur;
  p->init(l, r);
  return p;
}
unsigned int findMax(const unsigned int n, const unsigned int w,
                     const unsigned int h)
{
  unsigned int ret = 0;
  sort(p, p + n, [](Point& a, Point& b) { return a.x < b.x; });
  const Point *pl = p, *pr = p;
  for (const Point* i = p; i < p + n; ++i) {
    for (const unsigned int r = i->x; pr < p + n && pr->x <= r; ++pr)
      root->update(pr->y - min(h, pr->y), pr->y + 1, pr->val);
    for (const unsigned int x = i->x; pl < p + n && pl->x + w < x; ++pl)
      root->update(pl->y - min(h, pl->y), pl->y + 1, -(pl->val));
    ret = max<unsigned int>(ret, root->max(i->y - min(h, i->y), i->y + 1));
  }
  return ret;
}
inline SegmentTree* SegmentTree::maybe(SegmentTree* p)
{
  static SegmentTree nil{};
  return p ? p : &nil;
}

int main()
{
#ifndef APTEST
  ios_base::sync_with_stdio(false);
#endif
  unsigned int n, w, h;
  cin >> n >> w >> h;
  for (Point* i = p; i < p + n; ++i)
    cin >> i->x >> i->y >> i->val;
  root->init(0, 1u << 31);
  cout << findMax(n, w - 1, h - 1) << "\n";
  return 0;
}
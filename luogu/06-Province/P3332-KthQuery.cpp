#ifdef APTEST
#include "debug_tools/program.h"
#endif
#include <algorithm>
#include <iostream>
using namespace std;
const unsigned int maxn = 5e4, maxm = 5e4;

class STree {
 public:
  void build(const unsigned int l, const unsigned int r, STree*(&cur));
  void increase(const unsigned int l, const unsigned r);
  unsigned int query(const unsigned int l, const unsigned int r) const;
  void clear();

 private:
  unsigned int lv, mv, rv;
  unsigned int val, laz;
  STree *lc, *rc;
  bool dirty;
} SNodes[maxn * 4 + 1], *root = SNodes;
struct Operate {
  bool typ;  // true: change false: query
  unsigned int l, r;
  long long c, ans;
} obj[maxm + 1];
Operate* seq[maxm + 1];
long long orig[maxm + 1], dcnt;

void STree::build(const unsigned int l, const unsigned int r, STree*(&cur))
{
  lv = l;
  rv = r;
  mv = (l + r) >> 1;
  val = laz = 0;
  dirty = false;
  if (r - l == 1) return;
  lc = ++cur;
  rc = ++cur;
  lc->build(l, mv, cur);
  rc->build(mv, r, cur);
}
void STree::increase(const unsigned int l, const unsigned int r)
{
  dirty = true;
  val += r - l;
  if (l == lv && r == rv) {
    ++laz;
    return;
  }
  if (r <= mv)
    lc->increase(l, r);
  else if (l >= mv)
    rc->increase(l, r);
  else {
    lc->increase(l, mv);
    rc->increase(mv, r);
  }
}
unsigned int STree::query(const unsigned int l, const unsigned int r) const
{
  if (l == lv && r == rv) return val;
  if (r <= mv)
    return lc->query(l, r) + (r - l) * laz;
  else if (l >= mv)
    return rc->query(l, r) + (r - l) * laz;
  else
    return lc->query(l, mv) + rc->query(mv, r) + (r - l) * laz;
}
void STree::clear()
{
  if (!dirty) return;
  val = laz = 0;
  dirty = false;
  if (rv - lv == 1) return;
  lc->clear();
  rc->clear();
}

static void discretize(const unsigned int m)
{
  long long* oe = orig;
  for (Operate* i = obj; i < obj + m; ++i)
    if (i->typ) *(oe++) = i->c;
  sort(orig, oe);
  oe = unique(orig, oe);
  dcnt = oe - orig;
  for (Operate* i = obj; i < obj + m; ++i)
    if (i->typ) i->c = lower_bound(orig, oe, i->c) - orig;
}
void binary(const unsigned int lv, const unsigned int rv, const unsigned int lq,
            const unsigned int rq)
{
  static Operate *ql[maxm + 1], *qr[maxm + 1];
  if (rv - lv == 1) {
    for (unsigned int i = lq; i < rq; ++i)
      if (!seq[i]->typ) seq[i]->ans = orig[lv];
    return;
  }
  const unsigned int mid = (lv + rv) >> 1;
  unsigned int lp = 0, rp = 0;
  for (unsigned int i = lq; i < rq; ++i) {
    Operate* const c = seq[i];
    if (c->typ) {
      if (c->c >= mid) {
        root->increase(c->l, c->r + 1);
        qr[rp++] = c;
      }
      else
        ql[lp++] = c;
    }
    else {
      const unsigned int v = root->query(c->l, c->r + 1);
      if (v < c->c) {
        c->c -= v;
        ql[lp++] = c;
      }
      else
        qr[rp++] = c;
    }
  }
  const unsigned int mq = lq + lp;
  copy(ql, ql + lp, seq + lq);
  copy(qr, qr + rp, seq + mq);
  root->clear();
  binary(lv, mid, lq, mq);
  binary(mid, rv, mq, rq);
}
int main()
{
#ifndef APTEST
  ios_base::sync_with_stdio(false);
#endif
  unsigned int n, m;
  cin >> n >> m;
  {
    STree* c = SNodes;
    root->build(1, n + 1, c);
  }
  for (Operate* i = obj; i < obj + m; ++i) {
    unsigned int op;
    cin >> op >> i->l >> i->r >> i->c;
    i->typ = op == 1;
  }
  discretize(m);
  transform(obj, obj + m, seq, [](Operate& a) { return &a; });
  binary(0, dcnt + 1, 0, m);
  for (Operate* i = obj; i < obj + m; ++i)
    if (!i->typ) cout << i->ans << endl;
  return 0;
}
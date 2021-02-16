#ifdef APTEST
#include "debug_tools/program.h"
#endif
#include <algorithm>
#include <climits>
#include <iostream>
using namespace std;
const unsigned int maxn = 2e5, maxm = 2e5;
constexpr unsigned int inf = UINT_MAX;

struct Edge {
  unsigned int from, to;
} ed[maxm + 10];
unsigned int earlistTime[maxm + 10];
class SegmentTree {
 public:
  void build();
  void increase(const SegmentTree* old, const unsigned int pos);
  unsigned int countLess(const SegmentTree* old, const unsigned int n) const;

  static inline SegmentTree* create(const unsigned int l, const unsigned int r);
  static SegmentTree* cur;

 private:
  const SegmentTree *lc, *rc;
  unsigned int lv, mv, rv;
  unsigned int val;
} nodes[maxn * 4 + 10 + maxm * 30];
SegmentTree* SegmentTree::cur = nodes;
const SegmentTree* root[maxm + 10];
namespace LinkCutTree {
struct SplayNode {
  inline void pushRev();
  inline void pushdown();
  inline void pushup();
  inline bool isRoot();
  void pushPath();
  void rotate();
  void splay();

  bool rev = false;
  unsigned int val = inf, minVal = inf;
  SplayNode *fa = nullptr, *chl[2] = {};
};
SplayNode vertex[maxn + 10], virt[maxm + 10], *cur = virt;
}  // namespace LinkCutTree

inline SegmentTree* SegmentTree::create(const unsigned int l,
                                        const unsigned int r)
{
  cur->lv = l;
  cur->mv = (l + r) >> 1;
  cur->rv = r;
  return cur++;
}
void SegmentTree::build()
{
  val = 0;
  if (rv - lv == 1) return;
  lc = cur;
  create(lv, mv)->build();
  rc = cur;
  create(mv, rv)->build();
}
void SegmentTree::increase(const SegmentTree* old, const unsigned int pos)
{
  if (rv - lv == 1) {
    val = old->val + 1;
    return;
  }
  if (pos < mv) {
    rc = old->rc;
    lc = cur;
    create(lv, mv)->increase(old->lc, pos);
  }
  else {
    lc = old->lc;
    rc = cur;
    create(mv, rv)->increase(old->rc, pos);
  }
  val = lc->val + rc->val;
}
unsigned int SegmentTree::countLess(const SegmentTree* old,
                                    const unsigned int v) const
{
  if (rv == v) return val - old->val;
  if (v <= mv)
    return lc->countLess(old->lc, v);
  else
    return (lc->val - old->lc->val) + rc->countLess(old->rc, v);
}
namespace LinkCutTree {
SplayNode* maybe(SplayNode* s)
{
  static SplayNode nil;
  return s ? s : &nil;
}
inline void SplayNode::pushRev()
{
  swap(chl[0], chl[1]);
  rev ^= 1;
}
inline void SplayNode::pushdown()
{
  if (!rev) return;
  maybe(chl[0])->pushRev();
  maybe(chl[1])->pushRev();
  rev = false;
}
inline void SplayNode::pushup()
{
  minVal = min({val, maybe(chl[0])->minVal, maybe(chl[1])->minVal});
}
inline bool SplayNode::isRoot()
{
  return fa ? fa->chl[0] != this && fa->chl[1] != this : true;
}
void SplayNode::pushPath()
{
  if (!isRoot()) fa->pushPath();
  pushdown();
}
void SplayNode::rotate()
{
  SplayNode *const f = fa, *const ff = fa->fa;
  if (!f->isRoot()) ff->chl[ff->chl[1] == f] = this;
  const bool typ = f->chl[1] == this;
  fa = ff;
  f->fa = this;
  if (chl[!typ]) chl[!typ]->fa = f;
  f->chl[typ] = chl[!typ];
  chl[!typ] = f;
  f->pushup();
  this->pushup();
}
void SplayNode::splay()
{
  pushPath();
  while (!isRoot()) {
    SplayNode* const ff = fa->fa;
    if (!fa->isRoot()) {
      if ((ff->chl[1] == fa) ^ (fa->chl[1] == this))
        this->rotate();
      else
        fa->rotate();
    }
    rotate();
  }
}

void access(SplayNode* x)
{
  for (SplayNode* lst = nullptr; x; x = x->fa) {
    x->splay();
    x->chl[1] = lst;
    x->pushup();
    lst = x;
  }
}
void makeRoot(SplayNode* const x)
{
  access(x);
  x->splay();
  x->pushRev();
}
void split(SplayNode* const x, SplayNode* const rt)
{
  makeRoot(x);
  access(rt);
  rt->splay();
}
SplayNode* findRoot(SplayNode* x)
{
  access(x);
  x->splay();
  while (x->chl[0])
    x = x->chl[0];
  x->splay();
  return x;
}
unsigned int minPath(SplayNode* const x, SplayNode* const y)
{
  split(x, y);
  return y->minVal;
}
void cut(const Edge& e)
{
  SplayNode *const px = vertex + e.from, *const py = vertex + e.to;
  split(px, py);
  px->fa = py->chl[0] = nullptr;
  px->chl[1] = nullptr;
  px->pushup();
  py->pushup();
}
void link(SplayNode* const px, SplayNode* const py, const unsigned int id)
{
  makeRoot(px);
  cur->val = cur->minVal = id;
  px->fa = cur;
  cur->fa = py;
  ++cur;
}
unsigned int addEdge(const unsigned int id)
{
  const Edge& e = ed[id];
  SplayNode *const px = vertex + e.from, *const py = vertex + e.to;
  unsigned int ret = 0;
  if (findRoot(px) == findRoot(py)) {
    ret = minPath(px, py);
    cut(ed[ret]);
    ++ret;
  }
  link(px, py, id);
  return ret;
}
}  // namespace LinkCutTree

static void init(const unsigned int m)
{
  using LinkCutTree::addEdge;
  root[0] = SegmentTree::cur;
  SegmentTree::create(0, m + 1)->build();
  for (unsigned int i = 1; i <= m; ++i) {
    if (ed[i].from == ed[i].to) {
      root[i] = root[i - 1];
      continue;
    }
    root[i] = SegmentTree::cur;
    SegmentTree::create(0, m + 1)->increase(root[i - 1], addEdge(i));
  }
}

int main()
{
#ifndef APTEST
  ios_base::sync_with_stdio(false);
#endif
  unsigned int n, m, k, typ, lst = 0;
  cin >> n >> m >> k >> typ;
  for (Edge* i = ed + 1; i < ed + m + 1; ++i)
    cin >> i->from >> i->to;
  init(m);
  for (unsigned int i = 0; i < k; ++i) {
    unsigned int l, r;
    cin >> l >> r;
    if (typ) {
      l ^= lst;
      r ^= lst;
    }
    cout << (lst = n - root[r]->countLess(root[l - 1], l + 1)) << "\n";
  }
  return 0;
}
#ifdef APTEST
#include "debug_tools/program.h"
#endif
#include <array>
#include <iostream>
using namespace std;
const unsigned int maxn = 2e5, maxm = 2e5, maxl = 16;

struct Node {
  unsigned int fa, size;
};
class SegmentTree {
 public:
  void build();
  inline SegmentTree* init(const unsigned int l, const unsigned int r,
                           const unsigned int sta);
  void assign(const SegmentTree* old, const unsigned int pos, const Node& val);
  Node get(const unsigned int idx) const;

  static SegmentTree* cur;

  unsigned int sta;

 private:
  union {
    SegmentTree* chl[2];
    Node val;
  } dat;
  unsigned int lv, rv;
} node[maxn * 4 + maxm * (maxl + 10) * 2];
SegmentTree* SegmentTree::cur = node;
const SegmentTree* root[maxm + 10];

void SegmentTree::build()
{
  if (rv - lv == 1) {
    this->dat.val = Node{lv, 1};
    return;
  }
  const unsigned int mv = (lv + rv) >> 1;
  dat.chl[0] = cur;
  (cur++)->init(lv, mv, 0)->build();
  dat.chl[1] = cur;
  (cur++)->init(mv, rv, 0)->build();
}
inline SegmentTree* SegmentTree::init(const unsigned int l,
                                      const unsigned int r,
                                      const unsigned int sta)
{
  lv = l;
  rv = r;
  this->sta = sta;
  return this;
}
void SegmentTree::assign(const SegmentTree* old, const unsigned int pos,
                         const Node& val)
{
  if (rv - lv == 1) {
    this->dat.val = val;
    return;
  }
  const unsigned int mv = (lv + rv) >> 1;
  const bool typ = pos >= mv;
  if (!dat.chl[!typ]) dat.chl[!typ] = old->dat.chl[!typ];
  if (pos < mv) {
    if (!dat.chl[0] || dat.chl[0]->sta != sta)
      dat.chl[0] = (cur++)->init(lv, mv, sta);
  }
  else {
    if (!dat.chl[1] || dat.chl[1]->sta != sta)
      dat.chl[1] = (cur++)->init(mv, rv, sta);
  }
  dat.chl[typ]->assign(old->dat.chl[typ], pos, val);
}
Node SegmentTree::get(const unsigned int pos) const
{
  if (rv - lv == 1) return dat.val;
  const unsigned int mv = (lv + rv) >> 1;
  return dat.chl[pos >= mv]->get(pos);
}
Node find(const SegmentTree* const root, unsigned int x)
{
  Node ret = root->get(x);
  do {
    x = ret.fa;
    ret = root->get(x);
  } while (ret.fa != x);
  return ret;
}
const SegmentTree* merge(const SegmentTree* const root, const unsigned int n,
                         const unsigned int x, const unsigned int y)
{
  Node a = find(root, x), b = find(root, y);
  if (a.fa == b.fa) return root;
  if (a.size > b.size) swap(a, b);
  SegmentTree* const t = (SegmentTree::cur++)->init(1, n + 1, root->sta + 1);
  t->assign(root, a.fa, Node{b.fa, a.size});
  t->assign(root, b.fa, Node{b.fa, a.size + b.size});
  return t;
}
int main()
{
#ifndef APTEST
  ios_base::sync_with_stdio(false);
#endif
  unsigned int n, m;
  cin >> n >> m;
  {
    SegmentTree* rt = SegmentTree::cur++;
    rt->init(1, n + 1, 0)->build();
    root[0] = rt;
  }
  for (unsigned int i = 1; i <= m; ++i) {
    unsigned int op, a, b;
    cin >> op;
    switch (op) {
      case 1:
        cin >> a >> b;
        root[i] = merge(root[i - 1], n, a, b);
        break;
      case 2:
        cin >> a;
        root[i] = root[a];
        break;
      case 3:
        root[i] = root[i - 1];
        cin >> a >> b;
        cout << (find(root[i], a).fa == find(root[i], b).fa) << "\n";
        break;
    }
  }
  return 0;
}
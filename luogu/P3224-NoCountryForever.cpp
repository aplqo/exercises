#ifdef APTEST
#include "debug_tools/judge.h"
#endif
#include <algorithm>
#include <iostream>
const unsigned int maxn = 1e5, maxNode = maxn * 4 * 40;

class SegmentTree {
 public:
  unsigned int kth(const unsigned int k);
  void merge(SegmentTree* const other);
  void increase(const unsigned int pos, const unsigned int from);

  inline static SegmentTree* create(const unsigned int l, const unsigned int r);
  static SegmentTree mem[maxNode + 10], *cur;

  unsigned int size;

 private:
  unsigned int lv, mv, rv;
  unsigned int from;
  SegmentTree *lc, *rc;
};
SegmentTree SegmentTree::mem[maxNode + 10],
    *SegmentTree::cur = SegmentTree::mem;
unsigned int p[maxn + 10];

unsigned int SegmentTree::kth(const unsigned int k)
{
  if (rv - lv == 1) return from;
  const unsigned int lsize = lc ? lc->size : 0;
  return k <= lsize ? lc->kth(k) : rc->kth(k - lsize);
}
void SegmentTree::merge(SegmentTree* const other)
{
  if (!other) return;
  size += other->size;
  if (rv - lv == 1) return;
  if (!lc)
    lc = other->lc;
  else
    lc->merge(other->lc);
  if (!rc)
    rc = other->rc;
  else
    rc->merge(other->rc);
}
void SegmentTree::increase(const unsigned int pos, const unsigned int from)
{
  ++size;
  if (rv - lv == 1) {
    this->from = from;
    return;
  }
  if (pos < mv) {
    lc = create(lv, mv);
    lc->increase(pos, from);
  }
  else {
    rc = create(mv, rv);
    rc->increase(pos, from);
  }
}
inline SegmentTree* SegmentTree::create(const unsigned int l,
                                        const unsigned int r)
{
  cur->lv = l;
  cur->rv = r;
  cur->mv = (l + r) >> 1;
  cur->size = 0;
  cur->lc = cur->rc = nullptr;
  return cur++;
}

namespace Disjoint {
struct Node {
  Node* fa;
  unsigned int size;
  SegmentTree* root;

  Node* find();
} nodes[maxn + 10];

static void init(const unsigned int n)
{
  unsigned int idx = 1;
  for (Node* i = nodes + 1; i < nodes + 1 + n; ++i, ++idx) {
    i->fa = i;
    i->size = 1;
    i->root = SegmentTree::create(1, n + 1);
    i->root->increase(p[idx], idx);
  }
}
inline Node* find(const unsigned int x) { return nodes[x].find(); }
void merge(unsigned int x, unsigned int y)
{
  Node *a = nodes[x].find(), *b = nodes[y].find();
  if (a == b) return;
  if (a->size < b->size) std::swap(a, b);
  b->fa = a;
  a->size += b->size;
  a->root->merge(b->root);
  b->root = nullptr;
}
Node* Node::find() { return fa == this ? this : fa = fa->find(); }
}  // namespace Disjoint

int main()
{
  std::ios::sync_with_stdio(false);
  std::cin.tie(nullptr);
  std::cout.tie(nullptr);
  unsigned int n, m;
  std::cin >> n >> m;
  std::copy_n(std::istream_iterator<unsigned int>(std::cin), n, p + 1);
  Disjoint::init(n);
  for (unsigned int i = m; i; --i) {
    unsigned int x, y;
    std::cin >> x >> y;
    Disjoint::merge(x, y);
  }
  unsigned int q;
  std::cin >> q;
  for (; q; --q) {
    char op;
    unsigned int x, y;
    std::cin >> op >> x >> y;
    if (op == 'B')
      Disjoint::merge(x, y);
    else {
      SegmentTree* root = Disjoint::find(x)->root;
      if (root->size < y)
        std::cout << "-1\n";
      else
        std::cout << root->kth(y) << "\n";
    }
  }
  return 0;
}
#ifdef APTEST
#include "debug_tools/judge.h"
#endif
#include <algorithm>
#include <iostream>
#include <iterator>
#include <numeric>
#include <set>
const unsigned int maxn = 1e5, maxm = 1e5, maxl = 17;
const unsigned int maxNode = maxn * maxl * 4;

enum Order { Increase = 0, Decrease = 1 };
class SegmentTree {
 public:
  inline void init(const unsigned int l, const unsigned int r,
                   const unsigned int cnt);
  void build(const unsigned int l, const unsigned int r,
             const unsigned int pos);
  void merge(SegmentTree* other);
  SegmentTree* split(const unsigned int size);
  unsigned int get(const unsigned int pos) const;
  template <Order ord>
  void print() const;

  static void init();
  static SegmentTree** top;

 private:
  unsigned int lv, mv, rv;
  SegmentTree *lc, *rc;
  unsigned int count;
};
SegmentTree** SegmentTree::top;
struct SortedInterval {
  unsigned int l;
  mutable unsigned int r;
  mutable Order order;
  mutable SegmentTree* root;

  inline bool operator<(const SortedInterval& rhs) const { return l < rhs.l; }
  std::pair<SegmentTree*, SegmentTree*> split(const unsigned int idx) const;
};
std::set<SortedInterval> interval;
unsigned int iv[maxn + 10];

inline void SegmentTree::init(const unsigned int l, const unsigned int r,
                              const unsigned int cnt)
{
  lv = l;
  mv = (l + r) >> 1;
  rv = r;
  lc = rc = nullptr;
  count = cnt;
}
void SegmentTree::build(const unsigned int l, const unsigned int r,
                        const unsigned int pos)
{
  init(l, r, 1);
  if (r - l == 1) return;
  if (pos < mv) {
    lc = *(--top);
    lc->build(l, mv, pos);
  }
  else {
    rc = *(--top);
    rc->build(mv, r, pos);
  }
}
void SegmentTree::merge(SegmentTree* other)
{
  if (!other) return;
  count += other->count;
  if (rv - lv > 1) {
    if (lc)
      lc->merge(other->lc);
    else
      lc = other->lc;
    if (rc)
      rc->merge(other->rc);
    else
      rc = other->rc;
  }
  *(top++) = other;
}
SegmentTree* SegmentTree::split(const unsigned int size)
{
  SegmentTree* ret = *(--top);
  ret->init(lv, rv, size);
  count -= size;
  if (lc && size <= lc->count) {
    if (lc->count == size) {
      ret->lc = lc;
      lc = nullptr;
    }
    else
      ret->lc = lc->split(size);
    return ret;
  }
  else {
    ret->lc = lc;
    ret->rc = rc->split(size - (lc ? lc->count : 0));
    lc = nullptr;
    return ret;
  }
}
unsigned int SegmentTree::get(const unsigned int pos) const
{
  if (rv - lv == 1) return lv;
  const unsigned int lsize = lc ? lc->count : 0;
  return lsize >= pos ? lc->get(pos) : rc->get(pos - lsize);
}
template <Order ord>
void SegmentTree::print() const
{
  if (rv - lv == 1) {
    std::cout << lv << " ";
    return;
  }
  const SegmentTree* seq[2]{lc, rc};
  if constexpr (ord == Order::Decrease) std::swap(seq[0], seq[1]);
  for (auto i : seq)
    if (i) i->print<ord>();
}
void SegmentTree::init()
{
  static SegmentTree mem[maxNode + 10], *free[maxNode + 10];
  std::iota(free, free + maxNode, mem);
  top = free + maxNode;
}

static void init(const unsigned int n)
{
  SegmentTree::init();
  for (unsigned int i = 1; i <= n; ++i) {
    SegmentTree* rt = *(--SegmentTree::top);
    rt->build(1, n + 1, iv[i]);
    interval.insert(SortedInterval{i, i + 1, Order::Increase, rt});
  }
}
std::pair<SegmentTree*, SegmentTree*> SortedInterval::split(
    const unsigned int pos) const
{
  if (order == Increase) {
    SegmentTree* tp = root->split(pos);
    return std::make_pair(tp, root);
  }
  else {
    SegmentTree* tp = root->split(r - l - pos);
    return std::make_pair(root, tp);
  }
}
void sort(const unsigned int l, const unsigned int r, const Order order)
{
  auto left = std::prev(interval.upper_bound(SortedInterval{l})),
       right = std::prev(interval.upper_bound(SortedInterval{r}));
  SegmentTree* root = nullptr;
  if (left == right) {
    SortedInterval si = *left;
    if (left->l == l && left->r == r) {
      left->order = order;
      return;
    }
    interval.erase(left);
    if (si.l < l) {
      const auto [p, n] = si.split(l - si.l);
      si.root = n;
      interval.insert(SortedInterval{si.l, l, si.order, p});
      si.l = l;
    }
    if (si.r > r) {
      const auto [p, n] = si.split(r - si.l);
      root = p;
      interval.insert(SortedInterval{r, si.r, si.order, n});
    }
    else
      root = si.root;
  }
  else {
    if (left->l != l) {
      const auto [p, n] = left->split(l - left->l);
      n->merge(root);
      root = n;
      left->root = p;
      left->r = l;
      ++left;
    }
    while (left != right && left->r <= r) {
      left->root->merge(root);
      root = left->root;
      left = interval.erase(left);
    }
    if (right->l < r) {
      if (right->r > r) {
        const auto [p, n] = right->split(r - right->l);
        p->merge(root);
        root = p;
        interval.insert(SortedInterval{
            r,
            right->r,
            right->order,
            n,
        });
      }
      else {
        right->root->merge(root);
        root = right->root;
      }
      interval.erase(right);
    }
  }
  interval.insert(SortedInterval{l, r, order, root});
}
unsigned int get(const unsigned int pos)
{
  auto it = std::prev(
      interval.upper_bound(SortedInterval{pos, 0, Order::Increase, nullptr}));
  if (it->order == Order::Increase)
    return it->root->get(pos + 1 - it->l);
  else
    return it->root->get(it->r - pos);
}
void print()
{
  for (const auto& i : interval)
    if (i.order == Order::Increase)
      i.root->print<Order::Increase>();
    else
      i.root->print<Order::Decrease>();
}

int main()
{
  std::ios::sync_with_stdio(false);
  std::cin.tie(nullptr);
  std::cout.tie(nullptr);
  unsigned int n, m;
  std::cin >> n >> m;
  std::copy_n(std::istream_iterator<unsigned int>(std::cin), n, iv + 1);
  init(n);
  for (unsigned int i = m; i; --i) {
    unsigned int op, l, r;
    std::cin >> op >> l >> r;
    sort(l, r + 1, static_cast<Order>(op));
  }
  unsigned int q;
  std::cin >> q;
  std::cout << get(q) << "\n";
  return 0;
}

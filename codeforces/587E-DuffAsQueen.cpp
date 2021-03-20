#ifdef APTEST
#include "debug_tools/judge.h"
#endif
#include <algorithm>
#include <cstring>
#include <iostream>
#include <iterator>
const int maxN = 2e5, maxLog = 29;

class LinearBasis {
 public:
  void init(const unsigned int x)
  {
    std::memset(val, 0, sizeof(val));
    insert(x);
  }
  void insert(unsigned int x)
  {
    for (int i = maxLog; x && i >= 0; --i) {
      if (!(x & (1u << i))) continue;
      if (val[i])
        x ^= val[i];
      else {
        for (int j = 0; j < i; ++j)
          if (x & (1u << j)) x ^= val[j];
        for (int j = i + 1; j <= maxLog; ++j)
          if (val[j] & (1u << i)) val[j] ^= x;
        val[i] = x;
        break;
      }
    }
  }
  void operator+=(const LinearBasis& r)
  {
    for (unsigned int i : r.val)
      insert(i);
  }
  unsigned int size() const
  {
    return maxLog + 1 - std::count(val, val + maxLog + 1, 0u);
  }

 private:
  unsigned int val[maxLog + 1];
};

template <class T>
struct SegmentTreeBase {
  int lv, mv, rv;
  T *lc, *rc;
};
template <class T>
concept SegmentTree = std::is_base_of_v<SegmentTreeBase<T>, T>&& requires(T* x)
{
  {x->pushup()};
}
&&requires(T& x, const unsigned int val[])
{
  {x.initLeaf(val)};
  {x.initNode()};
};

template <SegmentTree T>
void buildTree(T* x, const int l, const int r, const unsigned int val[],
               T*& cur)
{
  x->lv = l;
  x->mv = (l + r) >> 1;
  x->rv = r;
  x->initNode();
  if (r - l == 1) {
    x->initLeaf(val);
    return;
  }
  x->lc = cur++;
  buildTree(x->lc, l, x->mv, val, cur);
  x->rc = cur++;
  buildTree(x->rc, x->mv, r, val, cur);
  x->pushup();
}
template <SegmentTree T>
T* build(T seq[], const int len, const unsigned int val[])
{
  T *const root = seq, *cur = seq + 1;
  buildTree(root, 1, len + 1, val, cur);
  return root;
}

class Sequence : public SegmentTreeBase<Sequence> {
 public:
  void addXor(const int l, const int r, const unsigned int val)
  {
    if (l == lv && r == rv) {
      this->val ^= val;
      return;
    }
    if (r <= mv)
      lc->addXor(l, r, val);
    else if (l >= mv)
      rc->addXor(l, r, val);
    else {
      lc->addXor(l, mv, val);
      rc->addXor(mv, r, val);
    }
  }
  unsigned int get(const int pos) const
  {
    if (rv - lv == 1) return val;
    return (pos < mv ? lc : rc)->get(pos) ^ val;
  }

  template <SegmentTree T>
  friend void buildTree(T*, const int, const int, const unsigned int[], T*&);
  template <SegmentTree T>
  friend T* build(T[], const int, const unsigned int[]);

 private:
  void initNode() { val = 0; }
  void initLeaf(const unsigned int val[]) { this->val = val[lv]; }
  void pushup() {}

  unsigned int val;
} * sRoot;
class Difference : private SegmentTreeBase<Difference> {
 public:
  void addXor(const int pos, const unsigned int val)
  {
    if (rv - lv == 1) {
      this->val ^= val;
      this->basis.init(this->val);
      return;
    }
    (pos < mv ? lc : rc)->addXor(pos, val);
    pushup();
  }
  void mergeBasis(const int l, const int r, LinearBasis& dest) const
  {
    if (l == lv && r == rv) {
      dest += basis;
      return;
    }
    if (r <= mv)
      lc->mergeBasis(l, r, dest);
    else if (l >= mv)
      rc->mergeBasis(l, r, dest);
    else {
      lc->mergeBasis(l, mv, dest);
      rc->mergeBasis(mv, r, dest);
    }
  }

  template <SegmentTree T>
  friend void buildTree(T*, const int, const int, const unsigned int[], T*&);
  template <SegmentTree T>
  friend T* build(T[], const int, const unsigned int[]);

 private:
  void initNode() {}
  void initLeaf(const unsigned int val[])
  {
    this->val = val[lv] ^ val[lv - 1];
    this->basis.init(this->val);
  }
  void pushup()
  {
    basis = lc->basis;
    basis += rc->basis;
  }

  LinearBasis basis;
  unsigned int val;
} * dRoot;

static void init(const int n, const unsigned int val[])
{
  static Sequence seqNodes[maxN * 4 + 10];
  static Difference difNodes[maxN * 4 + 10];
  sRoot = build(seqNodes, n, val);
  dRoot = build(difNodes, n + 1, val);
}
void addXor(const int l, const int r, const unsigned int val)
{
  sRoot->addXor(l, r, val);
  dRoot->addXor(l, val);
  dRoot->addXor(r, val);
}
unsigned int countSum(const int l, const int r)
{
  LinearBasis lb;
  lb.init(sRoot->get(l));
  if (r - l != 1) dRoot->mergeBasis(l + 1, r, lb);
  return lb.size();
}

int main()
{
  std::ios::sync_with_stdio(false);
  std::cin.tie(nullptr);
  std::cout.tie(nullptr);
  int n, q;
  std::cin >> n >> q;
  static unsigned int a[maxN + 10];
  std::copy_n(std::istream_iterator<unsigned int>(std::cin), n, a + 1);
  init(n, a);
  for (int i = q; i; --i) {
    int typ, l, r;
    std::cin >> typ >> l >> r;
    if (typ == 1) {
      unsigned int k;
      std::cin >> k;
      addXor(l, r + 1, k);
    }
    else
      std::cout << (1 << countSum(l, r + 1)) << "\n";
  }
  return 0;
}

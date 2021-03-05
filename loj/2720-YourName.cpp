#ifdef APTEST
#include "debug_tools/judge.h"
#endif
#include <algorithm>
#include <cstring>
#include <fstream>
#include <iostream>
#include <iterator>
#include <numeric>
const int maxc = 26, maxs = 5e5, maxt = 1e6, maxStr = maxt, maxl = 20;
const int maxNode = maxs * maxl * 4;

class SegmentTree {
 public:
  void build(const int l, const int r, const int pos);
  bool contains(const int l, const int r) const;
  static const SegmentTree *merge(const SegmentTree *l, const SegmentTree *r);

  static SegmentTree nodes[maxNode], *cur;

 private:
  int lv, rv;
  const SegmentTree *lc, *rc;
};
SegmentTree SegmentTree::nodes[maxNode], *SegmentTree::cur = SegmentTree::nodes;

void SegmentTree::build(const int l, const int r, const int pos)
{
  lv = l;
  const int mv = (l + r) >> 1;
  rv = r;
  if (r - l == 1) {
    lc = rc = nullptr;
    return;
  }
  if (pos < mv) {
    lc = cur;
    (cur++)->build(l, mv, pos);
    rc = nullptr;
  }
  else {
    lc = nullptr;
    rc = cur;
    (cur++)->build(mv, r, pos);
  }
}
bool SegmentTree::contains(const int l, const int r) const
{
  const int mv = (lv + rv) >> 1;
  return (l == lv && r == rv) ||
         (lc && l < mv && lc->contains(l, std::min(mv, r))) ||
         (rc && mv < r && rc->contains(std::max(l, mv), r));
}
const SegmentTree *SegmentTree::merge(const SegmentTree *l,
                                      const SegmentTree *r)
{
  if (!l && !r) return nullptr;
  if (!l || !r) return l ? l : r;
  if (l->rv - l->lv == 1) return l;
  SegmentTree *ret = cur++;
  ret->lv = l->lv;
  ret->rv = l->rv;
  ret->lc = merge(l->lc, r->lc);
  ret->rc = merge(l->rc, r->rc);
  return ret;
}

template <class T>
struct SAM {
  struct Node {
    int len;
    Node *link, *next[maxc];
    T data;
  } nodes[maxStr + 10], *end, *root;

  void clear();
  Node *insertChar(Node *const last, const int c);
};
SAM<const SegmentTree *> orig;

template <class T>
void SAM<T>::clear()
{
  new (nodes) Node{};
  root = nodes;
  end = nodes + 1;
}
template <class T>
typename SAM<T>::Node *SAM<T>::insertChar(SAM<T>::Node *const last, const int c)
{
  Node *const ret = end++, *from = last;
  new (ret) Node{last->len + 1};
  while (from && !from->next[c]) {
    from->next[c] = ret;
    from = from->link;
  }
  if (!from)
    ret->link = root;
  else {
    Node *const fNxt = from->next[c];
    if (fNxt->len == from->len + 1)
      ret->link = fNxt;
    else {
      Node *const cloned = end++;
      *cloned = *fNxt;
      cloned->len = from->len + 1;
      for (Node *i = from; i && i->next[c] == fNxt; i = i->link)
        i->next[c] = cloned;
      fNxt->link = ret->link = cloned;
    }
  }
  return ret;
}

void commonSubstr(const char str[], const size_t len, const int l, const int r,
                  int dest[])
{
  const auto check = [l, r](decltype(orig.root) ptr, const int match) {
    return ptr && ptr->data && ptr->data->contains(l + match, r);
  };
  auto ptr = orig.root;
  int match = 0;
  for (size_t i = 0; i < len; ++i) {
    const int ch = str[i] - 'a';
    while (ptr != orig.root && !check(ptr->next[ch], match)) {
      --match;
      if (match == ptr->link->len) ptr = ptr->link;
    }
    if (check(ptr->next[ch], match)) {
      ++match;
      ptr = ptr->next[ch];
    }
    dest[i] = match;
  }
}
void topological()
{
  typedef SAM<const SegmentTree *>::Node Node;
  static Node *seq[maxs * 4 + 10];
  const int n = orig.end - orig.nodes - 1;
  std::iota(seq, seq + n, orig.nodes + 1);
  std::sort(seq, seq + n, [](Node *a, Node *b) { return a->len > b->len; });
  for (int i = 0; i < n; ++i) {
    Node *const cur = seq[i], *const to = seq[i]->link;
    to->data = SegmentTree::merge(to->data, cur->data);
  }
}
template <class T, class PostFunc>
void createSAM(SAM<T> &sam, const char str[], const size_t len, PostFunc fun)
{
  sam.clear();
  auto last = sam.root;
  for (size_t i = 0; i < len; ++i) {
    last = sam.insertChar(last, str[i] - 'a');
    fun(i, last);
  }
}
static void init(const char str[])
{
  const size_t len = std::strlen(str);
  createSAM(orig, str, len,
            [len](const size_t pos, SAM<const SegmentTree *>::Node *x) {
              x->data = SegmentTree::cur;
              (SegmentTree::cur++)->build(0, len, pos);
            });
  topological();
}
long long query(const char str[], const int l, const int r)
{
  struct Void {
  };
  static SAM<Void> sam;
  static int common[maxt + 10];
  const size_t len = std::strlen(str);

  commonSubstr(str, len, l, r, common);
  long long commonCount = 0, subCount = 0;
  createSAM(sam, str, len,
            [&commonCount](const size_t pos, SAM<Void>::Node *x) {
              if (common[pos] > x->link->len)
                commonCount += common[pos] - x->link->len;
            });
  for (auto i = sam.nodes + 1; i < sam.end; ++i)
    subCount += i->len - i->link->len;
  return subCount - commonCount;
}

int main()
{
  std::ios::sync_with_stdio(false);
#ifdef FileIO
  std::ifstream in("name.in");
  std::ofstream out("name.out");
#else
  std::istream &in = std::cin;
  std::ostream &out = std::cout;
#endif
  int q;
  static char buf[maxStr + 10];
  in >> buf;
  init(buf);
  in >> q;
  for (int i = q; i; --i) {
    int l, r;
    in >> buf >> l >> r;
    out << query(buf, l - 1, r) << "\n";
  }
  return 0;
}

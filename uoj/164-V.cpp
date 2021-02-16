#ifdef APTEST
#include "debug_tools/program.h"
#endif
#include <algorithm>
#include <cctype>
#include <cstdio>
#include <iostream>
#include <iterator>
#include <limits>
using namespace std;
const unsigned int maxn = 5e5, maxm = 5e5;
constexpr long long ninf = numeric_limits<long long>::min() / 3;

char inbuf[maxn * 20 + maxm * 20 + 1000],
    *inpos = inbuf, outbuf[maxm * 20 + 100], *outpos = outbuf;

unsigned int readUInt()
{
  unsigned int ret = 0;
  char c = *(inpos++);
  while (!isdigit(c))
    c = *(inpos++);
  while (isdigit(c)) {
    ret = ret * 10 + (c - '0');
    c = *(inpos++);
  }
  return ret;
}
inline void writeUInt(const unsigned long long v)
{
  outpos += sprintf(outpos, "%llu\n", v);
}

struct Tag {
  long long add, max;

  inline Tag operator+(const Tag r) const
  {
    return Tag{std::max(ninf, add + r.add), std::max(r.max, max + r.add)};
  }
  inline void update(const Tag r)
  {
    add = std::max(add, r.add);
    max = std::max(max, r.max);
  }
};
class SegmentTree {
 public:
  void build(const unsigned int l, const unsigned int r, const long long v[],
             SegmentTree*(&cur));
  inline void add(const unsigned int l, const unsigned int r, const long long v)
  {
    applyTag(l, r, {v, ninf});
  }
  inline void minus(const unsigned int l, const unsigned int r,
                    const long long v)
  {
    applyTag(l, r, Tag{-v, 0});
  }
  inline void assign(const unsigned int l, const unsigned int r,
                     const long long v)
  {
    applyTag(l, r, Tag{ninf, v});
  }
  inline long long get(const unsigned int pos)
  {
    return query<Operate::Max>(pos);
  }
  inline long long historyMax(const unsigned int pos)
  {
    return query<Operate::HistoryMax>(pos);
  }

 private:
  enum class Operate { Max, HistoryMax };
  unsigned int lv, mv, rv;
  bool dirty;
  SegmentTree *lc, *rc;
  union {
    Tag tag;
    long long val;
  } current, history;

  inline void push(const Tag cur, const Tag his);
  inline void pushdown();
  void applyTag(const unsigned int l, const unsigned int r, const Tag& t);
  template <Operate op>
  long long query(const unsigned int pos);
} nodes[maxn * 4 + 10], *root = nodes;

void SegmentTree::build(const unsigned int l, const unsigned int r,
                        const long long v[], SegmentTree*(&cur))
{
  lv = l;
  rv = r;
  mv = (l + r) >> 1;
  dirty = false;
  if (r - l == 1) {
    current.val = history.val = v[l];
    return;
  }
  else
    current.tag = history.tag = Tag{};
  lc = ++cur;
  lc->build(l, mv, v, cur);
  rc = ++cur;
  rc->build(mv, r, v, cur);
}
void SegmentTree::applyTag(const unsigned int l, const unsigned int r,
                           const Tag& t)
{
  if (rv - lv == 1) {
    current.val = std::max(current.val + t.add, t.max);
    history.val = std::max(history.val, current.val);
    return;
  }
  pushdown();
  if (l == lv && r == rv) {
    push(t, t);
    return;
  }
  if (r <= mv)
    lc->applyTag(l, r, t);
  else if (l >= mv)
    rc->applyTag(l, r, t);
  else {
    lc->applyTag(l, mv, t);
    rc->applyTag(mv, r, t);
  }
}
template <SegmentTree::Operate op>
long long SegmentTree::query(const unsigned int pos)
{
  if (rv - lv == 1) return op == Operate::Max ? current.val : history.val;
  pushdown();
  return (pos < mv ? lc : rc)->query<op>(pos);
}
inline void SegmentTree::push(const Tag cur, const Tag his)
{
  if (rv - lv == 1) {
    history.val = std::max({history.val, current.val + his.add, his.max});
    current.val = std::max(current.val + cur.add, cur.max);
    return;
  }
  history.tag.update(current.tag + his);
  current.tag = current.tag + cur;
  dirty = true;
}
inline void SegmentTree::pushdown()
{
  if (dirty) {
    lc->push(current.tag, history.tag);
    rc->push(current.tag, history.tag);
    current.tag = history.tag = Tag{};
    dirty = false;
  }
}

int main()
{
#ifndef APTEST
  ios_base::sync_with_stdio(false);
#endif
  cin.read(inbuf, sizeof(inbuf));
  const unsigned int n = readUInt(), m = readUInt();
  {
    static long long val[maxn + 10];
    generate_n(val + 1, n, readUInt);
    SegmentTree* c = nodes;
    root->build(1, n + 1, val, c);
  }
  for (unsigned int i = m; i; --i) {
    const unsigned int op = readUInt();
    if (op < 4) {
      const unsigned int l = readUInt(), r = readUInt(), v = readUInt();
      switch (op) {
        case 1:
          root->add(l, r + 1, v);
          break;
        case 2:
          root->minus(l, r + 1, v);
          break;
        case 3:
          root->assign(l, r + 1, v);
          break;
      }
    }
    else {
      const unsigned int x = readUInt();
      writeUInt(op == 4 ? root->get(x) : root->historyMax(x));
    }
  }
  cout.write(outbuf, outpos - outbuf);
  return 0;
}

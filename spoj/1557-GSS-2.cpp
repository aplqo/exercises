#ifdef APTEST
#include "debug_tools/program.h"
#endif
#include <algorithm>
#include <iostream>
#include <iterator>
#include <vector>
using namespace std;
const unsigned int maxn = 100000, maxq = 100000, maxv = 100000;

class SegmentTree {
 public:
  void build(const unsigned int l, const unsigned int r, SegmentTree*(&cur));
  void add(const unsigned int l, const unsigned int r, const int v);
  long long historyMax(const unsigned int l, const unsigned int r);

 private:
  unsigned int lv, mv, rv;
  SegmentTree *lc, *rc;
  struct Tag {
    long long add, max;
  } current, history;

  inline void push(const long long delt, const long long his);
  inline void pushdown();
  inline void pushup();
} nodes[maxn * 4 + 10], *root = nodes;
struct Query {
  unsigned int l;
  long long* ret;
};
vector<Query> qry[maxn + 10];
long long ans[maxq + 10];
int val[maxn + 10];

void SegmentTree::build(const unsigned int l, const unsigned int r,
                        SegmentTree*(&cur))
{
  lv = l;
  rv = r;
  mv = (l + r) >> 1;
  current = history = Tag{};
  if (r - l == 1) return;
  lc = ++cur;
  lc->build(l, mv, cur);
  rc = ++cur;
  rc->build(mv, r, cur);
}
void SegmentTree::add(const unsigned int l, const unsigned int r, const int v)
{
  if (rv - lv == 1) {
    current.add += v;
    current.max += v;
    history.max = max(history.max, current.max);
    return;
  }
  pushdown();
  if (l == lv && r == rv) {
    push(v, v);
    return;
  }
  if (r <= mv)
    lc->add(l, r, v);
  else if (l >= mv)
    rc->add(l, r, v);
  else {
    lc->add(l, mv, v);
    rc->add(mv, r, v);
  }
  pushup();
}
long long SegmentTree::historyMax(const unsigned int l, const unsigned int r)
{
  if (l == lv && r == rv) return history.max;
  pushdown();
  if (r <= mv)
    return lc->historyMax(l, r);
  else if (l >= mv)
    return rc->historyMax(l, r);
  else
    return std::max(lc->historyMax(l, mv), rc->historyMax(mv, r));
}
inline void SegmentTree::push(const long long delt, const long long his)
{
  history.add = max(history.add, current.add + his);
  history.max = max(history.max, current.max + his);
  current.add += delt;
  current.max += delt;
}
inline void SegmentTree::pushdown()
{
  if (current.add || history.add) {
    lc->push(current.add, history.add);
    rc->push(current.add, history.add);
    current.add = history.add = 0;
  }
}
inline void SegmentTree::pushup()
{
  current.max = max(lc->current.max, rc->current.max);
  history.max = max(lc->history.max, rc->history.max);
}

static void answerQuery(const unsigned int n)
{
  static unsigned int lst[maxv * 2 + 19], *const head = lst + maxv + 5;
  {
    SegmentTree* cur = nodes;
    root->build(1, n + 1, cur);
  }
  for (unsigned int i = 1; i <= n; ++i) {
    root->add(head[val[i]] + 1, i + 1, val[i]);
    head[val[i]] = i;
    for (auto j : qry[i])
      *j.ret = root->historyMax(j.l, i + 1);
  }
}
int main()
{
#ifndef APTEST
  ios_base::sync_with_stdio(false);
#endif
  unsigned int n, q;
  cin >> n;
  copy_n(istream_iterator<int>(cin), n, val + 1);
  cin >> q;
  for (unsigned int i = 0; i < q; ++i) {
    unsigned int l, r;
    cin >> l >> r;
    qry[r].push_back(Query{l, ans + i});
  }
  answerQuery(n);
  copy(ans, ans + q, ostream_iterator<long long>(cout, "\n"));
  return 0;
}
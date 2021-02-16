#ifdef APTEST
#include "debug_tools/program.h"
#endif
#include <algorithm>
#include <climits>
#include <iostream>
#include <queue>
using namespace std;
const unsigned int maxm = 1e5, maxl = 20;
constexpr unsigned long long inf = ULLONG_MAX / 2;

struct Plan {
  int l, r, t;
  int lx, ly, rx, ry;
  unsigned int cost;
  unsigned long long dist;
  bool vis;
} pl[maxm + 10];
struct Node {
  unsigned long long dist;
  Plan* ptr;
};
struct NodeLess {
  inline bool operator()(const Node l, const Node r) const
  {
    return l.dist != r.dist ? l.dist > r.dist : l.ptr < r.ptr;
  }
};
priority_queue<Node, vector<Node>, NodeLess> q;
class SegmentTree {
 public:
  void build(const unsigned int l, const unsigned int r, Plan*(&p),
             Plan* const ed, SegmentTree*(&cur));
  void updatePrefix(const Plan& p);

 private:
  unsigned int lv, mv, rv;
  SegmentTree *lc, *rc;
  Plan **begin, **end;
  static Plan *mem[maxm * maxl + 10], **cur;
} nodes[maxm * 2 * 4 + 10], *root = nodes;
Plan *SegmentTree::mem[maxm * maxl + 10], **SegmentTree::cur = SegmentTree::mem;

void SegmentTree::build(const unsigned int l, const unsigned int r, Plan*(&p),
                        Plan* const ed, SegmentTree*(&cur))
{
  static const auto pred = [](Plan* l, Plan* r) { return l->ly < r->ly; };
  lv = l;
  rv = r;
  mv = (l + r) >> 1;
  if (rv - lv == 1) {
    begin = SegmentTree::cur;
    while (p < ed && p->lx == l) {
      if (!p->vis) *(SegmentTree::cur++) = p;
      ++p;
    }
    end = SegmentTree::cur;
    sort(begin, end, pred);
    return;
  }
  lc = ++cur;
  lc->build(l, mv, p, ed, cur);
  rc = ++cur;
  rc->build(mv, r, p, ed, cur);
  begin = SegmentTree::cur;
  end = merge(lc->begin, lc->end, rc->begin, rc->end, begin, pred);
  SegmentTree::cur = end;
}
void SegmentTree::updatePrefix(const Plan& p)
{
  if (begin == end) return;
  if (rv < p.rx || rv - lv == 1) {
    while (begin < end && (*begin)->ly <= p.ry) {
      if (!(*begin)->vis) {
        Plan* const ptr = *begin;
        ptr->dist = p.dist + ptr->cost;
        ptr->vis = true;
        q.push(Node{ptr->dist, ptr});
      }
      ++begin;
    }
    return;
  }
  lc->updatePrefix(p);
  if (mv <= p.rx) rc->updatePrefix(p);
}

static void initPlan(const unsigned int m)
{
  for (Plan* i = pl; i < pl + m; ++i) {
    i->lx = i->l - i->t;
    i->ly = i->l + i->t;
    i->rx = i->r - i->t + 1;
    i->ry = i->r + i->t + 1;
    if (i->l == 1) {
      i->vis = true;
      i->dist = i->cost;
    }
    else {
      i->vis = false;
      i->dist = inf;
    }
  }
}
static int discretize(const unsigned int m)
{
  static int buf[maxm * 2 + 10];
  int* cur = buf;
  for (Plan* i = pl; i < pl + m; ++i) {
    *(cur++) = i->lx;
    *(cur++) = i->rx;
  }
  sort(buf, cur);
  cur = unique(buf, cur);
  for (Plan* i = pl; i < pl + m; ++i) {
    i->lx = lower_bound(buf, cur, i->lx) - buf + 1;
    i->rx = lower_bound(buf, cur, i->rx) - buf + 1;
  }
  return cur - buf + 1;
}

unsigned long long solve(const unsigned int m, const unsigned int n)
{
  initPlan(m);
  {
    SegmentTree* c = nodes;
    Plan* p = pl;
    sort(pl, pl + m, [](const Plan& l, const Plan& r) { return l.lx < r.lx; });
    const int dcnt = discretize(m);
    for (Plan* i = pl; i < pl + m; ++i)
      if (i->vis) q.push(Node{i->dist, i});
    root->build(0, dcnt, p, pl + m, c);
  }
  unsigned long long ret = inf;
  while (!q.empty()) {
    const Plan* const cur = q.top().ptr;
    q.pop();
    root->updatePrefix(*cur);
    if (cur->r == n) ret = min(ret, cur->dist);
  }
  return ret;
}

int main()
{
#ifndef APTEST
  ios::sync_with_stdio(false);
#endif
  unsigned int n, m;
  cin >> n >> m;
  for (Plan* i = pl; i < pl + m; ++i)
    cin >> i->t >> i->l >> i->r >> i->cost;
  const unsigned long long v = solve(m, n);
  if (v == inf)
    cout << "-1\n";
  else
    cout << v << "\n";
  return 0;
}

#ifdef APTEST
#include "debug_tools/judge.h"
#endif
#include <algorithm>
#include <iostream>
#include <numeric>
#include <stack>
#include <unordered_map>
#include <vector>
const int maxN = 100000, maxQ = 100000;

struct Edge {
  int x, y;
  inline bool operator==(const Edge r) const { return x == r.x && y == r.y; }
};
struct EdgeHasher {
  inline unsigned long long operator()(const Edge e) const
  {
    return (static_cast<unsigned long long>(e.x) << 32) | e.y;
  }
};
namespace Disjoint {
int fa[maxN + 10], size[maxN + 10], distance[maxN + 10];
std::stack<int> stack;

static void init(const int n)
{
  std::iota(fa, fa + n + 1, 0);
  std::fill(size, size + n + 1, 1);
}
std::pair<int, int> find(int x)
{
  int ret = 0;
  while (fa[x] != x) {
    ret ^= distance[x];
    x = fa[x];
  }
  return std::make_pair(x, ret);
}
bool merge(const int x, const int y)
{
  auto [fx, dx] = find(x);
  auto [fy, dy] = find(y);
  if (fx == fy) return dx ^ dy;
  if (size[fx] < size[fy]) std::swap(fx, fy);
  stack.push(fy);
  fa[fy] = fx;
  size[fx] += size[fy];
  distance[fy] = dx ^ dy ^ 1;
  return true;
}
void revert(const size_t s)
{
  while (stack.size() > s) {
    const int cur = stack.top();
    stack.pop();
    size[fa[cur]] -= size[cur];
    fa[cur] = cur;
    distance[cur] = 0;
  }
}

class State {
 public:
  State() : s(stack.size()) {}
  ~State() { revert(s); }

 private:
  const size_t s;
};
}  // namespace Disjoint
bool answer[maxQ + 10];
class SegmentTree {
 public:
  void build(const int l, const int r, SegmentTree*& cur);
  void add(const int l, const int r, const Edge e);
  void dfs() const;

 private:
  int lv, mv, rv;
  SegmentTree *lc, *rc;
  std::vector<Edge> edge;
} nodes[maxQ * 4 + 10], root;

void SegmentTree::build(const int l, const int r, SegmentTree*& cur)
{
  lv = l;
  mv = (l + r) >> 1;
  rv = r;
  if (r - l == 1) return;
  lc = cur++;
  lc->build(l, mv, cur);
  rc = cur++;
  rc->build(mv, r, cur);
}
void SegmentTree::add(const int l, const int r, const Edge e)
{
  if (l == lv && r == rv) {
    edge.push_back(e);
    return;
  }
  if (r <= mv)
    lc->add(l, r, e);
  else if (l >= mv)
    rc->add(l, r, e);
  else {
    lc->add(l, mv, e);
    rc->add(mv, r, e);
  }
}
void SegmentTree::dfs() const
{
  const Disjoint::State ver;
  for (auto i : edge)
    if (!Disjoint::merge(i.x, i.y)) {
      std::fill(answer + lv, answer + rv, false);
      return;
    }
  if (rv - lv == 1)
    answer[lv] = true;
  else {
    lc->dfs();
    rc->dfs();
  }
}

static void init(const int n, const int q)
{
  Disjoint::init(n);
  SegmentTree* cur = nodes;
  root.build(0, q, cur);
}
void readQuery(const int q)
{
  std::unordered_map<Edge, int, EdgeHasher> edge;
  for (int i = 0; i < q; ++i) {
    Edge e;
    std::cin >> e.x >> e.y;
    if (e.x > e.y) std::swap(e.x, e.y);
    if (auto it = edge.find(e); it != edge.end()) {
      root.add(it->second, i, e);
      edge.erase(it);
    }
    else
      edge.insert({e, i});
  }
  for (auto [e, l] : edge)
    root.add(l, q, e);
}

int main()
{
  std::ios::sync_with_stdio(false);
  int n, q;
  std::cin >> n >> q;
  init(n, q);
  readQuery(q);
  root.dfs();
  for (const bool* i = answer; i < answer + q; ++i)
    std::cout << (*i ? "YES" : "NO") << "\n";
  return 0;
}
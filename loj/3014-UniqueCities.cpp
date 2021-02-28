#ifdef APTEST
#include "debug_tools/judge.h"
#endif
#include <algorithm>
#include <iostream>
#include <iterator>
const int maxn = 2e5;

struct Edge {
  int to;
  const Edge* pre;
} ed[maxn * 2 + 10];
const Edge* head[maxn + 10];

struct ValFrom {
  int val, from;

  inline bool operator<(const ValFrom& r) const { return val < r.val; }
  inline ValFrom operator+(const int d) const { return ValFrom{val + d, from}; }
};
struct Top2Val {
  ValFrom first, second;

  inline void update(const ValFrom x)
  {
    if (x.val > first.val) {
      second = first;
      first = x;
    }
    else
      second = std::max(second, x);
  }
};
Top2Val chain[maxn + 10];
int ans[maxn + 10], color[maxn + 10];
int stk[maxn + 10], *top = stk;
int dep[maxn + 10], count[maxn + 10], type;

inline void addEdge(const int from, const int to)
{
  static Edge* cur = ed;
  new (cur) Edge{to, head[from]};
  head[from] = cur++;
  new (cur) Edge{from, head[to]};
  head[to] = cur++;
}

ValFrom dfsFathest(const int x, const int from)
{
  ValFrom ret{0, x};
  for (const Edge* i = head[x]; i; i = i->pre)
    if (i->to != from) ret = std::max(ret, dfsFathest(i->to, x) + 1);
  return ret;
}
std::pair<int, int> findDiameter()
{
  const ValFrom p1 = dfsFathest(1, 0);
  return std::make_pair(p1.from, dfsFathest(p1.from, 0).from);
}

void dfsChain(const int x, const int f)
{
  chain[x] = Top2Val{};
  for (const Edge* i = head[x]; i; i = i->pre) {
    const int to = i->to;
    if (to == f) continue;
    dfsChain(to, x);
    chain[x].update(ValFrom{chain[to].first.val + 1, to});
  }
}

inline void popVertex()
{
  --top;
  --count[color[*top]];
  if (!count[color[*top]]) --type;
}
inline void pushVertex(const int x)
{
  *(top++) = x;
  if (!count[color[x]]) ++type;
  ++count[color[x]];
}
inline void removeNear(const int x, const int dist)
{
  while (top > stk && dep[x] - dep[*(top - 1)] <= dist)
    popVertex();
}
void dfs(const int x, const int f)
{
  dep[x] = dep[f] + 1;

  if (chain[x].first.from) {
    removeNear(x, chain[x].second.val);

    pushVertex(x);
    dfs(chain[x].first.from, x);
    removeNear(x, chain[x].first.val);

    ans[x] = std::max(ans[x], type);

    if (chain[x].second.from)
      for (const Edge* i = head[x]; i; i = i->pre) {
        pushVertex(x);
        if (i->to != f && i->to != chain[x].first.from) dfs(i->to, x);
        if (*(top - 1) == x) popVertex();
      }
  }
  else
    ans[x] = std::max(ans[x], type);
}
void solve(const int root)
{
  dfsChain(root, 0);
  dfs(root, 0);
}

int main()
{
  std::ios::sync_with_stdio(false);
  int n, m;
  std::cin >> n >> m;
  for (int i = 1; i < n; ++i) {
    int a, b;
    std::cin >> a >> b;
    addEdge(a, b);
  }
  std::copy_n(std::istream_iterator<int>(std::cin), n, color + 1);
  const auto [d1, d2] = findDiameter();
  solve(d1);
  solve(d2);
  std::copy(ans + 1, ans + 1 + n, std::ostream_iterator<int>(std::cout, "\n"));
  return 0;
}
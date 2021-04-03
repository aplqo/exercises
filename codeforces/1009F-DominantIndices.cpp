#ifdef APTEST
#include "debug_tools/judge.h"
#endif
#include <algorithm>
#include <iostream>
#include <iterator>
const int maxN = 1e6;

struct Edge {
  int to;
  const Edge *pre;
} ed[maxN * 2 + 10];
const Edge *head[maxN + 10];
int answer[maxN + 10], son[maxN + 10];
int count[maxN * 2 + 10], *freePtr = count;
struct ValFrom {
  int val, from;

  inline ValFrom operator+(const int v) const { return ValFrom{val, from + v}; }
  inline void update(const ValFrom v)
  {
    if (val < v.val)
      *this = v;
    else if (val == v.val && from > v.from)
      from = v.from;
  }
} ans;

inline void addEdge(const int from, const int to)
{
  static Edge *cur = ed;
  new (cur) Edge{to, head[from]};
  head[from] = cur++;
  new (cur) Edge{from, head[to]};
  head[to] = cur++;
}
int dfsSon(const int x, const int f)
{
  int ret = 0;
  for (const Edge *i = head[x]; i; i = i->pre) {
    const int to = i->to;
    if (to == f) continue;
    const int v = dfsSon(to, x);
    if (v > ret) {
      ret = v;
      son[x] = to;
    }
  }
  return ret + 1;
}
void merge(int *const dest, ValFrom &vf, const int *const from, const int len)
{
  for (int i = 0; i < len; ++i) {
    dest[i + 1] += from[i];
    vf.update(ValFrom{dest[i + 1], i + 1});
  }
}
std::pair<ValFrom, int> dfs(const int x, const int f, int *const ptr)
{
  ++freePtr;
  ValFrom ret{1, 0};
  int len = 0;
  ptr[0] = 1;
  if (son[x]) {
    const auto [val, dep] = dfs(son[x], x, ptr + 1);
    len = dep;
    ret.update(val + 1);
  }
  for (const Edge *i = head[x]; i; i = i->pre)
    if (i->to != f && i->to != son[x]) {
      int *const old = freePtr;
      const int v = dfs(i->to, x, freePtr).second;
      merge(ptr, ret, old, v);
      freePtr = old;
    }
  answer[x] = ret.from;
  return std::make_pair(ret, len + 1);
}

int main()
{
  std::ios::sync_with_stdio(false);
  int n;
  std::cin >> n;
  for (int i = 1; i < n; ++i) {
    int x, y;
    std::cin >> x >> y;
    addEdge(x, y);
  }
  dfsSon(1, 0);
  dfs(1, 0, freePtr);
  std::copy(answer + 1, answer + n + 1,
            std::ostream_iterator<int>(std::cout, "\n"));
  return 0;
}
#ifdef APTEST
#include "debug_tools/judge.h"
#endif
#include <algorithm>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <iterator>
const int maxn = 1e5;
const int maxv = maxn * 2 + 1, maxe = maxv * 2;

class Tree {
 public:
  struct Edge {
    int to;
    const Edge *pre;
  };
  const Edge *head[maxn + 10];

  inline void addEdge(const int from, const int to)
  {
    new (cur) Edge{to, head[from]};
    head[from] = cur++;
  }

 private:
  Edge ed[maxn + 10], *cur = ed;
} tree[2];
enum class Visit { NotVisit, RevVisit, Visited };
class Graph {
 public:
  struct Edge {
    int to;
    mutable Visit vis;
    const Edge *pre, *rev;
  };
  const Edge *head[maxv + 10];

  inline void addEdge(const int from, const int to)
  {
    new (cur) Edge{to, Visit::NotVisit, head[from], cur + 1};
    head[from] = cur++;
    new (cur) Edge{from, Visit::NotVisit, head[to], cur - 1};
    head[to] = cur++;
  }

 private:
  Edge ed[maxe * 2 + 10], *cur = ed;
} g;
bool parity[maxn + 10];
int val[maxn + 10];

template <int which>
void dfsParity(const int x)
{
  int cnt = 0;
  for (auto i = tree[which].head[x]; i; i = i->pre) {
    dfsParity<which>(i->to);
    ++cnt;
  }
  const bool p = !(cnt & 0x01);
  if constexpr (which == 1) {
    if (p != parity[x]) {
      std::cout << "IMPOSSIBLE\n";
      std::exit(0);
    }
  }
  else
    parity[x] = p;
}
const Graph::Edge *cur[maxv + 10];
void dfsEular(const int x)
{
  while (cur[x]) {
    if (cur[x]->vis == Visit::NotVisit) {
      cur[x]->vis = Visit::Visited;
      cur[x]->rev->vis = Visit::RevVisit;
      const int nxt = cur[x]->to;
      cur[x] = cur[x]->pre;
      dfsEular(nxt);
    }
    else
      cur[x] = cur[x]->pre;
  }
}
void findEular(const int n)
{
  for (int i = 0; i < n; ++i)
    if (parity[i]) g.addEdge(i, i + n);
  std::memcpy(cur, g.head, sizeof(g.head[0]) * (n * 2 + 1));
  dfsEular(0);
}
void construct(const int n)
{
  for (int i = 0; i < n; ++i)
    if (parity[i])
      for (auto j = g.head[i]; j; j = j->pre)
        if (j->to >= n) {
          val[i] = j->vis == Visit::Visited ? 1 : -1;
          break;
        }
}
template <bool which>
int readTree(const int n)
{
  int root, gid = which ? n : 0;
  for (int i = 0; i < n; ++i, ++gid) {
    int p;
    std::cin >> p;
    if (p == -1) {
      root = i;
      g.addEdge(n * 2, gid);
    }
    else {
      g.addEdge(which ? p - 1 + n : p - 1, gid);
      tree[which].addEdge(p - 1, i);
    }
  }
  return root;
}

int main()
{
  std::ios::sync_with_stdio(false);
  int n;
  std::cin >> n;
  const int rootA = readTree<0>(n), rootB = readTree<1>(n);
  dfsParity<0>(rootA);
  dfsParity<1>(rootB);
  findEular(n);
  construct(n);
  std::cout << "POSSIBLE\n";
  std::copy(val, val + n, std::ostream_iterator<int>(std::cout, " "));
  std::cout.put('\n');
  return 0;
}
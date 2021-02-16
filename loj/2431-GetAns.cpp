#include <algorithm>
#include <fstream>
#include <iostream>
using namespace std;
const unsigned int maxn = 5000;

struct Edge {
  unsigned int to;
  const Edge* pre;
} ed[maxn * 2 + 19];
const Edge* head[maxn + 1];
unsigned int dep[maxn + 1], fa[maxn + 1], top[maxn + 1], son[maxn + 1];

inline void addEdge(const unsigned int from, const unsigned int to)
{
  static Edge* cur = ed;
  new (cur) Edge{to, head[from]};
  head[from] = cur++;
  new (cur) Edge{from, head[to]};
  head[to] = cur++;
}
unsigned int dfsSon(const unsigned int x, const unsigned int f)
{
  fa[x] = f;
  dep[x] = dep[f] + 1;
  unsigned int ret = 1, ms = 0;
  for (const Edge* i = head[x]; i; i = i->pre) {
    const unsigned int to = i->to;
    if (to == f) continue;
    const unsigned int sz = dfsSon(to, x);
    ret += sz;
    if (sz > ms) {
      ms = sz;
      son[x] = to;
    }
  }
  return ret;
}
void dfsChain(const unsigned int x, const unsigned int tp)
{
  top[x] = tp;
  if (son[x]) dfsChain(son[x], tp);
  for (const Edge* i = head[x]; i; i = i->pre)
    if (!top[i->to]) dfsChain(i->to, i->to);
}
unsigned int lca(unsigned int a, unsigned int b)
{
  while (top[a] != top[b]) {
    if (dep[top[a]] > dep[top[b]]) swap(a, b);
    b = fa[top[b]];
  }
  return dep[a] < dep[b] ? a : b;
}
unsigned int distance(const unsigned int a, const unsigned int b)
{
  return dep[a] + dep[b] - 2 * dep[lca(a, b)];
}
unsigned long long count(const unsigned int n)
{
  unsigned long long ret = 0;
  for (unsigned int i = 1; i <= n; ++i)
    for (unsigned int j = i + 1; j <= n; ++j) {
      const unsigned int dist = distance(i, j);
      for (unsigned int k = j + 1; k <= n; ++k)
        if (distance(i, k) == dist && distance(k, j) == dist) {
          ++ret;
#ifdef PrintPair
          clog << i << " " << j << " " << k << "\n";
#endif
        }
    }
  return ret;
}
int main(int, char* argv[])
{
  ifstream in(argv[1]);
  ofstream ans(argv[2]);
  unsigned int n;
  in >> n;
  for (unsigned int i = 1; i < n; ++i) {
    unsigned int a, b;
    in >> a >> b;
    addEdge(a, b);
  }
  dfsSon(1, 0);
  dfsChain(1, 1);
  ans << count(n) << "\n";
  return 0;
}
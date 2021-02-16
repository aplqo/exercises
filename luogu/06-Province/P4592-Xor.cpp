#ifdef APTEST
#include "debug_tools/program.h"
#endif
#include <algorithm>
#include <array>
#include <iostream>
#include <iterator>
using namespace std;
const unsigned int maxn = 1e5, maxl = 30, maxv = 2 * maxn * (maxl + 2);

struct Edge {
  unsigned int to;
  const Edge* pre;
} ed[maxn * 2 + 10];
const Edge* head[maxn + 10];
unsigned int dfn[maxn + 10], siz[maxn + 10];
unsigned int val[maxn + 10];

inline void addEdge(const unsigned int from, const unsigned int to)
{
  static Edge* cur = ed;
  new (cur) Edge{to, head[from]};
  head[from] = cur++;
  new (cur) Edge{from, head[to]};
  head[to] = cur++;
}
namespace Trie {
struct Node {
  const Node* chl[2];
  unsigned int siz;
} nodes[maxv + 10], *cur = nodes;
const Node nil{{&nil, &nil}, 0};
typedef const Node* Root;
typedef array<Root, 2> RootList;

Root insert(Root old, const unsigned int val)
{
  Root const rt = ++cur;
  Node* pos = cur;
  for (unsigned int msk = 1u << maxl; msk; msk >>= 1) {
    const bool dir = val & msk;
    pos->chl[!dir] = old->chl[!dir];
    pos->chl[dir] = ++cur;
    pos->siz = old->siz + 1;
    old = old->chl[dir];
    pos = cur;
  }
  pos->siz = old->siz + 1;
  return rt;
}
unsigned int maxXor(RootList olds, RootList cur, const unsigned int val)
{
  static const auto subtree = [](const RootList& olds, const RootList& cur,
                                 const unsigned int cl) {
    return cur[0]->chl[cl]->siz + cur[1]->chl[cl]->siz - olds[0]->chl[cl]->siz -
           olds[1]->chl[cl]->siz;
  };
  unsigned int ret = 0;
  for (unsigned int msk = 1u << maxl; msk; msk >>= 1) {
    bool dir = val & msk;
    if (subtree(olds, cur, !dir)) {
      ret |= msk;
      dir = !dir;
    }
    olds[0] = olds[0]->chl[dir];
    olds[1] = olds[1]->chl[dir];
    cur[0] = cur[0]->chl[dir];
    cur[1] = cur[1]->chl[dir];
  }
  return ret;
}
inline unsigned int maxXor(Root old, Root cur, const unsigned int val)
{
  return maxXor(RootList{old, &nil}, RootList{cur, &nil}, val);
}
}  // namespace Trie
Trie::Root dfnRoots[maxn + 10], dfsRoots[maxn + 10];
namespace Lca {
unsigned int dep[maxn + 10], fa[maxn + 10], top[maxn + 10], son[maxn + 10];

void dfsSon(const unsigned int x, const unsigned int f)
{
  siz[x] = 1;
  dep[x] = dep[f] + 1;
  fa[x] = f;
  unsigned int ms = 0;
  for (const Edge* i = head[x]; i; i = i->pre) {
    const unsigned int to = i->to;
    if (to == f) continue;
    dfsSon(to, x);
    siz[x] += siz[to];
    if (siz[to] > ms) {
      ms = siz[to];
      son[x] = to;
    }
  }
}
void dfsChain(const unsigned int x, const unsigned int tp)
{
  static unsigned int sta = 0;
  top[x] = tp;
  dfn[x] = ++sta;
  dfnRoots[dfn[x]] = Trie::insert(dfnRoots[dfn[x] - 1], val[x]);
  if (son[x]) {
    dfsRoots[son[x]] = Trie::insert(dfsRoots[x], val[son[x]]);
    dfsChain(son[x], tp);
  }
  for (const Edge* i = head[x]; i; i = i->pre)
    if (!dfn[i->to]) {
      dfsRoots[i->to] = Trie::insert(dfsRoots[x], val[i->to]);
      dfsChain(i->to, i->to);
    }
}
static void init()
{
  dfnRoots[0] = dfsRoots[0] = &Trie::nil;
  dfsRoots[1] = Trie::insert(dfsRoots[0], val[1]);
  dfsSon(1, 0);
  dfsChain(1, 1);
}
unsigned int lca(unsigned int a, unsigned int b)
{
  while (top[a] != top[b]) {
    if (dep[top[a]] > dep[top[b]]) swap(a, b);
    b = fa[top[b]];
  }
  return dep[a] < dep[b] ? a : b;
}
}  // namespace Lca

int main()
{
#ifndef APTEST
  ios_base::sync_with_stdio(false);
#endif
  unsigned int n, q;
  cin >> n >> q;
  copy_n(istream_iterator<unsigned int>(cin), n, val + 1);
  for (unsigned int i = 1; i < n; ++i) {
    unsigned int x, y;
    cin >> x >> y;
    addEdge(x, y);
  }
  Lca::init();
  for (unsigned int i = 0; i < q; ++i) {
    using namespace Trie;
    unsigned int op, x, y;
    cin >> op >> x >> y;
    if (op == 1)
      cout << maxXor(dfnRoots[dfn[x] - 1], dfnRoots[dfn[x] + siz[x] - 1], y)
           << "\n";
    else {
      unsigned int z;
      cin >> z;
      const unsigned int l = Lca::lca(x, y);
      cout << maxXor(RootList{dfsRoots[l], dfsRoots[Lca::fa[l]]},
                     RootList{dfsRoots[x], dfsRoots[y]}, z)
           << "\n";
    }
  }
  return 0;
}
#ifdef APTEST
#include "debug_tools/program.h"
#endif
#include <climits>
#include <cstring>
#include <functional>
#include <iostream>
#include <limits>
#include <queue>
#include <type_traits>
#include <vector>
using namespace std;
const unsigned int maxn = 5e4, maxm = 5e4, maxk = 2e4;
typedef unsigned long long distance_t;
constexpr distance_t inf = numeric_limits<distance_t>::max() / 2;
constexpr unsigned int maxv = maxm * 4 + maxn + 10,
                       maxe = 2 * maxm * (4 + 3 * 2) + maxn;

namespace Trie {
struct Edge {
  unsigned int to;
  const Edge* pre;
} ed[maxk + 10], *cur = ed;
const Edge* head[maxk + 10];
unsigned int dfn[maxk + 10], dep[maxk + 10], sta = 0;
unsigned int fa[maxk + 10], top[maxk + 10], son[maxk + 10];

inline void addEdge(const unsigned int from, const unsigned int to)
{
  new (cur) Edge{to, head[from]};
  head[from] = cur++;
}
unsigned int dfsSon(const unsigned int x)
{
  unsigned int ms = 0, ret = 1;
  for (const Edge* i = head[x]; i; i = i->pre) {
    const unsigned int to = i->to;
    dep[to] = dep[x] + 1;
    fa[to] = x;
    const unsigned int sz = dfsSon(to);
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
  dfn[x] = ++sta;
  if (son[x]) dfsChain(son[x], tp);
  for (const Edge* i = head[x]; i; i = i->pre)
    if (!dfn[i->to]) dfsChain(i->to, i->to);
}
template <unsigned int p>
static void init()
{
  dfsSon(p);
  dfsChain(p, p);
}
inline void clear()
{
  sta = 0;
  cur = ed;
  memset(dfn, 0, sizeof(dfn));
  memset(head, 0, sizeof(head));
  memset(son, 0, sizeof(son));
}
unsigned int lca(unsigned int a, unsigned int b)
{
  while (top[a] != top[b]) {
    if (dep[top[a]] > dep[top[b]]) swap(a, b);
    b = fa[top[b]];
  }
  return dep[a] < dep[b] ? a : b;
}
}  // namespace Trie
namespace Rebuild {
struct Edge {
  unsigned int to, val;
  const Edge* pre;
} ed[maxe + 10], *cur = ed;
const Edge* head[maxv + 10];
distance_t dis[maxv + 10];
unsigned int vcur, endp[maxn + 10];

inline void addEdge(const unsigned int from, const unsigned int to,
                    const unsigned int val)
{
  new (cur) Edge{to, val, head[from]};
  head[from] = cur++;
}
void dijkstra(const unsigned int s)
{
  typedef pair<unsigned int, unsigned int> Pair;
  static bool vis[maxv + 10];
  priority_queue<Pair, vector<Pair>, greater<Pair>> q;
  memset(vis, 0, sizeof(vis));
  fill(dis, dis + vcur + 1, inf);
  dis[s] = 0;
  q.emplace(0, s);
  while (!q.empty()) {
    const Pair cur = q.top();
    q.pop();
    if (vis[cur.second]) continue;
    vis[cur.second] = true;
    for (const Edge* i = head[cur.second]; i; i = i->pre)
      if (dis[i->to] > cur.first + i->val) {
        dis[i->to] = cur.first + i->val;
        q.emplace(dis[i->to], i->to);
      }
  }
}
inline void clear()
{
  memset(head, 0, sizeof(head));
  vcur = 0;
  cur = ed;
}
}  // namespace Rebuild

namespace Orig {
enum type { vFrom = 0, vTo = 1 };
enum direct {
  toLeft = 0,  // from right to left
  toRight = 1  // from left to right
};
struct Edge {
  unsigned int from, to, word, val;
  unsigned int virt[2][2];
} ed[maxm + 10], *cur = ed;
typedef vector<const Edge*> EdgeList;
EdgeList inEdge[maxn + 10], outEdge[maxn + 10];

inline void addEdge(const unsigned int from, const unsigned int to,
                    const unsigned int w, const unsigned int val)
{
  using Rebuild::vcur;
  new (cur) Edge{from, to, w, val, {}};
  for (unsigned int& i : cur->virt[vFrom])
    i = ++vcur;
  for (unsigned int& i : cur->virt[vTo])
    i = ++vcur;
  for (unsigned int f : cur->virt[vFrom])
    for (unsigned int t : cur->virt[vTo])
      Rebuild::addEdge(f, t, val);
  inEdge[to].push_back(cur);
  outEdge[from].push_back(cur);
  ++cur;
}
template <type typ>
void linkVirt(const EdgeList& elist)
{
  using Rebuild::addEdge;
  if (elist.empty()) return;
  for (auto it = elist.cbegin(); it + 1 != elist.cend(); ++it)
    addEdge((*it)->virt[typ][toRight], (*(it + 1))->virt[typ][toRight], 0);
  for (auto it = elist.crbegin(); it + 1 != elist.crend(); ++it)
    addEdge((*it)->virt[typ][toLeft], (*(it + 1))->virt[typ][toLeft], 0);
}
void linkInOut(const EdgeList& in, const EdgeList& out)
{
  using Rebuild::addEdge;
  using Trie::dep, Trie::dfn, Trie::lca;
  if (in.empty() || out.empty()) return;
  const Edge *pin = nullptr, *pout = nullptr, *plast = nullptr;
  auto itIn = in.cbegin(), itOut = out.cbegin();
  if (dfn[(*itIn)->word] < dfn[(*itOut)->word])
    pin = plast = *(itIn++);
  else
    pout = plast = *(itOut++);
  while (itIn != in.cend() || itOut != out.cend()) {
    if (itOut == out.cend() ||
        (itIn != in.cend() && dfn[(*itIn)->word] < dfn[(*itOut)->word])) {
      const Edge* x = *(itIn++);
      const unsigned int h = dep[lca(x->word, plast->word)];
      if (pout) addEdge(x->virt[vTo][toLeft], pout->virt[vFrom][toLeft], h);
      if (pin && itOut != out.cend())
        addEdge(pin->virt[vTo][toRight], (*itOut)->virt[vFrom][toRight], h);
      plast = pin = x;
    }
    else {
      const Edge* x = *(itOut++);
      const unsigned int h = dep[lca(x->word, plast->word)];
      if (pin) addEdge(pin->virt[vTo][toRight], x->virt[vFrom][toRight], h);
      if (pout && itIn != in.cend())
        addEdge((*itIn)->virt[vTo][toLeft], pout->virt[vFrom][toLeft], h);
      plast = pout = x;
    }
  }
}
void processVertex(const unsigned int x)
{
  static const auto dfnLess = [](const Edge* a, const Edge* b) {
    return Trie::dfn[a->word] < Trie::dfn[b->word];
  };
  const unsigned int vp = ++Rebuild::vcur;
  sort(inEdge[x].begin(), inEdge[x].end(), dfnLess);
  sort(outEdge[x].begin(), outEdge[x].end(), dfnLess);
  linkVirt<vTo>(inEdge[x]);
  linkVirt<vFrom>(outEdge[x]);
  linkInOut(inEdge[x], outEdge[x]);
  Rebuild::endp[x] = vp;
  if (!inEdge[x].empty()) {
    Rebuild::addEdge(inEdge[x].front()->virt[vTo][toLeft], vp, 0);
    Rebuild::addEdge(inEdge[x].back()->virt[vTo][toRight], vp, 0);
  }
}
static unsigned int rebuild(const unsigned int n)
{
  for (unsigned int i = 1; i <= n; ++i)
    processVertex(i);
  for (const Edge* i : outEdge[1]) {
    Rebuild::addEdge(0, i->virt[vFrom][toLeft], 0);
    Rebuild::addEdge(0, i->virt[vFrom][toRight], 0);
  }
  return 0;
}
void clear()
{
  cur = ed;
  for (auto& i : inEdge)
    i.clear();
  for (auto& i : outEdge)
    i.clear();
}
}  // namespace Orig

void solve()
{
  unsigned int n, m, k;
  cin >> n >> m >> k;
  for (unsigned int i = 0; i < m; ++i) {
    unsigned int a, b, c, d;
    cin >> a >> b >> c >> d;
    Orig::addEdge(a, b, d, c);
  }
  for (unsigned int i = 1; i < k; ++i) {
    unsigned int u, v, w;
    cin >> u >> v >> w;
    Trie::addEdge(u, v);
  }
  Trie::init<1>();
  Rebuild::dijkstra(Orig::rebuild(n));
  {
    using Rebuild::dis;
    using Rebuild::endp;
    for (unsigned int i = 2; i <= n; ++i)
      cout << dis[endp[i]] << "\n";
  }
}

int main()
{
#ifndef APTEST
  ios_base::sync_with_stdio(false);
#endif
  unsigned int t;
  cin >> t;
  for (unsigned int i = 0; i < t; ++i) {
    solve();
    Trie::clear();
    Rebuild::clear();
    Orig::clear();
  }
  return 0;
}
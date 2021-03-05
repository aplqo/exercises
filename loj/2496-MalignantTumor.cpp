#ifdef APTEST
#include "debug_tools/judge.h"
#endif
#include <algorithm>
#include <iostream>
#include <vector>
const int maxn = 1e5, maxm = maxn + 10;
const int maxExtraEdge = maxm - maxn + 1, maxVirtVertex = maxExtraEdge * 4;
const unsigned int mod = 998244353;

template <int maxSize>
class Vector {
 public:
  void push_back(const int x) { *(ptr++) = x; }
  int *begin() { return data; }
  int *end() { return ptr; }
  size_t size() const { return ptr - data; }

 private:
  int data[maxSize], *ptr = data;
};

struct Number {
  inline friend Number operator+(const Number l, const Number r)
  {
    const unsigned int ret = l.val + r.val;
    return Number{ret >= mod ? ret - mod : ret};
  }
  inline void operator+=(const Number r)
  {
    val += r.val;
    if (val >= mod) val -= mod;
  }
  inline friend Number operator*(const Number l, const Number r)
  {
    return Number{static_cast<unsigned int>(
        (static_cast<unsigned long long>(l.val) * r.val) % mod)};
  }
  inline void operator*=(const Number r)
  {
    val = (static_cast<unsigned long long>(val) * r.val) % mod;
  }

  unsigned int val;
};

struct Transition {
  inline Transition operator+(const Number r) const
  {
    return Transition{dp0, dp1, constant + r};
  }
  inline Transition operator+(const Transition &r) const
  {
    return Transition{dp0 + r.dp0, dp1 + r.dp1, constant + r.constant};
  }
  inline Transition operator*(const Number r) const
  {
    return Transition{dp0 * r, dp1 * r, constant * r};
  }
  inline Number operator()(const Number v[2]) const
  {
    return dp0 * v[0] + dp1 * v[1] + constant;
  }

  Number dp0, dp1, constant;
};

namespace VirtualTree {
struct Edge {
  int to;
  const Transition *trans;
  const Edge *pre;
};
const Edge *head[maxn + 10];
Number dpInit[maxn + 10][2], f[maxn + 10][2];
unsigned int conflict[maxn + 10], vertexId[maxn + 10];
Vector<maxVirtVertex> vertices, namedVertices;
bool onTree[maxn + 10];

inline void addEdge(const int from, const int to, const Transition *trans)
{
  static Edge ed[maxVirtVertex + 10], *cur = ed;
  new (cur) Edge{to, trans, head[from]};
  head[from] = cur++;
}
template <bool allocateId>
inline void addVertex(const int x)
{
  static unsigned int cid = 1;
  if constexpr (allocateId)
    if (!vertexId[x]) {
      vertexId[x] = cid;
      cid <<= 1;
      namedVertices.push_back(x);
    }
  if (onTree[x]) return;
  onTree[x] = true;
  vertices.push_back(x);
}
void dfsDP(const int x, const unsigned int msk)
{
  f[x][0] = dpInit[x][0];
  f[x][1] = conflict[x] & msk ? Number{0} : dpInit[x][1];
  if (vertexId[x]) f[x][msk & vertexId[x] ? 0 : 1].val = 0;
  for (const Edge *i = head[x]; i; i = i->pre) {
    dfsDP(i->to, msk);
    const Number v[2]{i->trans[0](f[i->to]), i->trans[1](f[i->to])};
    f[x][0] *= v[0] + v[1];
    f[x][1] *= v[0];
  }
}
bool check(const unsigned int msk)
{
  for (auto i : namedVertices)
    if ((msk & vertexId[i]) && (conflict[i] & msk)) return false;
  return true;
}
Number enumerate()
{
  if (!namedVertices.size()) return dpInit[1][0] + dpInit[1][1];
  Number ret{0};
  const size_t e = 1u << namedVertices.size();
  for (unsigned int i = 0; i < e; ++i)
    if (check(i)) {
      dfsDP(1, i);
      ret += f[1][0] + f[1][1];
    }
  return ret;
}
}  // namespace VirtualTree

namespace Origin {
struct Edge {
  int to;
  const Edge *pre;
};
const Edge *head[maxn + 10];

inline void addEdge(const int from, const int to)
{
  static Edge ed[maxm * 2 + 10], *cur = ed;
  new (cur) Edge{to, head[from]};
  head[from] = cur++;
  new (cur) Edge{from, head[to]};
  head[to] = cur++;
}

bool dfsExtraEdge(const int x, const int fa)
{
  using VirtualTree::addVertex;
  static int dfn[maxn + 10], sta;
  static int back[maxn + 10], *end = back;
  int *const oldTop = end, virtCnt = 0;
  dfn[x] = ++sta;
  for (const Edge *i = head[x]; i; i = i->pre) {
    const int to = i->to;
    if (!dfn[to])
      virtCnt += dfsExtraEdge(to, x);
    else if (to != fa && dfn[to] < dfn[x]) {
      addVertex<true>(to);
      *(end++) = to;
    }
  }
  if (end > oldTop) {
    addVertex<false>(x);
    ++virtCnt;
    for (const int *i = oldTop; i < end; ++i) {
      using VirtualTree::conflict, VirtualTree::vertexId;
      conflict[*i] |= vertexId[x];
      conflict[x] |= vertexId[*i];
    }
    end = oldTop;
  }
  else if (virtCnt > 1)
    addVertex<false>(x);
  return virtCnt;
}
}  // namespace Origin

Transition f[maxn + 10][2];
bool vis[maxn + 10];

template <class V, class T, class U>
inline void update(V dest[2], const T cur[2], const U child[2])
{
  dest[0] = (child[0] + child[1]) * cur[0];
  dest[1] = child[0] * (cur[1]);
}
template <bool onTree>
int dfsTransition(const int x)
{
  using VirtualTree::dpInit;
  dpInit[x][0].val = dpInit[x][1].val = 1;
  vis[x] = true;
  int ret = onTree ? x : 0, next;
  for (auto i = Origin::head[x]; i; i = i->pre) {
    const int to = i->to;
    if (vis[to]) continue;
    const int nxt = VirtualTree::onTree[to] ? dfsTransition<true>(to)
                                            : dfsTransition<false>(to);
    if (!nxt)
      update(dpInit[x], dpInit[x], dpInit[to]);
    else {
      if constexpr (onTree)
        VirtualTree::addEdge(x, nxt, f[to]);
      else {
        ret = nxt;
        next = to;
      }
    }
  }
  if constexpr (onTree)
    f[x][0].dp0.val = f[x][1].dp1.val = 1;
  else {
    if (ret) update(f[x], dpInit[x], f[next]);
  }
  return ret;
}

Number solve()
{
  Origin::dfsExtraEdge(1, 0);
  VirtualTree::addVertex<false>(1);
  dfsTransition<true>(1);
  return VirtualTree::enumerate();
}

int main()
{
  std::ios::sync_with_stdio(false);
  int n, m;
  std::cin >> n >> m;
  for (int i = m; i; --i) {
    int u, v;
    std::cin >> u >> v;
    Origin::addEdge(u, v);
  }
  std::cout << solve().val << "\n";
  return 0;
}
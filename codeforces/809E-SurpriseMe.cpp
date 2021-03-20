#ifdef APTEST
#include "debug_tools/judge.h"
#endif
#include <algorithm>
#include <iostream>
#include <iterator>
#include <numeric>
const int maxn = 2e5;
const unsigned int mod = 1e9 + 7;

struct Number {
#define toNumber(x) \
  Number { static_cast<unsigned int>(x) }
  inline friend Number operator+(const Number l, const Number r)
  {
    const unsigned int v = l.val + r.val;
    return Number{v >= mod ? v - mod : v};
  }
  inline void operator+=(const Number r)
  {
    val += r.val;
    if (val >= mod) val -= mod;
  }
  inline friend Number operator-(const Number l, const Number r)
  {
    return Number{l.val >= r.val ? l.val - r.val : l.val + mod - r.val};
  }
  inline void operator-=(const Number r)
  {
    val = val >= r.val ? val - r.val : val + mod - r.val;
  }
  inline friend Number operator*(const Number l, const Number r)
  {
    return toNumber((static_cast<unsigned long long>(l.val) * r.val) % mod);
  }
  inline void operator*=(const Number r)
  {
    val = (static_cast<unsigned long long>(val) * r.val) % mod;
  }

  unsigned int val;
};
struct Tree {
  struct Edge {
    int to;
    const Edge *pre;
  } ed[maxn * 2 + 10], *cur = ed;
  const Edge *head[maxn + 10];

  inline void addDirectEdge(const int from, const int to)
  {
    new (cur) Edge{to, head[from]};
    head[from] = cur++;
  }
  inline void addEdge(const int u, const int v)
  {
    addDirectEdge(u, v);
    addDirectEdge(v, u);
  }
  inline void clearEdge() { cur = ed; }
} orig;

Number invPhi[maxn + 10], phi[maxn + 10], mu[maxn + 10];

Number quickPow(Number a, unsigned int e)
{
  Number ret{1};
  for (; e; e >>= 1) {
    if (e & 0x01) ret *= a;
    a *= a;
  }
  return ret;
}
inline Number inverse(const Number x) { return quickPow(x, mod - 2); }
static void eular(const int n)
{
  static int mindiv[maxn + 10], primes[maxn + 10], *cur = primes;
  phi[1].val = mu[1].val = 1;
  for (long long i = 2; i <= n; ++i) {
    if (!mindiv[i]) {
      mindiv[i] = i;
      *(cur++) = i;
      phi[i].val = i - 1;
      mu[i].val = mod - 1;
    }
    const Number nxtMu{mu[i].val ? mod - mu[i].val : 0};
    for (const int *j = primes; j < cur && *j < mindiv[i] && *j * i <= n; ++j) {
      const int p = *j * i;
      mindiv[p] = *j;
      phi[p].val = phi[i].val * (*j - 1);
      mu[p] = nxtMu;
    }
    if (const long long v = mindiv[i] * i; v <= n) {
      mindiv[v] = mindiv[i];
      phi[v].val = phi[i].val * mindiv[i];
    }
  }
}
static void initModularInv(const int n)
{
  static Number prd[maxn + 10], iprd[maxn + 10];
  prd[0].val = 1;
  for (int i = 1; i <= n; ++i)
    prd[i] = prd[i - 1] * phi[i];
  iprd[n] = inverse(prd[n]);
  for (int i = n; i; --i) {
    iprd[i - 1] = iprd[i] * phi[i];
    invPhi[i] = iprd[i] * prd[i - 1];
  }
}

unsigned int dep[maxn + 10];
int dfn[maxn + 10];
namespace HLD {
int son[maxn + 10], top[maxn + 10], fa[maxn + 10];

int dfsSon(const int x, const int f)
{
  dep[x] = dep[f] + 1;
  fa[x] = f;
  int ret = 1, ms = 0;
  for (auto i = orig.head[x]; i; i = i->pre) {
    const int to = i->to;
    if (to == f) continue;
    const int sz = dfsSon(to, x);
    ret += sz;
    if (sz > ms) {
      ms = sz;
      son[x] = to;
    }
  }
  return ret;
}
void dfsChain(const int x, const int tp)
{
  static int sta = 0;
  dfn[x] = ++sta;
  top[x] = tp;
  if (son[x]) dfsChain(son[x], tp);
  for (auto i = orig.head[x]; i; i = i->pre)
    if (!dfn[i->to]) dfsChain(i->to, i->to);
}
static void init()
{
  dfsSon(1, 0);
  dfsChain(1, 1);
}
int lca(int a, int b)
{
  while (top[a] != top[b]) {
    if (dep[top[a]] > dep[top[b]]) std::swap(a, b);
    b = fa[top[b]];
  }
  return dep[a] < dep[b] ? a : b;
}
}  // namespace HLD

namespace VirtTree {
Tree tree;
int keyId = 0, isKey[maxn + 10];

int build(int *const vertices, int *const end)
{
  static int stk[maxn + 10];
  ++keyId;
  std::sort(vertices, end, [](int a, int b) { return dfn[a] < dfn[b]; });
  std::for_each(vertices, end, [](int i) { isKey[i] = keyId; });
  int *top = stk, root = vertices[0];
  *(top++) = vertices[0];
  for (const int *it = vertices + 1; it != end; ++it) {
    const int lca = HLD::lca(*it, *(top - 1));
    while (top - stk > 1 && dfn[*(top - 2)] > dfn[lca]) {
      tree.addDirectEdge(*(top - 2), *(top - 1));
      --top;
    }
    if (top > stk && dfn[lca] < dfn[*(top - 1)])
      tree.addDirectEdge(lca, *(--top));
    if (top == stk) root = lca;
    if (top == stk || *(top - 1) != lca) *(top++) = lca;
    *(top++) = *it;
  }
  for (const int *i = stk + 1; i < top; ++i)
    tree.addDirectEdge(*(i - 1), *i);
  return root;
}

struct Subtree {
  Number answer, sum;
};
Subtree dfs(const unsigned int x)
{
  Subtree ret{{0}, {0}};
  unsigned long long acc = 0;
  if (isKey[x] == keyId) {
    ret.sum = phi[x];
    acc = (phi[x] * phi[x]).val;
  }
  for (auto i = tree.head[x]; i; i = i->pre) {
    const auto p = dfs(i->to);
    acc += (p.sum * ret.sum * Number{2}).val;
    ret.answer += p.answer;
    ret.sum += p.sum;
  }
  ret.answer += toNumber(acc % mod) * Number{dep[x]};
  tree.head[x] = nullptr;
  return ret;
}
inline Number dp(int *const begin, int *const end)
{
  tree.clearEdge();
  return dfs(build(begin, end)).answer;
}
}  // namespace VirtTree

static void init(const int n)
{
  HLD::init();
  eular(n);
  initModularInv(n);
}

Number sumFor(const int p, const int n)
{
  static int vertices[maxn + 10];
  int *ptr = vertices;
  unsigned long long accPhi = 0, accPrd = 0;
  for (int i = p; i <= n; i += p) {
    *(ptr++) = i;
    accPhi += phi[i].val;
    accPrd += (phi[i] * Number{dep[i]}).val;
  }
  return toNumber(accPrd % mod) * toNumber(accPhi % mod) -
         VirtTree::dp(vertices, ptr);
}
Number solve(const int n)
{
  static Number f[maxn + 10];
  unsigned long long ret = 0;
  for (int i = 1; i <= n; ++i)
    f[i] = sumFor(i, n);
  for (int i = 1; i <= n; ++i) {
    unsigned long long acc = 0;
    for (int j = i, r = 1; j <= n; j += i, ++r)
      acc += (f[j] * mu[r]).val;
    ret += (toNumber(acc % mod) * toNumber(i) * invPhi[i]).val;
  }
  return toNumber((ret * 2) % mod);
}

int main()
{
  std::ios::sync_with_stdio(false);
  static int a[maxn + 10];
  int n;
  std::cin >> n;
  init(n);
  std::copy_n(std::istream_iterator<int>(std::cin), n, a + 1);
  for (int i = 1; i < n; ++i) {
    int u, v;
    std::cin >> u >> v;
    orig.addEdge(a[u], a[v]);
  }
  init(n);
  std::cout << (solve(n) * inverse(toNumber(n) * toNumber(n - 1))).val << "\n";
  return 0;
}
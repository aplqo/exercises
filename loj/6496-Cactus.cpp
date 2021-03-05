#ifdef APTEST
#include "debug_tools/judge.h"
#endif
#include <algorithm>
#include <iostream>
#include <iterator>
#include <type_traits>
const int maxn = 1e5, maxm = 2e5, maxl = 19;
const int maxv = maxn * 2, maxe = maxv - 1;
const unsigned int mod = 998244353, primitiveRoot = 3;

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
    return Number{static_cast<unsigned int>(
        (static_cast<unsigned long long>(l.val) * r.val) % mod)};
  }
  inline void operator*=(const Number r)
  {
    val = (val * static_cast<unsigned long long>(r.val)) % mod;
  }

  unsigned int val;
};
template <class T>
struct Pointer {
  T *ptr;
  int size;
};
typedef Pointer<Number> ArrayPtr;
typedef Pointer<const Number> ConstArrayPtr;
template <int maxv, int maxe, bool directed>
class Graph {
 public:
  struct Void {
  };
  struct Edge {
    int to;
    const Edge *pre;
    std::conditional_t<directed, Void, const Edge *> rev;
  } ed[maxe];
  const Edge *head[maxv + 10];
  inline void addEdge(const int from, const int to);

 private:
  Edge *cur = ed;
};
Graph<maxn, maxm * 2, false> orig;
Graph<maxv, maxe, true> tree;

int virtVertex, n;
int a[maxn + 10];

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

template <int maxv, int maxe, bool directed>
void Graph<maxv, maxe, directed>::addEdge(const int from, const int to)
{
  if constexpr (directed) {
    new (cur) Edge{to, head[from], {}};
    head[from] = cur++;
  }
  else {
    new (cur) Edge{to, head[from], cur + 1};
    head[from] = cur++;
    new (cur) Edge{from, head[to], cur - 1};
    head[to] = cur++;
  }
}

namespace Polynomial {
const int maxsn = 1u << maxl;

Number unit[2][maxl + 1];
unsigned int rev[maxl + 1][maxsn + 10], lg[maxsn + 10];

static void initUnit(const int lgn)
{
  for (int i = 1; i <= lgn; ++i) {
    unit[0][i] = quickPow(Number{primitiveRoot}, (mod - 1) >> i);
    unit[1][i] = inverse(unit[0][i]);
  }
}
static void initLog(const int lgn)
{
  for (int i = 1; i <= lgn; ++i)
    std::fill(lg + (1u << (i - 1)) + 1, lg + (1u << i) + 1, i);
}
static void initRev(const int lgn)
{
  for (int i = 1; i <= lgn; ++i) {
    unsigned int *const crev = rev[i];
    const unsigned int n = 1u << i;
    for (int j = 0; j < i; ++j)
      crev[1 << j] = 1 << (i - 1 - j);
    for (unsigned int j = 3; j < n; ++j) {
      const unsigned int lb = j & -j;
      crev[j] = crev[j ^ lb] | crev[lb];
    }
  }
}
int Log2(const int x)
{
  for (int i = 0;; ++i)
    if (x <= (1 << i)) return i;
}
static void init(const int n)
{
  const int lgn = Log2(n * 2);
  initRev(lgn);
  initUnit(lgn);
  initLog(lgn);
}

void reverseCopy(Number dest[], const ConstArrayPtr src, const int lgn)
{
  const unsigned int *const crev = rev[lgn];
  const int n = 1 << lgn;
  for (int i = 0; i < src.size; ++i)
    dest[crev[i]] = src.ptr[i];
  for (int i = src.size; i < n; ++i)
    dest[crev[i]].val = 0;
}
template <bool typ>
void dft(const ConstArrayPtr src, Number dest[], const int lgn)
{
  reverseCopy(dest, src, lgn);
  const unsigned int n = 1u << lgn;
  for (int i = 1; i <= lgn; ++i) {
    const int step = 1 << i, half = 1 << (i - 1);
    const Number wm = unit[typ][i];
    for (Number *blk = dest; blk < dest + n; blk += step) {
      Number w{1};
      for (Number *j = blk, *k = blk + half; j < blk + half; ++j, ++k) {
        const Number u = *j, v = *k * w;
        *j = u + v;
        *k = u - v;
        w *= wm;
      }
    }
  }
  if constexpr (typ) {
    const Number inv = inverse(Number{n});
    for (Number *i = dest; i < dest + n; ++i)
      *i *= inv;
  }
}
void ntt(const ConstArrayPtr a, const ConstArrayPtr b, ArrayPtr &c)
{
  static Number tmpa[maxsn + 10], tmpb[maxsn + 10];
  const int lgn = lg[a.size + b.size - 1], n = 1u << lgn;
  dft<false>(a, tmpa, lgn);
  dft<false>(b, tmpb, lgn);
  for (int i = 0; i < n; ++i)
    tmpa[i] *= tmpb[i];
  dft<true>(ConstArrayPtr{tmpa, n}, c.ptr, lgn);
  c.size = a.size + b.size - 1;
}

void product(const Number dat[][3], const unsigned int n, ArrayPtr &dest)
{
  static Number mem[maxsn * maxl * 4 + 10];
  static ConstArrayPtr prod[maxsn * 4 + 10];
  if (n == 1) {
    std::copy(dat[0], dat[0] + 3, dest.ptr);
    dest.size = 3;
    return;
  }
  Number *cur = mem;
  const unsigned int lgn = lg[n], msk = 1u << lgn;
  {
    for (unsigned int i = 0; i < n; ++i)
      prod[msk | i] = ConstArrayPtr{dat[i], 3};
    *cur = Number{1};
    for (unsigned int i = n; i < msk; ++i)
      prod[msk | i] = ConstArrayPtr{cur, 1};
    ++cur;
  }
  for (unsigned int i = msk - 1; i > 1; --i) {
    const unsigned int left = i << 1, right = left | 1;
    if (prod[right].size == 1 && prod[right].ptr->val == 1)
      prod[i] = prod[left];
    else {
      ArrayPtr tp{cur, 0};
      ntt(prod[left], prod[right], tp);
      cur += tp.size;
      prod[i] = ConstArrayPtr{tp.ptr, tp.size};
    }
  }
  ntt(prod[2], prod[3], dest);
}
}  // namespace Polynomial

void tarjan(const int x, const decltype(orig)::Edge *rev)
{
  static int dfn[maxn + 10], low[maxn + 10], sta;
  static int stk[maxn + 10], *top = stk;
  *(top++) = x;
  dfn[x] = low[x] = ++sta;
  for (auto i = orig.head[x]; i; i = i->pre) {
    if (i == rev) continue;
    const int to = i->to;
    if (!dfn[to]) {
      tarjan(to, i->rev);
      low[x] = std::min(low[x], low[to]);
      if (dfn[x] < low[to]) {
        tree.addEdge(x, to);
        --top;
      }
      else if (low[to] == dfn[x]) {
        const int virt = virtVertex++;
        while (*(top - 1) != to)
          tree.addEdge(virt, *(--top));
        tree.addEdge(virt, *(--top));
        tree.addEdge(x, virt);
      }
    }
    else
      low[x] = std::min(low[x], dfn[to]);
  }
}

namespace DP {
Number f[maxv + 10][3];

template <bool first>
void dpSquare(const int x)
{
  Number g[2]{{!first}, {first}};  // g[0] x->? g[1] x<-?
  for (auto i = tree.head[x]; i; i = i->pre) {
    const int to = i->to;
    const Number old[2]{g[0], g[1]};
    g[0] = old[0] * f[to][1] + old[1] * f[to][2];
    g[1] = old[0] * f[to][0] + old[1] * f[to][1];
  }
  f[x][first] += g[1];
  f[x][first + 1] += g[0];
}
void dpRound(const int x)
{
  static Number tmp[maxv * 3 + 10], child[maxv + 10][3];
  int cnt = 0;
  for (auto i = tree.head[x]; i; i = i->pre, ++cnt)
    if (i->to >= n)
      std::reverse_copy(f[i->to], f[i->to] + 3, child[cnt]);
    else {
      std::reverse_copy(f[i->to], f[i->to] + 2, child[cnt]);
      child[cnt][2].val = 0;
    }
  ArrayPtr ans{tmp, 0};
  Polynomial::product(child, cnt, ans);
  {
    unsigned long long vacc = 0;
    for (int i = 0; i + 2 <= a[x] && i < ans.size; ++i)
      vacc += ans.ptr[i].val;
    f[x][0].val = f[x][1].val = f[x][2].val = vacc % mod;
  }
  f[x][1] =
      f[x][2] + (a[x] && ans.size > a[x] - 1 ? ans.ptr[a[x] - 1] : Number{0});
  f[x][0] = f[x][1] + (ans.size > a[x] ? ans.ptr[a[x]] : Number{0});
}

void dfs(const int x)
{
  if (x < n && !tree.head[x]) {
    for (int i = 0; i < 3 && i <= a[x]; ++i)
      f[x][i].val = 1;
    return;
  }
  for (auto i = tree.head[x]; i; i = i->pre)
    dfs(i->to);
  if (x < n)
    dpRound(x);
  else {
    dpSquare<true>(x);
    dpSquare<false>(x);
  }
}
}  // namespace DP

Number solve(const int n)
{
  virtVertex = n;
  tarjan(0, nullptr);
  Polynomial::init(virtVertex);
  DP::dfs(0);
  return DP::f[0][0];
}

int main()
{
  std::ios::sync_with_stdio(false);
  int m;
  std::cin >> n >> m;
  for (int i = 0; i < m; ++i) {
    int u, v;
    std::cin >> u >> v;
    orig.addEdge(u - 1, v - 1);
  }
  std::copy_n(std::istream_iterator<int>(std::cin), n, a);
  std::cout << solve(n).val << "\n";
  return 0;
}
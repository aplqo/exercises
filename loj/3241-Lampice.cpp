#ifdef APTEST
#include "debug_tools/program.h"
#endif
#include <algorithm>
#include <climits>
#include <iostream>
#include <unordered_set>
using namespace std;
const unsigned int maxn = 5e4, maxl = 16;
constexpr unsigned long long mod = 1e9 + 9, base = 131;
constexpr unsigned int msk = UINT_MAX ^ 0x01;

enum PathType { Long = 0, Short = 1, Same = 2, Invalid };
constexpr PathType inv[3] = {Short, Long, Same};
class Tree {
 public:
  struct Edge {
    unsigned int to;
    const Edge* pre;
  };
  const Edge* head[maxn + 10]{};
  inline void addEdge(const unsigned int from, const unsigned int to);

 private:
  Edge ed[maxn * 2 + 10], *cur = ed;
} orig, nxt;
unsigned int ch[maxn + 10];
unsigned int siz[maxn + 10], dep[maxn + 10], centDep[maxn + 10];
unsigned int vis[maxn + 10], sta;
unsigned int stk[maxn + 10], *top = stk + 1;
unsigned long long po[maxn + 10];
unsigned long long up[maxl + 10][maxn + 10], down[maxl + 10][maxn + 10];
unordered_set<unsigned long long> hashes[3];

inline void Tree::addEdge(const unsigned int from, const unsigned int to)
{
  new (cur) Edge{to, head[from]};
  head[from] = cur++;
}
static void getPow(const unsigned int n)
{
  po[0] = 1;
  for (unsigned int i = 1; i <= n; ++i)
    po[i] = (po[i - 1] * base) % mod;
}
pair<unsigned int, unsigned int> dfsPoint(const unsigned int x,
                                          const unsigned int f,
                                          const unsigned int n)
{
  siz[x] = 1;
  unsigned int ms = 0;
  pair<unsigned int, unsigned int> ret(UINT_MAX, 0);
  for (auto i = orig.head[x]; i; i = i->pre) {
    const unsigned int to = i->to;
    if (to == f || vis[to] == sta) continue;
    ret = min(ret, dfsPoint(to, x, n));
    siz[x] += siz[to];
    ms = max(ms, siz[to]);
  }
  ms = max(ms, n - siz[x]);
  return min(ret, make_pair(ms, x));
}
unsigned int dfsNextInfo(const unsigned int x, const unsigned int f,
                         const unsigned int cd)
{
  siz[x] = 1;
  dep[x] = dep[f] + 1;
  down[cd][x] = (down[cd][f] * base + ch[x]) % mod;
  up[cd][x] = (up[cd][f] + ch[x] * po[dep[x]]) % mod;
  for (auto i = orig.head[x]; i; i = i->pre)
    if (i->to != f && vis[i->to] != sta) siz[x] += dfsNextInfo(i->to, x, cd);
  return siz[x];
}
void dfsNextTree(const unsigned int x)
{
  vis[x] = sta;
  siz[x] = 1;
  dep[x] = 0;
  const unsigned int cd = centDep[x];
  up[cd][x] = down[cd][x] = ch[x];
  for (auto i = orig.head[x]; i; i = i->pre)
    if (vis[i->to] != sta) siz[x] += dfsNextInfo(i->to, x, cd);
  for (auto i = orig.head[x]; i; i = i->pre) {
    const unsigned int to = i->to;
    if (vis[to] == sta) continue;
    const unsigned int c = dfsPoint(to, x, siz[to]).second;
    centDep[c] = centDep[x] + 1;
    nxt.addEdge(x, c);
    dfsNextTree(c);
  }
}
static void initNext(const unsigned int n)
{
  ++sta;
  const unsigned int c = dfsPoint(1, 0, n).second;
  nxt.addEdge(0, c);
  centDep[c] = 1;
  dfsNextTree(c);
}

inline pair<PathType, unsigned long long> getHash(const unsigned int x,
                                                  const unsigned int len)
{
  static const auto sub = [](const unsigned int x, const unsigned int f,
                             const unsigned int len, const unsigned int cd) {
    return (down[cd][x] + mod - (down[cd][f] * po[len]) % mod) % mod;
  };
  const unsigned int cd = centDep[stk[0]], dep = top - stk - 1;
  if (dep * 2 == len - 1)
    return make_pair(Same, sub(x, stk[0], dep, cd));
  else if (len - dep > dep)
    return make_pair(Short, sub(x, stk[0], dep, cd));
  else {
    const unsigned int clen = len - dep - 1, c = *(top - clen - 1);
    if (up[cd][c] != down[cd][c]) return make_pair(Invalid, 0);
    return make_pair(Long, sub(x, c, clen, cd));
  }
}
bool dfsSubtree(const unsigned int x, const unsigned int f,
                const unsigned int len)
{
  if (top - stk >= len) return false;
  *(top++) = x;
  {
    const auto [typ, val] = getHash(x, len);
    if (typ != PathType::Invalid && hashes[inv[typ]].count(val)) return true;
  }
  for (auto i = orig.head[x]; i; i = i->pre)
    if (i->to != f && vis[i->to] != sta && dfsSubtree(i->to, x, len))
      return true;
  --top;
  return false;
}
void dfsAddVertex(const unsigned int x, const unsigned int f,
                  const unsigned int len)
{
  if (top - stk >= len) return;
  *(top++) = x;
  {
    const auto [typ, val] = getHash(x, len);
    if (typ != PathType::Invalid) hashes[typ].insert(val);
  }
  for (auto i = orig.head[x]; i; i = i->pre)
    if (i->to != f && vis[i->to] != sta) dfsAddVertex(i->to, x, len);
  --top;
}
bool conquer(const unsigned int x, const unsigned int len)
{
  vis[x] = sta;
  for (auto& i : hashes)
    i.reserve(siz[x]);
  hashes[Short].insert(0);
  stk[0] = x;
  for (auto i = orig.head[x]; i; i = i->pre) {
    const unsigned int to = i->to;
    if (vis[to] == sta) continue;
    if (dfsSubtree(to, x, len)) return true;
    dfsAddVertex(to, x, len);
  }
  for (auto& i : hashes)
    i.clear();

  for (auto i = nxt.head[x]; i; i = i->pre)
    if (conquer(i->to, len)) return true;
  return false;
}
static bool check(const unsigned int len)
{
  if (len <= 1) return true;
  ++sta;
  top = stk + 1;
  for (auto& i : hashes)
    i.clear();
  return conquer(nxt.head[0]->to, len);
}

template <unsigned int p>
unsigned int binarySearch(const unsigned int n)
{
  int l = 0, r = (n & msk) + p;
  while (l <= r) {
    const int mid = (((l + r) >> 1) & msk) + p;
    if (check(mid))
      l = mid + 2;
    else
      r = mid - 2;
  }
  return max(r, 0);
}

int main()
{
#ifndef APTEST
  ios_base::sync_with_stdio(false);
#endif
  static char str[maxn + 10];
  unsigned int n;
  cin >> n >> (str + 1);
  transform(str + 1, str + 1 + n, ch + 1,
            [](const char c) { return c + 1 - 'a'; });
  for (unsigned int i = 1; i < n; ++i) {
    unsigned int a, b;
    cin >> a >> b;
    orig.addEdge(a, b);
    orig.addEdge(b, a);
  }
  if (n == 2 && str[1] == str[2]) {
    cout << 2 << "\n";
    return 0;
  }
  else if (n == 1) {
    cout << 1 << "\n";
    return 0;
  }

  getPow(n);
  initNext(n);
  cout << max(binarySearch<0>(n), binarySearch<1>(n)) << "\n";
  return 0;
}
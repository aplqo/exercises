#ifdef APTEST
#include "debug_tools/program.h"
#endif
#include <algorithm>
#include <cstring>
#include <iostream>
#include <numeric>
using namespace std;
const unsigned int maxn = 1000;
constexpr unsigned long long mod = 1000000007;

class Number {
 public:
  Number() = default;
  Number(const unsigned int i) : val(i){};
  Number(const unsigned long long v) : val(v % mod){};
#define decl(x)                                                   \
  friend inline Number operator x(const Number l, const Number r) \
  {                                                               \
    return (l.val + mod) x r.val;                                 \
  }                                                               \
  inline Number& operator x##=(const Number r) { return *this = *this x r; }
  decl(+);
  decl(-);
  decl(*);
#undef decl
  friend inline ostream& operator<<(ostream& os, const Number v)
  {
    return os << v.val;
  }

 private:
  unsigned long long val = 0;
};
struct Edge {
  unsigned int to;
  bool dir;  // true from -> to
  const Edge* pre;
} ed[maxn * 2 + 10], *cur = ed;
const Edge* head[maxn + 10];
unsigned int siz[maxn + 10];
Number g[maxn + 10][maxn + 10], f[maxn + 10][maxn + 10];

inline void addEdge(const unsigned int from, const unsigned int to)
{
  new (cur) Edge{to, true, head[from]};
  head[from] = cur++;
  new (cur) Edge{from, false, head[to]};
  head[to] = cur++;
}
static void initG(const unsigned int n)
{
  fill(g[0], g[0] + n + 1, 1u);
  for (unsigned int i = 1; i <= n; ++i) {
    g[i][0] = 1u;
    for (unsigned int j = 1; j <= n; ++j)
      g[i][j] = g[i - 1][j] + g[i][j - 1];
  }
}
void dfs(const unsigned int x, const unsigned int fa)
{
  static Number sum[maxn + 10][maxn + 10];
  f[x][1] = 1u;
  siz[x] = 1;
  for (const Edge* i = head[x]; i; i = i->pre) {
    const unsigned int to = i->to;
    if (to == fa) continue;
    dfs(to, x);
    if (i->dir) {
      for (unsigned int i = siz[x]; i; --i) {
        for (unsigned int j = 1; j <= siz[to]; ++j)
          f[x][i + j] += f[x][i] * (sum[to][siz[to]] - sum[to][j]) *
                         g[i - 1][j] * g[siz[x] - i][siz[to] - j];
        f[x][i] = f[x][i] * sum[to][siz[to]] * g[siz[x] - i][siz[to]];
      }
    }
    else {
      for (unsigned int i = siz[x]; i; --i) {
        for (unsigned int j = 1; j <= siz[to]; ++j)
          f[x][i + j] +=
              f[x][i] * sum[to][j] * g[i - 1][j] * g[siz[x] - i][siz[to] - j];
        f[x][i] = 0u;
      }
    }
    siz[x] += siz[to];
  }
  partial_sum(f[x], f[x] + siz[x] + 1, sum[x]);
}
Number solve()
{
  unsigned int n;
  cin >> n;
  for (unsigned int i = 1; i < n; ++i) {
    unsigned int u, v;
    char typ;
    cin >> u >> typ >> v;
    if (typ == '<')
      addEdge(u, v);
    else
      addEdge(v, u);
  }
  dfs(0, 0);
  return accumulate(f[0], f[0] + siz[0] + 1, Number(0u));
}

int main()
{
#ifndef APTEST
  ios_base::sync_with_stdio(false);
#endif
  unsigned int t;
  cin >> t;
  initG(maxn);
  for (unsigned int i = 0; i < t; ++i) {
    cout << solve() << "\n";
    cur = ed;
    memset(head, 0, sizeof(head));
    fill(reinterpret_cast<Number*>(f),
         reinterpret_cast<Number*>(f) + sizeof(f) / sizeof(Number), 0u);
  }
  return 0;
}
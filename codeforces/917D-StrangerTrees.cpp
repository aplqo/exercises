#ifdef APTEST
#include "debug_tools/judge.h"
#endif
#include <algorithm>
#include <iostream>
#include <iterator>
const unsigned int maxn = 100;
constexpr unsigned long long mod = 1e9 + 7;

class Number {
 public:
#define decl(x)                                                   \
  inline friend Number operator x(const Number l, const Number r) \
  {                                                               \
    return Number{(l.val x r.val) % mod};                         \
  }                                                               \
  inline void operator x##=(const Number r) { val = (val x r.val) % mod; }
  decl(+);
  decl(*);
#undef decl
  inline friend Number operator-(const Number l, const Number r)
  {
    return Number{l.val > r.val ? l.val - r.val : l.val + mod - r.val};
  }
  inline void operator-=(const Number r)
  {
    val = val >= r.val ? val - r.val : val + mod - r.val;
  }
  inline friend std::ostream& operator<<(std::ostream& os, const Number v)
  {
    return os << v.val;
  }

  unsigned long long val;
};
struct Edge {
  unsigned int to;
  const Edge* pre;
} ed[maxn * 2 + 10];
const Edge* head[maxn + 10];
Number c[maxn + 10][maxn + 10], pow[maxn + 10], ans[maxn + 10];
Number f[maxn + 10][maxn + 10][2];

inline void addEdge(const unsigned int from, const unsigned int to)
{
  static Edge* cur = ed;
  new (cur) Edge{to, head[from]};
  head[from] = cur++;
  new (cur) Edge{from, head[to]};
  head[to] = cur++;
}

static void initC(const unsigned int n)
{
  c[0][0].val = 1;
  for (unsigned int i = 1; i <= n; ++i) {
    c[i][0].val = 1;
    for (unsigned int j = 1; j <= i; ++j)
      c[i][j] = c[i - 1][j] + c[i - 1][j - 1];
  }
}
static void initPow(const unsigned int n)
{
  pow[0].val = 1;
  for (unsigned int i = 1; i <= n; ++i)
    pow[i] = pow[i - 1] * Number{n};
}
static void init(const unsigned int n)
{
  initPow(n);
  initC(n);
}
void dfs(const unsigned int x, const unsigned int fa)
{
  static unsigned int size[maxn + 10];
  static Number tmp[maxn + 10][maxn + 10][2];
  size[x] = 1;
  f[x][1][1].val = f[x][1][0].val = 1;
  Number(*const ctmp)[2] = tmp[x], (*const cf)[2] = f[x];
  for (const Edge* i = head[x]; i; i = i->pre) {
    const unsigned int to = i->to;
    if (to == fa) continue;
    dfs(to, x);
    Number(*const cto)[2] = f[to];
    for (unsigned int j = 1; j <= size[x]; ++j)
      for (unsigned int k = 1; k <= size[to]; ++k) {
        ctmp[j + k][0] += cf[j][0] * cto[k][1];
        ctmp[j + k][1] += cf[j][1] * cto[k][1];
        ctmp[j + k - 1][0] += cf[j][0] * cto[k][0];
        ctmp[j + k - 1][1] += cf[j][1] * cto[k][0] + cf[j][0] * cto[k][1];
      }
    size[x] += size[to];
    for (unsigned int i = 1; i <= size[x]; ++i) {
      cf[i][0] = ctmp[i][0];
      cf[i][1] = ctmp[i][1];
      ctmp[i][0].val = ctmp[i][1].val = 0;
    }
  }
}
void inversation(const int n)
{
  for (int i = n - 1; i >= 0; --i)
    for (int j = i + 1; j < n; ++j)
      ans[i] -= c[j][i] * ans[j];
}
void count(const unsigned int n)
{
  dfs(1, 0);
  for (unsigned int i = 0; i < n - 1; ++i)
    ans[i] = pow[n - 2 - i] * f[1][n - i][1];
  ans[n - 1].val = 1;
  inversation(n);
}

int main()
{
  std::ios::sync_with_stdio(false);
  unsigned int n;
  std::cin >> n;
  for (unsigned int i = 1; i < n; ++i) {
    unsigned int x, y;
    std::cin >> x >> y;
    addEdge(x, y);
  }
  init(n);
  count(n);
  std::copy(ans, ans + n, std::ostream_iterator<Number>(std::cout, " "));
  return 0;
}
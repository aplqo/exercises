#ifdef APTEST
#include "debug_tools/program.h"
#endif
#include <iostream>
using namespace std;
const unsigned int maxn = 1e5;
constexpr unsigned long long mod = 1e9 + 7;

class Number {
 public:
  Number() = default;
  Number(const unsigned long long v) : val(v % mod){};
#define decl(x)                                                   \
  inline friend Number operator x(const Number l, const Number r) \
  {                                                               \
    return (l.val + mod) x r.val;                                 \
  }                                                               \
  inline void operator x##=(const Number r)                       \
  {                                                               \
    val = ((val + mod) x r.val) % mod;                            \
  }
  decl(+);
  decl(-);
  decl(*);
#undef decl
  inline friend ostream& operator<<(ostream& os, const Number v)
  {
    return os << v.val;
  }

 private:
  unsigned long long val = 0;
};
struct Edge {
  unsigned int to;
  const Edge* pre;
} ed[maxn * 2 + 10];
const Edge* head[maxn + 10];
Number f[maxn + 10][2];

inline void addEdge(const unsigned int from, const unsigned int to)
{
  static Edge* cur = ed;
  new (cur) Edge{to, head[from]};
  head[from] = cur++;
}
void dfs(const unsigned int x)
{
  f[x][1] = 1;
  Number g[2] = {1, 0}, even = 1;
  for (const Edge* i = head[x]; i; i = i->pre) {
    const unsigned int to = i->to;
    dfs(to);
    {
      const Number odd = f[x][1], even = f[x][0];
      f[x][0] = even + even * f[to][0] + odd * f[to][1];
      f[x][1] = odd + odd * f[to][0] + even * f[to][1];
    }
    even *= f[to][0] + 1;
    {
      const Number odd = g[1], even = g[0];
      g[0] = even + odd * f[to][1];
      g[1] = odd + even * f[to][1];
    }
  }
  f[x][0] = f[x][0] * 2 - g[1];
  f[x][1] = f[x][1] * 2 - even;
}

int main()
{
#ifndef APTEST
  ios::sync_with_stdio(false);
#endif
  unsigned int n;
  cin >> n;
  for (unsigned int i = 2; i <= n; ++i) {
    unsigned int p;
    cin >> p;
    addEdge(p, i);
  }
  dfs(1);
  cout << f[1][0] + f[1][1] << "\n";
  return 0;
}
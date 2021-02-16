#ifdef APTEST
#include "debug_tools/program.h"
#endif
#include <algorithm>
#include <cassert>
#include <cstring>
#include <iostream>
#include <queue>
#include <vector>

using namespace std;
const unsigned int maxn = 600, maxm = 1e5;

class Number {
 public:
  Number() = default;
  Number(const unsigned long long x) : val(x % mod){};
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
  inline bool isZero() const { return !val; }
  friend inline std::ostream& operator<<(std::ostream& os, const Number v)
  {
    return os << v.val;
  }

  static unsigned long long mod;

 private:
  unsigned long long val = 0;
};
unsigned long long Number::mod;
Number quickPow(Number a, unsigned long long e)
{
  Number ret = 1;
  for (; e; e >>= 1) {
    if (e & 0x01) ret *= a;
    a *= a;
  }
  return ret;
}
vector<unsigned int> gra[maxn + 10];
unsigned int indeg[maxn + 10], outdeg[maxn + 10];
Number mat[maxn + 10][maxn + 10], cnt[maxn + 10];
unsigned int indices[2][maxn + 10];

inline void addEdge(const unsigned int from, const unsigned int to)
{
  gra[from].push_back(to);
  ++indeg[to];
  ++outdeg[from];
}
static unsigned int initIndex(const unsigned int n)
{
  unsigned int idx = 0, odx = 0;
  for (unsigned int i = 1; i <= n; ++i) {
    if (!indeg[i]) indices[0][i] = idx++;
    if (!outdeg[i]) indices[1][i] = odx++;
  }
  return idx;
}
void topological(const unsigned int x, const unsigned int n)
{
  static unsigned int degree[maxn + 10];
  copy(indeg, indeg + n + 1, degree);
  fill(cnt, cnt + n + 1, 0);
  queue<unsigned int> q;
  cnt[x] = 1;
  for (unsigned int i = 1; i <= n; ++i)
    if (!indeg[i]) q.push(i);
  while (!q.empty()) {
    const unsigned int cur = q.front();
    q.pop();
    for (const unsigned int to : gra[cur]) {
      cnt[to] += cnt[cur];
      --degree[to];
      if (!degree[to]) q.push(to);
    }
  }
  const unsigned int id = indices[0][x];
  for (unsigned int i = 1; i <= n; ++i)
    if (!outdeg[i]) mat[id][indices[1][i]] = cnt[i];
}
Number gaussian(const unsigned int n)
{
  Number ret = 1;
  for (unsigned int i = 0; i < n; ++i) {
    if (mat[i][i].isZero()) {
      for (unsigned int j = i + 1; j < n; ++j)
        if (!mat[j][i].isZero()) {
          swap_ranges(mat[i], mat[i] + n, mat[j]);
          ret = 0 - ret;
          break;
        }
    }
    if (mat[i][i].isZero()) break;
    const Number inv = quickPow(mat[i][i], Number::mod - 2);
    for (unsigned int j = i + 1; j < n; ++j) {
      if (mat[j][i].isZero()) continue;
      const Number rat = mat[j][i] * inv;
      for (unsigned int k = i; k < n; ++k)
        mat[j][k] -= mat[i][k] * rat;
    }
  }
  return ret;
}
Number det(const unsigned int n)
{
  Number ret = gaussian(n);
  for (unsigned int i = 0; i < n; ++i)
    ret *= mat[i][i];
  return ret;
}

int main()
{
#ifndef APTEST
  std::ios_base::sync_with_stdio(false);
#endif
  unsigned int n, m;
  cin >> n >> m >> Number::mod;
  for (unsigned int i = 0; i < m; ++i) {
    unsigned int a, b;
    cin >> a >> b;
    addEdge(a, b);
  }
  const unsigned int id = initIndex(n);
  for (unsigned int i = 1; i <= n; ++i)
    if (!indeg[i]) topological(i, n);
  cout << det(id) << "\n";
  return 0;
}
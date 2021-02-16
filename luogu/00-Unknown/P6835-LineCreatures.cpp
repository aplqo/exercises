#ifdef APTEST
#include "debug_tools/program.h"
#endif
#include <iostream>
#include <vector>
using namespace std;
const unsigned int maxn = 1e6, maxm = 1e6;
constexpr unsigned long long mod = 998244353;

class Number {
 public:
  Number() = default;
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

vector<unsigned int> ed[maxn + 10];
unsigned int self[maxn + 10];

Number count(const unsigned int n)
{
  static Number sum[maxn + 10], f[maxn + 10];
  for (unsigned int i = 1; i <= n; ++i) {
    sum[i] = ed[i].size() * sum[i - 1] + self[i] + 1 + ed[i].size();
    for (unsigned int v : ed[i])
      sum[i] -= sum[v - 1];
    sum[i] += sum[i - 1];
  }
  return sum[n];
}

int main()
{
#ifndef APTEST
  ios_base::sync_with_stdio(false);
#endif
  unsigned int n, m, id;
  cin >> id >> n >> m;
  for (unsigned int i = 0; i < m; ++i) {
    unsigned int u, v;
    cin >> u >> v;
    if (u == v)
      ++self[u];
    else
      ed[u].push_back(v);
  }
  cout << count(n) << "\n";
  return 0;
}
#ifdef APTEST
#include "debug_tools/program.h"
#endif
#include <iostream>
#include <vector>
using namespace std;
const unsigned int maxn = 200000;
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
vector<unsigned int> chl[maxn + 1];
Number f[maxn + 1][2];

void dfs(const unsigned int x)
{
  static Number pre[maxn + 10], suf[maxn + 10];
  if (chl[x].empty()) {
    f[x][0] = f[x][1] = 1;
    return;
  }
  Number acc = 1, accNone = 1, accOne = 0;
  for (unsigned int to : chl[x]) {
    dfs(to);
    acc *= f[to][0] + f[to][1];
    accNone *= f[to][0];
  }
  {
    const unsigned int siz = chl[x].size();
    pre[0] = suf[siz] = 1;
    for (unsigned int i = 0; i < siz; ++i) {
      pre[i + 1] = pre[i] * f[chl[x][i]][0];
      suf[siz - i - 1] = suf[siz - i] * f[chl[x][siz - i - 1]][0];
    }
    for (unsigned int i = 0; i < siz; ++i)
      accOne += pre[i] * suf[i + 1] * f[chl[x][i]][1];
  }
  f[x][0] = acc - accOne;
  f[x][1] = acc - accNone;
}
int main()
{
#ifndef APTEST
  ios_base::sync_with_stdio(false);
#endif
  unsigned int n;
  cin >> n;
  for (unsigned int i = 2; i <= n; ++i) {
    unsigned int f;
    cin >> f;
    chl[f].push_back(i);
  }
  dfs(1);
  cout << f[1][0] << "\n";
  return 0;
}
#ifdef APTEST
#include "debug_tools/program.h"
#endif
#include <algorithm>
#include <functional>
#include <iostream>
#include <iterator>
using namespace std;
const unsigned int maxn = 30, maxs = maxn * maxn, maxc = 10;
constexpr unsigned long long mod = 1e9 + 9;

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

unsigned int cnt[maxc + 10];
Number C[maxs + 10][maxs + 10], use[maxs + 10][maxn + 10][maxn + 10],
    col[maxc + 10][maxn + 10][maxn + 10];

static void getC(const unsigned int n)
{
  C[0][0] = 1;
  for (unsigned int i = 1; i <= n; ++i) {
    C[i][0] = 1;
    for (unsigned int j = 1; j <= i; ++j)
      C[i][j] = C[i - 1][j] + C[i - 1][j - 1];
  }
}
static void dpUse(const unsigned int n, const unsigned int m,
                  const unsigned int k)
{
  for (unsigned int i = 1; i <= n; ++i)
    for (unsigned int j = 1; j <= m; ++j) {
      if (i * j < k) continue;
      use[k][i][j] = C[i * j][k];
      for (unsigned int r = 1; r <= i; ++r)
        for (unsigned int c = 1; c <= j; ++c)
          if (r < i || c < j) use[k][i][j] -= use[k][r][c] * C[i][r] * C[j][c];
    }
}
static void dpColor(const unsigned int n, const unsigned int m,
                    const unsigned int colCnt)
{
  col[0][0][0] = 1;
  for (unsigned int k = 1; k <= colCnt; ++k)
    for (unsigned int i = 1; i <= n; ++i)
      for (unsigned int j = 1; j <= m; ++j) {
        for (unsigned int l = 0; l < i; ++l)
          for (unsigned int r = 0; r < j; ++r)
            if ((i - l) * (j - r) >= cnt[k])
              col[k][i][j] += col[k - 1][l][r] * use[cnt[k]][i - l][j - r] *
                              C[n - l][i - l] * C[m - r][j - r];
      }
}
Number count(const unsigned int n, const unsigned int m, const unsigned int c)
{
  Number ret = 0;
  for (unsigned int i = 1; i <= n; ++i)
    for (unsigned int j = 1; j <= m; ++j)
      ret += col[c][i][j];
  return ret;
}
int main()
{
#ifndef APTEST
  ios_base::sync_with_stdio(false);
#endif
  unsigned int n, m, c;
  cin >> n >> m >> c;
  copy_n(istream_iterator<unsigned int>(cin), c, cnt + 1);
  getC(n * m);
  for_each(cnt + 1, cnt + 1 + c, bind(dpUse, n, m, placeholders::_1));
  dpColor(n, m, c);
  cout << count(n, m, c) << "\n";
  return 0;
}

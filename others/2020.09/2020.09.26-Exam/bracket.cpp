#ifdef APTEST
#include "debug_tools/program.h"
#endif
#include <algorithm>
#include <iostream>
#include <string>
#include <utility>
using namespace std;
const unsigned int maxl = 2000, maxm = 1e5;
constexpr unsigned long long mod = 1e9 + 7;

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
  decl(*);
#undef decl
  friend inline ostream& operator<<(ostream& os, const Number v)
  {
    return os << v.val;
  }

 private:
  unsigned long long val = 0;
};
char str[maxm + 10];
Number cnt[maxl * 2 + 10][maxl * 2 + 10];

static void dpCount(const unsigned int n)
{
  cnt[0][0] = 1;
  for (unsigned int i = 0; i < n; ++i) {
    cnt[i + 1][1] += cnt[i][0];
    for (unsigned int j = 1; j <= i; ++j) {
      cnt[i + 1][j + 1] += cnt[i][j];
      cnt[i + 1][j - 1] += cnt[i][j];
    }
  }
}
pair<unsigned int, unsigned int> matchBracket(const char* str, const size_t len)
{
  static char stk[maxm + 10];
  char* top = stk;
  for (const char* i = str; i < str + len; ++i) {
    if (*i == '(')
      *(top++) = *i;
    else {
      if (top > stk && *(top - 1) == '(')
        --top;
      else
        *(top++) = *i;
    }
  }
  const unsigned int c = count(stk, top, ')');
  return make_pair(c, top - stk - c);
}
Number countPQ(const unsigned int len, const unsigned int left,
               const unsigned int right)
{
  Number ret = 0;
  for (unsigned int i = 0; i <= len; ++i)
    for (unsigned int j = 0; j <= len; ++j)
      ret += cnt[i][left + j] * cnt[len - i][right + j];
  return ret;
}
int main()
{
#ifndef APTEST
  ios_base::sync_with_stdio(false);
#endif
  unsigned int n, m;
  cin >> n >> m;
  cin >> str;
  dpCount(n - m);
  const auto [l, r] = matchBracket(str, m);
  cout << countPQ(n - m, l, r) << "\n";
  return 0;
}
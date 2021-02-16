/*Luogu team T132163: F2 */
#ifdef APTEST
#include "debug_tools/program.h"
#endif
#include <initializer_list>
#include <iostream>
using namespace std;
const int maxn = 50000;

const unsigned int seq[][4] = {{0, 1, 2, 3}, {0, 2, 1, 3}, {0, 3, 1, 2},
                               {1, 2, 0, 3}, {1, 3, 0, 2}, {2, 3, 0, 1}};
unsigned int ans;
constexpr unsigned int toInt(const char c)
{
  return c >= 'a' ? c - 'a' : c - '0' + 26;
}
struct trie {
  void insert(const char c[4], const unsigned int s[4])
  {
    unsigned int t = 0;
    for (unsigned int i = 0; i < 4; ++i) {
      const unsigned int v = toInt(c[s[i]]);
      if (!ch[t][v]) ch[t][v] = ++cnt;
      t = ch[t][v];
    }
    ++b[t];
  }

  unsigned int ch[maxn * 4 + 10][26 + 10 + 10] = {}, cnt = 0;
  unsigned int b[maxn * 4 + 10] = {};
} tr[6];
template <int dep>
struct fun {
  inline unsigned int operator()(const trie& t, const unsigned int pos,
                                 const unsigned int m)
  {
    unsigned int ret = 0;
    const unsigned int beg = toInt('a'), end = toInt('9');
    for (unsigned int i = beg; i <= end; ++i) {
      if (!t.ch[pos][i]) continue;
      unsigned int cur = fun<dep + 1>()(t, t.ch[pos][i], m);
      if (dep == 4 - m) ans += cur * ret;
      ret += cur;
    }
    return ret;
  }
};
template <>
struct fun<4> {
  inline unsigned int operator()(const trie& t, const unsigned int pos,
                                 const unsigned int m)
  {
    return t.b[pos];
  }
};
namespace bf {
#define test(s, m) (static_cast<bool>((s) & (m)))
uint_fast32_t str[maxn + 10];

constexpr uint_fast32_t msk(const unsigned int v) { return 0xff << (v * 8); }
inline unsigned int calc(const uint_fast32_t v)
{
  return test(v, msk(0)) + test(v, msk(1)) + test(v, msk(2)) + test(v, msk(3));
}
unsigned int solve(const unsigned int n, const unsigned int m)
{
  unsigned int ret = 0;
  for (unsigned int i = 0; i < n; ++i)
    for (unsigned int j = i + 1; j < n; ++j)
      ret += calc(str[i] ^ str[j]) == m;
  if (m == 0) ret += n;
  return ret;
}
void read(const unsigned int n)
{
  for (unsigned int i = 0; i < n; ++i)
    scanf("%s", reinterpret_cast<char*>(&str[i]));
}
}  // namespace bf

int main()
{
  unsigned int n, m;
  scanf("%d%d", &n, &m);
  if (n > 30000) {
    for (unsigned int i = 0; i < n; ++i) {
      unsigned int j = 0;
      char str[5];
      cin >> str;
      scanf("%s", str);
      for (const auto& k : seq) {
        tr[j].insert(str, k);
        ++j;
      }
    }
    for (unsigned int i = 0; i < 6; ++i)
      fun<0>()(tr[i], 0, m);
    printf("%d\n", ans);
  }
  else {
    bf::read(n);
    printf("%d\n", bf::solve(n, m));
  }
  return 0;
}

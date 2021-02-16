#ifdef APDEBUG
#include "debug_tools/program.h"
#endif
#include <cctype>
#include <cstdint>
#include <cstring>
#include <iostream>
using namespace std;
typedef uint_fast64_t hash_t;
const int maxn = 1e6;
const hash_t base = 131;

hash_t po[maxn + 10], hasb, has[maxn + 10];
size_t la, lb;
char sa[maxn + 10], sb[maxn + 10], typ;

inline void getPow(size_t n)
{
  po[0] = 1;
  for (size_t i = 1; i <= n; ++i)
    po[i] = po[i - 1] * base;
}
inline void getHash(const char* str, size_t len)
{
  for (int i = 1; i <= len; ++i)
    has[i] = has[i - 1] * base + str[i] - typ;
}
inline hash_t getSub(int beg, size_t len)
{
  return has[beg - 1 + len] - has[beg - 1] * po[len];
}

int main()
{
  ios_base::sync_with_stdio(false);
  cin.tie(nullptr);
  int ans = 0;
  cin >> (sa + 1) >> (sb + 1);
  la = strlen(sa + 1);
  lb = strlen(sb + 1);
  if (lb > la) {
    cout << "0" << endl;
    return 0;
  }
  getPow(la + 1);
  typ = isupper(sa[0]) ? 'A' : 'a';
  getHash(sb, lb);
  hasb = has[lb];
  getHash(sa, la);
  for (int i = 1; i <= la - lb + 1; ++i)
    if (getSub(i, lb) == hasb) ++ans;
  cout << ans << endl;
  return 0;
}
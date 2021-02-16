#ifdef APTEST
#include "debug_tools/program.h"
#endif
#include <algorithm>
#include <iostream>
using namespace std;
const unsigned int maxn = 1e5;
constexpr unsigned long long mod = 1e9 + 7;

struct Section {
  unsigned int h = 0;
  unsigned long long w = 0;
  unsigned int lc = 0, rc = 0;
} sec[maxn + 10];
unsigned long long ans;

inline unsigned long long c2(const unsigned long long n,
                             const unsigned long long v)
{
  return (n * v / 2) % mod;
}
void build(const unsigned int n)
{
  static unsigned int stk[maxn + 10];
  unsigned int* top = stk + 1;
  for (unsigned int i = 1; i <= n; ++i) {
    unsigned int* ptr = top;
    while (ptr > stk && sec[*(ptr - 1)].h > sec[i].h)
      --ptr;
    if (ptr > stk) sec[*(ptr - 1)].rc = i;
    if (ptr < top) sec[i].lc = *ptr;
    top = ptr;
    *(top++) = i;
  }
}
unsigned long long dfs(const unsigned int x, const unsigned long long fv)
{
  if (sec[x].lc) sec[x].w += dfs(sec[x].lc, sec[x].h);
  if (sec[x].rc) sec[x].w += dfs(sec[x].rc, sec[x].h);
  sec[x].w %= mod;
  ans = (ans + (c2(sec[x].h + fv + 1, sec[x].h - fv) *
                c2(sec[x].w + 1, sec[x].w) % mod)) %
        mod;
  return sec[x].w;
}
unsigned int unique(const unsigned int n)
{
  Section* ptr = sec;
  for (Section* i = sec + 1; i < sec + 1 + n; ++i)
    if (i->h != ptr->h)
      *(++ptr) = *i;
    else
      ptr->w += i->w;
  return ptr - sec;
}

int main()
{
#ifndef APTEST
  ios_base::sync_with_stdio(false);
#endif
  unsigned int n;
  cin >> n;
  for (Section* i = sec + 1; i < sec + 1 + n; ++i)
    cin >> i->h;
  for (Section* i = sec + 1; i < sec + 1 + n; ++i)
    cin >> i->w;
  build(unique(n));
  dfs(sec[0].rc, 0);
  cout << ans << "\n";
  return 0;
}
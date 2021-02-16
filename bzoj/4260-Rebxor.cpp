#ifdef APDEBUG
#include "debug_tools/program.h"
#endif
#include <algorithm>
#include <cctype>
#include <cstdint>
#include <cstdio>
#include <cstring>
#include <functional>
#include <iostream>
using namespace std;
typedef uint_fast32_t num;
const int maxn = 4e5, maxl = 29;

int ch[(maxn + 10) * maxl][2], cur = 0, n;
num a[maxn + 10], ans;
num ls[maxn + 10], rs[maxn + 10];

template <class T>
inline void read(T& dat)
{
  dat = 0;
  char c = getchar();
  while (!isdigit(c))
    c = getchar();
  while (isdigit(c)) {
    dat = dat * 10 + c - '0';
    c = getchar();
  }
}
void insert(num val)
{
  num t = num(1) << maxl;
  int k = 0;
  for (int i = 0; i < maxl + 1; ++i, t >>= 1) {
    int p = val & t ? 1 : 0;
    if (!ch[k][p]) {
      ch[k][p] = ++cur;
      k = ch[k][p];
      ch[k][0] = 0;
      ch[k][1] = 0;
    }
    else
      k = ch[k][p];
  }
}
num find(num val)
{
  num t = num(1) << maxl, ret = 0;
  int k = 0;
  for (int i = 0; i < maxl + 1; ++i, t >>= 1) {
    int p1 = val & t ? 0 : 1;
    if (ch[k][p1]) {
      k = ch[k][p1];
      ret |= t;
    }
    else
      k = ch[k][!p1];
  }
  return ret;
}
template <class T>
void fun(int beg, num res[])
{
  T inc;
  num lst = 0;
  int lpos = 0;
  insert(0);
  for (int i = beg; i > 0 && i <= n; i = inc(i, 1)) {
    res[i] = max(res[lpos], find(lst ^= a[i]));
    lpos = i;
    insert(lst);
  }
}

int main()
{
  read(n);
  for (num* i = a + 1; i < a + n + 1; ++i)
    read(*i);

  fun<plus<int>>(1, ls);
  cur = 0;
  ch[0][0] = 0;
  ch[0][1] = 0;
  fun<minus<int>>(n, rs);

  for (int i = 1; i <= n - 1; ++i)
    ans = max(ans, ls[i] + rs[i + 1]);
  cout << ans << endl;
  return 0;
}
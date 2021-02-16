#ifdef APTEST
#include "debug_tools/program.h"
#endif
#include <algorithm>
#include <cstdio>
#include <iostream>
#include <iterator>
using namespace std;
const unsigned int maxn = 1e5, maxm = 1e5;

char outbuf[maxm * 2 * 10 + 1000], *outpos = outbuf;
unsigned int lc[maxn + 10], rc[maxn + 10], siz[maxn + 10];
unsigned int a[maxn + 10];

inline void writeRange(const unsigned int l, const unsigned int r)
{
  outpos += sprintf(outpos, "%u %u\n", l, r);
}
static unsigned int buildTree(const unsigned int n)
{
  static unsigned int stk[maxn + 10];
  unsigned int *top = stk, root = 1;
  *(top++) = 1;
  for (unsigned int i = 2; i <= n; ++i) {
    unsigned int* ptr = top;
    while (ptr > stk && a[i] < a[*(ptr - 1)])
      --ptr;
    if (ptr > stk)
      rc[*(ptr - 1)] = i;
    else
      root = i;
    if (ptr < top) lc[i] = *ptr;
    top = ptr;
    *(top++) = i;
  }
  return root;
}
unsigned int dfsCount(const unsigned int x, const unsigned int f)
{
  if (!x) return 0;
  const unsigned int ret =
      dfsCount(lc[x], x) + dfsCount(rc[x], x) + a[x] - a[f];
  siz[x] = 1 + siz[lc[x]] + siz[rc[x]];
  return ret;
}
inline void repeat(const unsigned int l, const unsigned int r,
                   const unsigned int t)
{
  for (unsigned int i = 0; i < t; ++i)
    writeRange(l, r);
}
void dfsSolution(const unsigned int x, const unsigned int f)
{
  if (!x) return;
  repeat(x - siz[lc[x]], x + siz[rc[x]], a[x] - a[f]);
  dfsSolution(lc[x], x);
  dfsSolution(rc[x], x);
}

int main()
{
#ifndef APTEST
  ios_base::sync_with_stdio(false);
#endif
  unsigned int n;
  cin >> n;
  copy_n(istream_iterator<unsigned int>(cin), n, a + 1);
  const unsigned int root = buildTree(n);
  cout << dfsCount(root, 0) << "\n";
  dfsSolution(root, 0);
  cout.write(outbuf, outpos - outbuf);
  return 0;
}

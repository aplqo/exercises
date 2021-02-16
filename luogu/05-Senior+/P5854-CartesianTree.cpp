#ifdef APTEST
#include "debug_tools/program.h"
#endif
#include <cctype>
#include <cstdio>
using namespace std;
const unsigned int maxn = 1e7;

struct node {
  unsigned int val;
  unsigned int lc = 0, rc = 0;
} nodes[maxn + 1];
unsigned int p[maxn + 1];

inline unsigned int read()
{
  unsigned int ret = 0;
  char c = getchar();
  while (!isdigit(c))
    c = getchar();
  while (isdigit(c)) {
    ret = ret * 10 + c - '0';
    c = getchar();
  }
  return ret;
}
void buildTree(const unsigned int n)
{
  static unsigned int stk[maxn + 1], *top = stk;
  *(top++) = 1;
  for (unsigned int i = 2; i <= n; ++i) {
    unsigned int* ptr = top;
    while (ptr > stk && nodes[*(ptr - 1)].val > nodes[i].val)
      --ptr;
    if (ptr > stk) nodes[*(ptr - 1)].rc = i;
    if (ptr < top) nodes[i].lc = *ptr;
    top = ptr;
    *(top++) = i;
  }
}
int main()
{
  const unsigned int n = read();
  for (unsigned int i = 1; i <= n; ++i)
    nodes[i].val = read();
  buildTree(n);
  unsigned long long xl = 0, xr = 0;
  for (unsigned long long i = 1; i <= n; ++i) {
    xl ^= i * (nodes[i].lc + 1);
    xr ^= i * (nodes[i].rc + 1);
  }
  printf("%lld %lld\n", xl, xr);
  return 0;
}
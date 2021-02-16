#ifdef APTEST
#include "debug_tools/program.h"
#endif
#include <algorithm>
#include <cstdio>
using namespace std;
const unsigned int maxn = 1e6;
constexpr unsigned long long mod = 998244353;

inline int read()
{
  char x;
  int num;
  while (x = getchar(), x < '0' || x > '9')
    ;
  num = x - '0';
  while (x = getchar(), x >= '0' && x <= '9')
    num *= 10, num += x - '0';
  return num;
}

struct node {
  void remove()
  {
    pre->nxt = nxt;
    nxt->pre = pre;
  }
  unsigned int val;
  node *pre, *nxt;
} seq[maxn + 1];
node* a[maxn + 1];

static void build(const unsigned int n)
{
  for (node* i = seq + 1; i < seq + 1 + n; ++i) {
    i->pre = i - 1;
    i->nxt = i + 1;
  }
  seq[n].nxt = seq + n + 1;
}
unsigned long long calcPoint(const unsigned int n, node* const p,
                             const unsigned int k)
{
  node *l = p, *r = p;
  unsigned int acc = 1;
  unsigned long long ret = 0;
  for (; acc < k && l->pre != seq; ++acc)
    l = l->pre;
  for (; acc < k && r->nxt != seq + n + 1; ++acc)
    r = r->nxt;
  if (acc < k) return 0;
  while (l != p->nxt && r != seq + 1 + n) {
    const unsigned long long v = ((l - l->pre) * (r->nxt - r)) % mod;
    ret = (ret + v) % mod;
    l = l->nxt;
    r = r->nxt;
  }
  return (ret * p->val) % mod;
}
unsigned long long calc(const unsigned int n, const unsigned int k)
{
  unsigned long long ret = 0;
  build(n);
  for (unsigned int j = 1; j <= n; ++j) {
    ret = (ret + calcPoint(n, a[j], k)) % mod;
    a[j]->remove();
  }
  return ret;
}

int main()
{
  const unsigned int n = read();
  const unsigned int k = read();
  for (unsigned int i = 1; i <= n; ++i)
    seq[i].val = read();
  transform(seq + 1, seq + 1 + n, a + 1, [](node& i) { return &i; });
  sort(a + 1, a + 1 + n,
       [](node* const l, node* const r) { return l->val < r->val; });
  unsigned long long v = calc(n, k);
  reverse(a + 1, a + 1 + n);
  v = (v + calc(n, k)) % mod;
  printf("%lld\n", v);
  return 0;
}
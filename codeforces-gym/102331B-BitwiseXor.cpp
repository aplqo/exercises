#ifdef APTEST
#include "debug_tools/program.h"
#endif
#include <iostream>
using namespace std;
const unsigned int maxn = 3e5, maxl = 61;
const unsigned long long mod = 998244353;

class Number {
 public:
  Number() = default;
  Number(const unsigned long long v) : val(v % mod){};
#define decl(x)                                                   \
  friend inline Number operator x(const Number l, const Number r) \
  {                                                               \
    return (l.val + mod) x r.val;                                 \
  }                                                               \
  inline void operator x##=(const Number r) { *this = *this x r; }
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
struct Node {
  unsigned long long greaterCount(const unsigned long long val,
                                  const unsigned long long lim) const;
  unsigned long long dfsSelectOne(const Node* rt, const unsigned long long acc,
                                  const unsigned long long x) const;
  Number dfsSubseq(const unsigned long long acc,
                   const unsigned long long x) const;

  Node* chl[2];
  unsigned int size, dep;
  bool isEnd;
} nodes[maxn * maxl + 19];
Node root;

Number quickPow(Number a, unsigned long long e)
{
  Number ret = 1;
  for (; e; e >>= 1) {
    if (e & 0x01) ret *= a;
    a *= a;
  }
  return ret;
}
void insert(const unsigned long long v)
{
  static Node* cur = nodes;
  Node* ptr = &root;
  for (unsigned long long i = 1ull << maxl; i; i >>= 1) {
    const bool b = v & i;
    if (!ptr->chl[b])
      ptr->chl[b] = new (cur++) Node{{}, 0, ptr->dep - 1, false};
    ++(ptr->size);
    ptr = ptr->chl[b];
  }
  ++(ptr->size);
  ptr->isEnd = true;
}
unsigned long long Node::greaterCount(const unsigned long long val,
                                      const unsigned long long lim) const
{
  if (isEnd) return (val & 0x01) >= (lim & 0x01) ? size : 0;
  unsigned int ret = 0;
  const Node* ptr = this;
  for (unsigned long long msk = 1ull << dep; msk && ptr; msk >>= 1) {
    const bool b = val & msk;
    if (lim & msk)
      ptr = ptr->chl[!b];
    else {
      if (ptr->chl[!b]) ret += ptr->chl[!b]->size;
      ptr = ptr->chl[b];
    }
  }
  if (ptr) ret += ptr->size;
  return ret;
}
unsigned long long Node::dfsSelectOne(const Node* rt,
                                      const unsigned long long acc,
                                      const unsigned long long x) const
{
  if (isEnd) return rt->greaterCount(acc, x) * size;
  unsigned long long ret = 0;
  for (unsigned long long i = 0; i < 2; ++i)
    if (chl[i]) ret += chl[i]->dfsSelectOne(rt, acc | (i << dep), x);
  return ret;
}
Number Node::dfsSubseq(const unsigned long long acc,
                       const unsigned long long x) const
{
  const unsigned long long msk = 1ull << dep;
  if (isEnd) return 0;
  if (x & msk)
    return chl[0] && chl[1] ? chl[1]->dfsSelectOne(chl[0], acc | msk, x) : 0;
  if (chl[0] && chl[1])
    return (chl[0]->dfsSubseq(acc, x) + chl[0]->size + 1) *
               (chl[1]->dfsSubseq(acc | msk, x) + chl[1]->size + 1) -
           (1 + size);
  else {
    const unsigned long long typ = chl[0] ? 0 : 1;
    return chl[typ]->dfsSubseq(acc | (typ << dep), x);
  }
}

int main()
{
#ifndef APTEST
  ios_base::sync_with_stdio(false);
#endif
  unsigned int n;
  unsigned long long x;
  cin >> n >> x;
  if (!x) {
    cout << (quickPow(2, n) - 1) << "\n";
    return 0;
  }
  new (&root) Node{{}, 0, maxl, false};
  for (unsigned int i = n; i; --i) {
    unsigned long long v;
    cin >> v;
    insert(v);
  }
  cout << (n + root.dfsSubseq(0, x)) << "\n";
  return 0;
}

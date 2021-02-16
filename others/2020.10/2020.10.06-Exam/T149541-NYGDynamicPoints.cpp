#ifdef APTEST
#include "debug_tools/program.h"
#endif
#include <algorithm>
#include <iostream>
#include <iterator>
#include <numeric>
#include <utility>
using namespace std;
const unsigned int maxn = 5e5;

enum { L = 0, R = 1 };
struct Range {
  unsigned int pos[2];
  inline unsigned int length() const { return pos[1] - pos[0]; }
  inline operator const unsigned int*() const { return pos; }
};
class SegmentTree {
 public:
  void build(const unsigned int l, const unsigned int r, SegmentTree*(&cur),
             const unsigned int val[]);
  Range binary(const unsigned int p, const unsigned int gcdV) const;

 private:
  template <bool dir>  // true: l->r false: r->l
  unsigned int binaryFind(const unsigned int val) const;

  SegmentTree* chl[2];
  Range rg;
  unsigned int mv;
  unsigned int val;
} snodes[maxn * 4 + 10], *root = snodes;
unsigned int a[maxn + 10];
Range rng[maxn + 10];
bool hasAns[maxn + 10];

void SegmentTree::build(const unsigned int l, const unsigned int r,
                        SegmentTree*(&cur), const unsigned int val[])
{
  rg = Range{l, r};
  mv = (l + r) >> 1;
  if (r - l == 1) {
    this->val = val[l];
    return;
  }
  chl[L] = ++cur;
  chl[R] = ++cur;
  chl[L]->build(rg[L], mv, cur, val);
  chl[R]->build(mv, rg[R], cur, val);
  this->val = gcd(chl[L]->val, chl[R]->val);
}
Range SegmentTree::binary(const unsigned int p, const unsigned int gcdV) const
{
  if (val % gcdV == 0) return rg;
  const unsigned int typ = p >= mv;
  const Range v = chl[typ]->binary(p, gcdV);
  if (v.pos[!typ] == chl[typ]->rg[!typ]) {
    if (typ)
      return Range{chl[L]->binaryFind<false>(gcdV), v[R]};
    else
      return Range{v[L], chl[R]->binaryFind<true>(gcdV)};
  }
  return v;
}
template <bool dir>
unsigned int SegmentTree::binaryFind(const unsigned int gVal) const
{
  if (val % gVal == 0) return rg[dir];
  if (rg.length() == 1) return rg[!dir];
  if (chl[!dir]->val % gVal == 0)
    return chl[dir]->binaryFind<dir>(gVal);
  else
    return chl[!dir]->binaryFind<dir>(gVal);
}

int main()
{
#ifndef APTEST
  ios_base::sync_with_stdio(false);
#endif
  unsigned int n;
  cin >> n;
  copy_n(istream_iterator<unsigned int>(cin), n, a);
  {
    SegmentTree* c = snodes;
    root->build(0, n, c, a);
  }
  for (unsigned int i = 0; i < n; ++i)
    rng[i] = root->binary(i, a[i]);
  const unsigned int len =
      max_element(rng, rng + n, [](const Range& a, const Range& b) {
        return a.length() < b.length();
      })->length();
  for (const Range* i = rng; i < rng + n; ++i)
    if (i->length() == len) hasAns[i->pos[L]] = true;
  cout << count(hasAns, hasAns + n, true) << " " << len - 1 << "\n";
  for (unsigned int i = 0; i < n; ++i)
    if (hasAns[i]) cout << (i + 1) << " ";
  cout.put('\n');
  return 0;
}
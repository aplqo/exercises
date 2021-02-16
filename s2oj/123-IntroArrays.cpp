#ifdef APTEST
#include "debug_tools/program.h"
#endif
#include <algorithm>
#include <climits>
#include <cmath>
#include <iostream>
#include <iterator>
using namespace std;
const unsigned int maxn = 1e5, maxq = 500;
constexpr unsigned int mod = 1e9 + 7, inf = UINT_MAX;

class Block {
 public:
  void init(unsigned int *l, unsigned int *r);
  void blockAdd(const unsigned int x);
  void add(const unsigned int l, const unsigned int r, const unsigned int x);
  unsigned int min(const unsigned int l, const unsigned int r);

  unsigned int minVal;

 private:
  unsigned int blockMin() const;
  void build();

  unsigned int *begin, *end;
  unsigned int order[maxq + 10], acc;
  unsigned int size, val;
} blk[maxq + 10];
unsigned int a[maxn + 10], blockSize;

void Block::init(unsigned int *l, unsigned int *r)
{
  begin = l;
  end = r;
  size = r - l;
  build();
}
void Block::blockAdd(const unsigned int x)
{
  acc = (acc + x) % mod;
  minVal = blockMin();
}
void Block::add(const unsigned int l, const unsigned int r,
                const unsigned int x)
{
  if (acc)
    for_each(begin, end, [this](unsigned int &i) { i = (i + acc) % mod; });
  for_each(begin + l, begin + r, [x](unsigned int &i) { i = (i + x) % mod; });
  build();
}
unsigned int Block::min(const unsigned int l, const unsigned int r)
{
  unsigned int ret = inf;
  for (const unsigned int *i = begin + l; i < begin + r; ++i)
    ret = std::min(ret, (*i + acc) % mod);
  return ret;
}
unsigned int Block::blockMin() const
{
  const unsigned int *p = lower_bound(order, order + size, (mod - acc));
  return ((p == order + size ? order[0] : *p) + acc) % mod;
}
void Block::build()
{
  copy(begin, end, order);
  sort(order, order + size);
  acc = 0;
  minVal = order[0];
}

static void init(const unsigned int n)
{
  blockSize = sqrt(n);
  for (unsigned int i = 0; i * blockSize < n; ++i)
    blk[i].init(a + i * blockSize, a + min(n, (i + 1) * blockSize));
}
unsigned int minVal(const unsigned int l, const unsigned int r)
{
  unsigned int ret = inf;
  const unsigned int indl = l / blockSize, indr = r / blockSize;
  if (indl != indr) {
    for (const Block *i = blk + indl + 1; i < blk + indr; ++i)
      ret = min(ret, i->minVal);
    ret = min(ret, blk[indl].min(l - indl * blockSize, blockSize));
    ret = min(ret, blk[indr].min(0, r - indr * blockSize));
  }
  else {
    const unsigned int base = indl * blockSize;
    ret = min(ret, blk[indl].min(l - base, r - base));
  }
  return ret;
}
void add(const unsigned int l, const unsigned int r, const unsigned int x)
{
  const unsigned int indl = l / blockSize, indr = r / blockSize;
  if (indl != indr) {
    for (Block *i = blk + indl + 1; i < blk + indr; ++i)
      i->blockAdd(x);
    blk[indl].add(l - indl * blockSize, blockSize, x);
    blk[indr].add(0, r - blockSize * indr, x);
  }
  else {
    const unsigned int base = indl * blockSize;
    blk[indl].add(l - base, r - base, x);
  }
}

int main()
{
#ifndef APTEST
  ios_base::sync_with_stdio(false);
#endif
  unsigned int n, q;
  cin >> n >> q;
  copy_n(istream_iterator<unsigned int>(cin), n, a);
  init(n);
  for (unsigned int i = 0; i < q; ++i) {
    unsigned int op, l, r;
    cin >> op >> l >> r;
    if (op == 1) {
      unsigned int x;
      cin >> x;
      add(l - 1, r, x);
    }
    else
      cout << minVal(l - 1, r) << "\n";
  }
  return 0;
}

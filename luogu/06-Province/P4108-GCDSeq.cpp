#ifdef APTEST
#include "debug_tools/program.h"
#endif
#include <algorithm>
#include <cmath>
#include <iostream>
#include <iterator>
#include <numeric>
#include <utility>
using namespace std;
const unsigned int maxn = 100000, maxb = 1000, maxs = 1300;

unsigned long long a[maxn + 1];
unsigned int bsize;
class block {
 public:
  void build(const unsigned int l, const unsigned int r)
  {
    this->l = l;
    this->r = r;
    init();
  }
  void init()
  {
    xen = xval;
    gsum = a[l];
    xsum = 0;
    for (unsigned int i = l; i < r; ++i) {
      xsum ^= a[i];
      *(xen++) = make_pair(xsum, i);
      gsum = gcd(gsum, a[i]);
    }
    sort(xval, xen);
  }
  unsigned int findXorVal(const unsigned long long xv) const
  {
    auto* p = lower_bound<const pair<unsigned long long, unsigned int>*>(
        xval, xen, make_pair(xv, 0u));
    return p == xen || p->first != xv ? r : p->second;
  }
  unsigned int findPos(const unsigned long long ex, unsigned long long gs,
                       unsigned long long xs) const
  {
    for (unsigned int i = l; i < r; ++i) {
      gs = gcd(gs, a[i]);
      xs ^= a[i];
      if (gs * xs == ex) return i;
    }
    return r;
  }

  unsigned long long gsum, xsum;
  unsigned int l, r;

 private:
  pair<unsigned long long, unsigned int> xval[maxs + 1], *xen = xval;
} blk[maxb + 1];

static void init(const unsigned int n)
{
  bsize = sqrt(n);
  for (unsigned int i = 0; i < (n - 1) / bsize; ++i)
    blk[i].build(i * bsize, (i + 1) * bsize);
  blk[(n - 1) / bsize].build(((n - 1) / bsize) * bsize, n);
}
unsigned int query(const unsigned int n, const unsigned long long x)
{
  if (auto p = blk[0].findPos(x, a[0], 0); p != blk[0].r) return p;
  unsigned long long gs = blk[0].gsum, xs = blk[0].xsum;
  for (unsigned int i = 1; i <= (n - 1) / bsize; ++i) {
    const unsigned long long ngs = gcd(gs, blk[i].gsum);
    if (ngs == gs) {
      const unsigned int p = blk[i].findXorVal((x / gs) ^ xs);
      if (p != blk[i].r) return p;
    }
    else if (auto p = blk[i].findPos(x, gs, xs); p != blk[i].r)
      return p;
    gs = ngs;
    xs ^= blk[i].xsum;
  }
  return n;
}
void modify(const unsigned int i, const unsigned long long x)
{
  a[i] = x;
  blk[i / bsize].init();
}

int main()
{
#ifndef APTEST
  ios_base::sync_with_stdio(false);
#endif
  unsigned int n, q;
  cin >> n;
  copy_n(istream_iterator<unsigned long long>(cin), n, a);
  init(n);
  cin >> q;
  for (unsigned int i = 0; i < q; ++i) {
    char str[15];
    cin >> str;
    if (str[0] == 'M') {
      unsigned int id;
      unsigned long long x;
      cin >> id >> x;
      modify(id, x);
    }
    else {
      unsigned long long x;
      cin >> x;
      const unsigned int p = query(n, x);
      if (p == n)
        cout << "no" << endl;
      else
        cout << p << endl;
    }
  }
  return 0;
}
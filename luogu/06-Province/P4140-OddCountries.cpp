#ifdef APTEST
#include "debug_tools/program.h"
#endif
#include <iostream>
#include <utility>
using namespace std;
const unsigned int maxx = 100000, maxl = 100000, maxp = 60;
const unsigned long long mod = 19961993;

const unsigned long long primes[] = {
    2,   3,   5,   7,   11,  13,  17,  19,  23,  29,  31,  37,  41,  43,  47,
    53,  59,  61,  67,  71,  73,  79,  83,  89,  97,  101, 103, 107, 109, 113,
    127, 131, 137, 139, 149, 151, 157, 163, 167, 173, 179, 181, 191, 193, 197,
    199, 211, 223, 227, 229, 233, 239, 241, 251, 257, 263, 269, 271, 277, 281};
const unsigned long long inv[] = {
    9980997,  13307996, 7984798,  11406854, 14517814, 18426456, 9393880,
    5253157,  16490343, 8260136,  2575742,  18343454, 3895024,  17640832,
    1698894,  3013132,  7443456,  4581442,  9236147,  18275065, 6562848,
    2779519,  7936697,  4037258,  6379607,  19566707, 13566404, 4104336,
    3662752,  13602421, 16661192, 1219054,  13259427, 9047523,  3751248,
    8196316,  14621843, 1714528,  12192356, 11884887, 8029406,  13455046,
    17976246, 13342473, 14084859, 15548287, 10217514, 9846724,  5364237,
    3486812,  1627803,  14950615, 1076789,  12406658, 19340609, 8652728,
    7791857,  7955334,  1657495,  8808852};

unsigned long long factor(unsigned long long v)
{
  unsigned long long ret = 0;
  for (unsigned int i = 0; i < maxp; ++i) {
    if (v % primes[i]) continue;
    while (!(v % primes[i]))
      v /= primes[i];
    ret |= 1ull << i;
  }
  return ret;
}
class STree {
 public:
  void build(const unsigned int l, const unsigned int r, STree*(&cur))
  {
    lv = l;
    rv = r;
    mv = (l + r) >> 1;
    if (r - l == 1) {
      val = 3;
      fact = 2;
      return;
    }
    lc = ++cur;
    rc = ++cur;
    lc->build(l, mv, cur);
    rc->build(mv, r, cur);
    pushup();
  }
  void assign(const unsigned int p, const unsigned long long v)
  {
    if (rv - lv == 1) {
      fact = factor(v);
      val = v;
      return;
    }
    if (p < mv)
      lc->assign(p, v);
    else
      rc->assign(p, v);
    pushup();
  }
  pair<unsigned long long, unsigned long long> query(const unsigned int l,
                                                     const unsigned int r) const
  {
    if (l == lv && r == rv) return make_pair(fact, val);
    if (r <= mv)
      return lc->query(l, r);
    else if (l >= mv)
      return rc->query(l, r);
    else {
      const auto lv = lc->query(l, mv), rv = rc->query(mv, r);
      return make_pair(lv.first | rv.first, (lv.second * rv.second) % mod);
    }
  }

 private:
  void pushup()
  {
    val = (lc->val * rc->val) % mod;
    fact = lc->fact | rc->fact;
  }

  unsigned int lv, mv, rv;
  unsigned long long fact, val;
  STree *lc, *rc;
} SNodes[maxx * 4 + 1], *SRoot = SNodes;

unsigned long long phi(const pair<unsigned long long, unsigned long long> v)
{
  unsigned long long ret = v.second;
  for (unsigned int i = 0; i < maxp; ++i)
    if (v.first & (1ull << i)) ret = (ret * inv[i]) % mod;
  return ret;
}

int main()
{
#ifndef APTEST
  ios_base::sync_with_stdio(false);
#endif
  unsigned int x;
  cin >> x;
  {
    STree* c = SNodes;
    SRoot->build(1, maxl + 1, c);
  }
  for (unsigned int i = 0; i < x; ++i) {
    unsigned int a, b;
    unsigned long long c;
    cin >> a >> b >> c;
    if (!a)
      cout << phi(SRoot->query(b, c + 1)) << endl;
    else
      SRoot->assign(b, c);
  }
  return 0;
}
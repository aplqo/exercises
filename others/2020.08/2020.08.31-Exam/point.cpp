#ifdef APTEST
#include "debug_tools/program.h"
#endif
#include <algorithm>
#include <fstream>
#include <iostream>
#include <utility>
#include <vector>
using namespace std;
const unsigned int maxn = 5e5;

unsigned int gcd(unsigned int a, unsigned int b)
{
  if (a < b) swap(a, b);
  while (b) {
    const unsigned int t = a;
    a = b;
    b = t % b;
  }
  return a;
}
struct Range {
  Range() {}
  Range(const unsigned int l, const unsigned int r)
  {
    rng[0] = l;
    rng[1] = r;
  }
  unsigned int rng[2];
  inline unsigned int& operator[](const unsigned int ind) { return rng[ind]; }
  inline unsigned int operator[](const unsigned int ind) const
  {
    return rng[ind];
  }
  inline unsigned int length() const { return rng[1] - rng[0]; }
};
class STree {
 public:
  void build(const unsigned int l, const unsigned int r, STree*(&cur),
             const unsigned int val[]);
  Range query(const unsigned int pos, const unsigned int val) const;

 private:
  template <unsigned int side>
  unsigned int binaryFind(const unsigned int val) const;

  Range range;
  unsigned int mv;
  unsigned int val;
  STree* chl[2];
} SNodes[maxn * 4 + 10], *root = SNodes;
unsigned int a[maxn + 1], len[maxn + 1], lp[maxn + 1];
vector<unsigned int> ansPos;

void STree::build(const unsigned int l, const unsigned int r, STree*(&cur),
                  const unsigned int val[])
{
  range = Range(l, r);
  mv = (l + r) >> 1;
  if (r - l == 1) {
    this->val = val[l];
    return;
  }
  chl[0] = ++cur;
  chl[1] = ++cur;
  chl[0]->build(l, mv, cur, val);
  chl[1]->build(mv, r, cur, val);
  this->val = gcd(chl[0]->val, chl[1]->val);
}
Range STree::query(const unsigned int pos, const unsigned int val) const
{
  if (range[1] - range[0] == 1) return Range(range[0], range[1]);
  Range ret = chl[pos >= mv]->query(pos, val);
  const unsigned int typ = pos < mv;
  if (ret[typ] == mv) {
    if (this->val % val) {
      if (typ)
        ret[1] += chl[1]->binaryFind<0>(val);
      else
        ret[0] -= chl[0]->binaryFind<1>(val);
    }
    else
      ret[typ] = range[typ];
  }
  return ret;
}
template <unsigned int side>
unsigned int STree::binaryFind(const unsigned int val) const
{
  if (range[1] - range[0] == 1) return !(this->val % val);
  if (!(this->val % val)) return range.length();
  if (!(chl[side]->val % val))
    return chl[side]->range.length() + chl[!side]->binaryFind<side>(val);
  else
    return chl[side]->binaryFind<side>(val);
}

static unsigned int findMax(const unsigned int n)
{
  unsigned int ret = 0;
  for (unsigned int i = 0; i < n; ++i) {
    const Range t = root->query(i, a[i]);
    len[i] = t.length();
    lp[i] = t[0];
    ret = max(ret, len[i]);
  }
  return ret;
}
unsigned int count(const unsigned int n, const unsigned int mv)
{
  static bool vis[maxn + 1];
  unsigned int ret = 0;
  for (unsigned int i = 0; i < n; ++i) {
    if (len[i] != mv || vis[lp[i]]) continue;
    ansPos.push_back(lp[i]);
    vis[lp[i]] = true;
    ++ret;
  }
  sort(ansPos.begin(), ansPos.end());
  return ret;
}

int main()
{
#if !defined APTEST && !defined APDEBUG
  ios_base::sync_with_stdio(false);
  ifstream in("point.in");
  ofstream out("point.out");
#else
  istream& in = cin;
  ostream& out = cout;
#endif
  unsigned int n;
  in >> n;
  for (unsigned int* i = a; i < a + n; ++i)
    in >> *i;
  {
    STree* c = SNodes;
    root->build(0, n, c, a);
  }
  const unsigned int mv = findMax(n);
  out << count(n, mv) << " " << mv - 1 << "\n";
  for (vector<unsigned int>::const_iterator it = ansPos.cbegin();
       it != ansPos.cend(); ++it)
    out << *it + 1 << " ";
  out << "\n";
  return 0;
}
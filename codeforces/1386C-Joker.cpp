#ifdef APTEST
#include "debug_tools/program.h"
#endif
#include <algorithm>
#include <iostream>
#include <numeric>
#include <stack>
using namespace std;
const unsigned int maxn = 200000, maxm = 200000;

struct Edge {
  unsigned int from, to;
} ed[maxm + 10];
unsigned int last[maxm + 10], n, m;

namespace FindUnion {
unsigned int fa[maxn * 2 + 10], siz[maxn * 2 + 10];
stack<unsigned int> stk;

void init(const unsigned int n)
{
  iota(fa, fa + 1 + n, 0);
  fill(siz, siz + 1 + n, 1);
}
unsigned int find(unsigned int x)
{
  while (fa[x] != x)
    x = fa[x];
  return fa[x];
}
void merge(unsigned int x, unsigned int y)
{
  x = find(x);
  y = find(y);
  if (x == y) return;
  if (siz[x] < siz[y]) swap(x, y);
  fa[y] = x;
  siz[x] += siz[y];
  stk.push(y);
}
inline bool isConnect(const unsigned int x, const unsigned int y)
{
  return find(x) == find(y);
}
class Checkpoint {
 public:
  Checkpoint() : stat(stk.size()){};
  ~Checkpoint()
  {
    while (stk.size() > stat) {
      const unsigned int p = stk.top();
      siz[fa[p]] -= siz[p];
      fa[p] = p;
      stk.pop();
    }
  }

 private:
  const size_t stat;
};
}  // namespace FindUnion
using FindUnion::Checkpoint;
inline bool tryAdd(const Edge e)
{
  if (FindUnion::isConnect(e.from, e.to)) return false;
  FindUnion::merge(e.from, e.to + n);
  FindUnion::merge(e.to, e.from + n);
  return true;
}
unsigned int findPos(const unsigned int l, const unsigned int r)
{
  Checkpoint ck;
  unsigned int cpos = r + 1;
  while (cpos > l)
    if (!tryAdd(ed[--cpos])) break;
  return cpos;
}
void conquer(const unsigned int leftL, const unsigned int leftR,
             const unsigned int rightL, const unsigned int rightR)
{
  if (leftR - leftL <= 1) {
    if (leftL != leftR) last[leftL] = findPos(max(leftL, rightL - 1), rightR);
    return;
  }
  const unsigned int mid = (leftL + leftR) >> 1;
  unsigned int mpos = rightR;
  if (Checkpoint ini; all_of(ed + leftL, ed + mid, tryAdd)) {
    last[mid] = mpos = findPos(max(mid, rightL - 1), rightR);
    if (tryAdd(ed[mid])) conquer(mid + 1, leftR, mpos, rightR);
  }
  if (Checkpoint ini; all_of(ed + mpos + 1, ed + rightR + 1, tryAdd))
    conquer(leftL, mid, rightL, mpos);
}

int main()
{
#ifndef APTEST
  ios_base::sync_with_stdio(false);
#endif
  unsigned int q;
  cin >> n >> m >> q;
  for (Edge* i = ed + 1; i < ed + 1 + m; ++i) {
    cin >> i->from >> i->to;
    --(i->from);
    --(i->to);
  }
  FindUnion::init(n * 2);
  fill(last, last + 1 + m, m + 1);
  conquer(1, m + 1, 1, m);
  for (unsigned int i = 0; i < q; ++i) {
    unsigned int l, r;
    cin >> l >> r;
    cout << (r < last[l] ? "YES" : "NO") << "\n";
  }
  return 0;
}
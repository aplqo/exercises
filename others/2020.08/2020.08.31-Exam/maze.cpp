#ifdef APTEST
#include "debug_tools/program.h"
#endif
#include <algorithm>
#include <climits>
#include <iostream>
using namespace std;
constexpr unsigned int inf = UINT_MAX / 4;
const unsigned int maxn = 5, maxm = 200000;

unsigned int n, m;
bool ban[maxm + 3][maxn + 3];
struct Matrix {
  Matrix()
  {
    for (unsigned int i = 0; i < n; ++i)
      fill(val[i], val[i] + n, inf);
  }
  Matrix(const bool ban[]) : Matrix()
  {
    for (unsigned int i = 0; i < n; ++i) {
      if (ban[i]) continue;
      for (int j = i; j >= 0 && !ban[j]; --j)
        val[i][j] = i - j;
      for (unsigned int j = i; j < n && !ban[j]; ++j)
        val[i][j] = j - i;
    }
  }
  inline Matrix operator*(const Matrix& r) const
  {
    Matrix ret;
    for (unsigned int k = 0; k < n; ++k)
      for (unsigned int i = 0; i < n; ++i)
        for (unsigned int j = 0; j < n; ++j)
          if (val[i][k] != inf && r[k][j] != inf)
            ret[i][j] = min(ret[i][j], val[i][k] + r[k][j] + 1);
    return ret;
  }
  const unsigned int* operator[](const unsigned int ind) const
  {
    return val[ind];
  }
  unsigned int* operator[](const unsigned int ind) { return val[ind]; }

  unsigned int val[maxn][maxn];
};
class STree {
 public:
  void build(const unsigned int l, const unsigned int r, STree*(&cur));
  unsigned int query(const unsigned int x1, const unsigned int y1,
                     const unsigned int x2, const unsigned int y2) const;
  void change(const unsigned int x, const unsigned int y);

 private:
  Matrix queryDist(const unsigned int l, const unsigned int r) const;

  unsigned int lv, mv, rv;
  Matrix val;
  STree *lc, *rc;
} SNodes[maxm * 4 + 1], *root = SNodes;
void STree::build(const unsigned int l, const unsigned int r, STree*(&cur))
{
  lv = l;
  rv = r;
  mv = (l + r) >> 1;
  if (r - l == 1) {
    new (&val) Matrix(ban[l]);
    return;
  }
  lc = ++cur;
  rc = ++cur;
  lc->build(l, mv, cur);
  rc->build(mv, r, cur);
  val = lc->val * rc->val;
}
unsigned int STree::query(const unsigned int x1, const unsigned int y1,
                          const unsigned int x2, const unsigned int y2) const
{
  return queryDist(y1, y2 + 1)[x1][x2];
}
void STree::change(const unsigned int x, const unsigned int y)
{
  if (rv - lv == 1) {
    ban[x][y] = !ban[x][y];
    new (&val) Matrix(ban[x]);
    return;
  }
  if (x < mv)
    lc->change(x, y);
  else
    rc->change(x, y);
  val = lc->val * rc->val;
}
Matrix STree::queryDist(const unsigned int l, const unsigned int r) const
{
  if (l == lv && r == rv) return val;
  if (r <= mv)
    return lc->queryDist(l, r);
  else if (l >= mv)
    return rc->queryDist(l, r);
  else
    return lc->queryDist(l, mv) * rc->queryDist(mv, r);
}

int main()
{
#ifndef APTEST
  ios_base::sync_with_stdio(false);
#endif
  unsigned int q;
  cin >> n >> m >> q;
  for (unsigned int i = 0; i < n; ++i)
    for (unsigned int j = 0; j < m; ++j) {
      unsigned int v;
      cin >> v;
      ban[j][i] = !v;
    }
  {
    STree* c = SNodes;
    root->build(0, m, c);
  }
  for (unsigned int i = 0; i < q; ++i) {
    unsigned int opt, a, b;
    cin >> opt >> a >> b;
    if (opt == 1)
      root->change(b - 1, a - 1);
    else {
      unsigned int c, d;
      cin >> c >> d;
      const unsigned int v = root->query(a - 1, b - 1, c - 1, d - 1);
      if (v != inf)
        cout << v << "\n";
      else
        cout << "-1"
             << "\n";
    }
  }
  return 0;
}
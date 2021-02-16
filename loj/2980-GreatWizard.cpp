#ifdef APTEST
#include "debug_tools/program.h"
#endif
#include <algorithm>
#include <array>
#include <iostream>
#include <numeric>
#include <valarray>
using namespace std;
const unsigned int maxn = 2.5e5, maxl = 3;
constexpr unsigned long long mod = 998244353;

typedef array<unsigned long long, maxl + 1> Vector;
inline Vector operator+(const Vector& a, const Vector& b)
{
  Vector ret;
  for (unsigned int i = 0; i <= maxl; ++i)
    ret[i] = a[i] + b[i];
  return ret;
}
inline Vector operator%(const Vector& a, const unsigned long long b)
{
  Vector ret;
  for (unsigned int i = 0; i <= maxl; ++i)
    ret[i] = a[i] % b;
  return ret;
}

struct Matrix {
  Matrix() = default;
  Matrix(const unsigned int siz) { setUnit(siz); }
  inline void setUnit(const unsigned int siz)
  {
    for (unsigned int i = 0; i < siz; ++i)
      vec[i][i] = 1;
  }
  inline const Vector& operator[](const unsigned int i) const { return vec[i]; }
  inline Vector& operator[](const unsigned int i) { return vec[i]; }
  Vector operator*(const Vector& v) const
  {
    Vector ret{};
    for (unsigned int i = 0; i <= maxl; ++i)
      for (unsigned int j = 0; j <= maxl; ++j)
        ret[j] = (ret[j] + v[i] * vec[i][j]) % mod;
    return ret;
  }
  Matrix operator*(const Matrix& r) const
  {
    Matrix ret;
    for (unsigned int i = 0; i <= maxl; ++i)
      ret[i] = *this * r[i];
    return ret;
  }

 private:
  Vector vec[maxl + 1] = {};
};
class SegmentTree {
 public:
  void build(const unsigned int l, const unsigned int r, SegmentTree*(&cur),
             const Vector val[]);
  void transform(const unsigned int l, const unsigned int r, const Matrix& m);
  Vector vectorSum(const unsigned int l, const unsigned int r);

 private:
  inline void push(const Matrix& m);
  inline void pushdown();

  unsigned int lv, mv, rv;
  SegmentTree *lc, *rc;
  Matrix val;
  Vector sum;
  bool dirty;
} nodes[maxn * 4 + 10], *root = nodes;
Vector val[maxn + 10];

void SegmentTree::build(const unsigned int l, const unsigned int r,
                        SegmentTree*(&cur), const Vector val[])
{
  lv = l;
  mv = (l + r) >> 1;
  rv = r;
  dirty = false;
  if (r - l == 1) {
    sum = val[l];
    return;
  }
  lc = ++cur;
  rc = ++cur;
  lc->build(l, mv, cur, val);
  rc->build(mv, r, cur, val);
  this->sum = (lc->sum + rc->sum) % mod;
}
void SegmentTree::transform(const unsigned int l, const unsigned int r,
                            const Matrix& m)
{
  if (l == lv && r == rv) {
    push(m);
    return;
  }
  pushdown();
  if (r <= mv)
    lc->transform(l, r, m);
  else if (l >= mv)
    rc->transform(l, r, m);
  else {
    lc->transform(l, mv, m);
    rc->transform(mv, r, m);
  }
  this->sum = (lc->sum + rc->sum) % mod;
}
Vector SegmentTree::vectorSum(const unsigned int l, const unsigned int r)
{
  if (l == lv && r == rv) return sum;
  pushdown();
  if (r <= mv)
    return lc->vectorSum(l, r);
  else if (l >= mv)
    return rc->vectorSum(l, r);
  else
    return (lc->vectorSum(l, mv) + rc->vectorSum(mv, r)) % mod;
}
inline void SegmentTree::push(const Matrix& m)
{
  sum = m * sum;
  val = dirty ? m * val : m;
  dirty = true;
}
inline void SegmentTree::pushdown()
{
  if (!dirty) return;
  lc->push(val);
  rc->push(val);
  dirty = false;
}

int main()
{
#ifndef APTEST
  ios_base::sync_with_stdio(false);
#endif
  unsigned int n, m;
  cin >> n;
  for (Vector* i = val + 1; i < val + 1 + n; ++i) {
    (*i)[maxl] = 1;
    for (unsigned int j = 0; j < maxl; ++j)
      cin >> (*i)[j];
  }
  {
    SegmentTree* c = nodes;
    root->build(1, n + 1, c, val);
  }
  cin >> m;
  for (unsigned int i = 0; i < m; ++i) {
    static const auto read = []() {
      unsigned long long v;
      cin >> v;
      return v;
    };
    unsigned int opt, l, r;
    cin >> opt >> l >> r;
    Matrix mat(maxl + 1);
    switch (opt) {
      case 1:
      case 2:
      case 3:
        mat[opt % 3][opt - 1] = 1;
        root->transform(l, r + 1, mat);
        break;
      case 4:
        mat[maxl][0] = read();
        root->transform(l, r + 1, mat);
        break;
      case 5:
        mat[1][1] = read();
        root->transform(l, r + 1, mat);
        break;
      case 6:
        mat[2][2] = 0;
        mat[maxl][2] = read();
        root->transform(l, r + 1, mat);
        break;
      case 7:
        const Vector&& v = root->vectorSum(l, r + 1);
        for (unsigned int j = 0; j < maxl; ++j)
          cout << v[j] << " ";
        cout.put('\n');
        break;
    }
  }
  return 0;
}
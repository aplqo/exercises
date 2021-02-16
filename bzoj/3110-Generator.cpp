#include <algorithm>
#include <fstream>
#include <random>

#include "debug_tools/generator.h"
using namespace std;
using namespace apdebug::generator;
const int maxn = 5e4, maxm = 5e4;
const long long maxv = (1ull << 63) - 1;

class STree {
 public:
  void build(const unsigned int l, const unsigned int r, STree*(&cur));
  void increase(const unsigned int l, const unsigned int r);
  unsigned int query(const unsigned int l, const unsigned int r) const;

 private:
  unsigned int lv, mv, rv;
  STree *lc, *rc;
  unsigned int val, laz;
} SNodes[maxn * 4 + 1], *root = SNodes;
void STree::build(const unsigned int l, const unsigned int r, STree*(&cur))
{
  lv = l;
  rv = r;
  mv = (l + r) >> 1;
  val = laz = 0;
  if (r - l == 1) return;
  lc = ++cur;
  rc = ++cur;
  lc->build(l, mv, cur);
  rc->build(mv, r, cur);
}
void STree::increase(const unsigned int l, const unsigned int r)
{
  val += (r - l);
  if (l == lv && r == rv) {
    ++laz;
    return;
  }
  if (r <= mv)
    lc->increase(l, r);
  else if (l >= mv)
    rc->increase(l, r);
  else {
    lc->increase(l, mv);
    rc->increase(mv, r);
  }
}
unsigned int STree::query(const unsigned int l, const unsigned int r) const
{
  if (l == lv && r == rv) return val;
  if (r <= mv)
    return lc->query(l, r) + laz * (r - l);
  else if (l >= mv)
    return rc->query(l, r) + laz * (r - l);
  else
    return lc->query(l, mv) + rc->query(mv, r) + laz * (r - l);
}

void createQuery(ofstream& in, const unsigned int n, const unsigned int m,
                 const unsigned long long mv)
{
  uniform_int_distribution<unsigned int> dis(1, 2), disp(1, n);
  uniform_int_distribution<unsigned long long> disv(1, mv);
  for (unsigned int i = 0; i < m; ++i) {
    unsigned int op = dis(default_rnd), p1 = disp(default_rnd),
                 p2 = disp(default_rnd);
    if (p1 > p2) swap(p1, p2);
    const unsigned int v = root->query(p1, p2 + 1);
    if (!v) op = 1;
    in << op << " " << p1 << " " << p2 << " ";
    if (op == 1) {
      in << disv(default_rnd) << endl;
      root->increase(p1, p2 + 1);
    }
    else
      in << GetRandom(1u, root->query(p1, p2 + 1)) << endl;
  }
}
int main(int, char* argv[])
{
  Init();
  const unsigned int n = GetRandom(1, MayBe(atoi(argv[1]), maxn));
  const unsigned int m = GetRandom(1, MayBe(atoi(argv[2]), maxm));
  ofstream in(argv[4]);
  in << n << " " << m << endl;
  {
    STree* c = SNodes;
    root->build(1, n + 1, c);
  }
  createQuery(in, n, m, MayBe(atoll(argv[3]), maxv));
  return 0;
}
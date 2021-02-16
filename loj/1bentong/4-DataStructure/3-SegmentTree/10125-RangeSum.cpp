#ifdef APDEBUG
#define COLOR
#define APINPROG
#include "debug_tools/time.cpp"
#endif
#include <iostream>
using namespace std;
typedef long long num;
const int maxn = 1e6 + 5;

class node {
 public:
  void build(int, int, num[maxn], node *(&));
  num query(int, int);
  void change(int, num);

 private:
  node *lc = nullptr, *rc = nullptr;
  int lv, mv, rv;
  num val;
} nds[maxn * 4];
void node::build(int l, int r, num dat[maxn], node *(&cur))
{
  lv = l;
  rv = r;
  mv = (l + r) >> 1;
  if (r - l == 1) {
    val = dat[l];
    return;
  }
  lc = ++cur;
  lc->build(lv, mv, dat, cur);
  rc = ++cur;
  rc->build(mv, rv, dat, cur);
  val = lc->val + rc->val;
}
void node::change(int pos, num val)
{
  if (rv - lv == 1 && pos == lv) {
    this->val += val;
    return;
  }
  if (pos < mv)
    lc->change(pos, val);
  else
    rc->change(pos, val);
  this->val = lc->val + rc->val;
}
num node::query(int l, int r)
{
  if (rv - lv == 1 || (l == lv && r == rv)) return val;
  if (r <= mv)
    return lc->query(l, r);
  else if (l >= mv)
    return rc->query(l, r);
  else
    return lc->query(l, mv) + rc->query(mv, r);
}
num dat[maxn];
node *tree = nds, *cur = nds;

int main()
{
  ios_base::sync_with_stdio(false);
  int n, q;
  cin >> n >> q;
  for (num *i = dat; i < dat + n; ++i)
    cin >> *i;
  tree->build(0, n, dat, cur);
  for (int i = 0; i < q; ++i) {
    char op;
    int x;
    cin >> op >> x;
    switch (op) {
      case '1':
        num k;
        cin >> k;
        tree->change(x - 1, k);
        break;
      case '2':
        int y;
        cin >> y;
        cout << tree->query(x - 1, y) << endl;
        break;
    }
  }
  return 0;
}
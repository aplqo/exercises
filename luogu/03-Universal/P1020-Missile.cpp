#ifdef APDEBUG
#define COLOR
#define APINPROG
#include "debug_tools/time.cpp"
#endif
#include <algorithm>
#include <climits>
#include <iostream>
#include <set>
using namespace std;
const int maxn = 100005;
typedef set<int> setType;

class node {
 public:
  node() = delete;
  node(int lv, int rv) : lr(lv), rr(rv), mid((lv + rv) >> 1)
  {
    if (rr - lr == 1) return;
    lc = new node(lr, mid);
    rc = new node(mid, rr);
  }
  void update(int pos, int val)
  {
    if (rr - lr == 1) {
      this->val = val;
      return;
    }
    if (pos < mid)
      lc->update(pos, val);
    else
      rc->update(pos, val);
    this->val = max(lc->val, rc->val);
  }
  int query(int lval, int rval)
  {
    if (lr == lval && rr == rval) return this->val;
    if (rval < mid)
      return lc->query(lval, rval);
    else if (lval >= mid)
      return rc->query(lval, rval);
    else
      return max(lc->query(lval, mid), rc->query(mid, rval));
  }
  ~node()
  {
    delete lc;
    delete rc;
  }

 private:
  int val = 0, lr, rr, mid;
  node *lc = nullptr, *rc = nullptr;
} * tree;
int a[maxn], cur = 0, maxh;
setType equ;

int main()
{
  ios_base::sync_with_stdio(false);
  while (cin)
    cin >> a[cur++];
  if (cin.fail()) --cur;
  maxh = *max_element(a, a + cur) + 1;
  tree = new node(0, maxh);
  for (int* i = a; i < a + cur; ++i)
    tree->update(*i, tree->query(*i, maxh) + 1);
  cout << tree->query(0, maxh) << endl;
  delete tree;
  equ.insert(INT_MAX);
  for (int* i = a; i < a + cur; ++i) {
    setType::iterator it = equ.lower_bound(*i);
    if (it == equ.end())
      equ.insert(*i);
    else {
      equ.erase(*it);
      equ.insert(*i);
    }
  }
  cout << equ.size();
  return 0;
}
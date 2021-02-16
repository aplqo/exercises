#ifdef APDEBUG
#define COLOR
#define APINPROG
#undef _MSC_VER
#include "debug_tools/time.cpp"
#endif
#include <cmath>
#include <iostream>
#include <numeric>
using namespace std;
const int maxn = 1e5 + 5, maxl = 33;
typedef unsigned long long num;

struct door {
  num t;
  enum { AND, OR, XOR } op;
} dr[maxn];
num ans, m;
int n, lg;

num fun(const num& va, const door& dr)
{
  switch (dr.op) {
    case door::AND:
      return va & (dr.t);
    case door::OR:
      return va | (dr.t);
    case door::XOR:
      return va ^ (dr.t);
  }
}

int main()
{
  ios_base::sync_with_stdio(false);
  cin >> n >> m;
  for (door* i = dr; i < dr + n; ++i) {
    char c[5];
    cin >> c >> i->t;
    switch (c[0]) {
      case 'A':
        i->op = door::AND;
        break;
      case 'O':
        i->op = door::OR;
        break;
      case 'X':
        i->op = door::XOR;
        break;
    }
  }
  lg = log2(m);
  num init = accumulate(dr, dr + n, num(0), fun), val = num(1) << lg;
  for (int i = lg; i >= 0; --i, val >>= 1) {
    num tv = accumulate(dr, dr + n, val, fun), bi = tv & val;
    if (bi == (init & val)) continue;
    if (bi && ((ans | val) <= m)) ans |= val;
  }
  cout << accumulate(dr, dr + n, ans, fun) << endl;
  return 0;
}
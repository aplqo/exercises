#ifdef APDEBUG
#define APINPROG
#include "time.cpp"
#endif
#include <algorithm>
#include <cstdlib>
#include <iostream>
using namespace std;

enum class oper { NUL = 0, ADD = 1, DEC = 2, MUL = 3, DIV = 4 };
const char out[] = {' ', '+', '-', '*', '/'};
struct num {
  int nu;
  oper op;
  num *l, *r;
  bool vis;
  const static oper posop[4];
} n[20], *res;
const oper num::posop[4] = {oper::ADD, oper::DEC, oper::MUL, oper::DIV};

inline void insert(num *l, num *r, int val, oper op)
{
  *res = {val, op, l, r, false};
  ++res;
}
inline void remove() { --res; }
void print(num *n)
{
  if (n->op == oper::NUL) return;
  print(n->l);
  print(n->r);
  cout << n->l->nu << out[static_cast<int>(n->op)];
  if (n->r->nu < 0)
    cout << "(" << n->r->nu << ")";
  else
    cout << n->r->nu;
  cout << "=" << n->nu << endl;
}
template <int dep>
inline void solve()
{
  num *const en = res;
  for (num *i = n; i < en; ++i) {
    if (i->vis) continue;
    i->vis = true;
    for (num *j = n; j < en; ++j) {
      if (j->vis) continue;
      if (j->nu > i->nu) continue;
      j->vis = true;
      for (const auto k : num::posop) {
        int a;
        switch (k) {
          case oper::ADD:
            a = i->nu + j->nu;
            break;
          case oper::DEC:
            a = i->nu - j->nu;
            break;
          case oper::MUL:
            a = i->nu * j->nu;
            break;
          case oper::DIV:
            if (j->nu == 0 || i->nu % j->nu) continue;
            a = i->nu / j->nu;
            break;
        }
        insert(i, j, a, k);
        solve<dep + 1>();
        remove();
      }
      j->vis = false;
    }
    i->vis = false;
  }
}
template <>
inline void solve<3>()
{
  num *i = find_if(n, n + 7, [](const num &i) -> bool { return !(i.vis); });
  if (i->nu == 24) {
    print(i);
    exit(0);
  }
}

int main()
{
  for (num *i = n; i < n + 4; ++i) {
    cin >> i->nu;
    i->op = oper::NUL;
  }
  res = n + 4;
  solve<0>();
  cout << "No answer!" << endl;
  return 0;
}
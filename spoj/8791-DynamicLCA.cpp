#ifdef APTEST
#include "debug_tools/program.h"
#endif
#include <algorithm>
#include <iostream>
using namespace std;
const unsigned int maxn = 100000;

struct SplayNode {
  inline bool isRoot() const;
  void rotate();
  void splay();

  SplayNode *fa = nullptr, *chl[2] = {};
};
SplayNode vertex[maxn + 10];

inline bool SplayNode::isRoot() const
{
  return fa ? fa->chl[1] != this && fa->chl[0] != this : true;
}
void SplayNode::rotate()
{
  SplayNode *const f = fa, *const ff = fa->fa;
  if (!f->isRoot()) ff->chl[ff->chl[1] == f] = this;
  const bool typ = f->chl[1] == this;
  fa = ff;
  f->fa = this;
  if (chl[!typ]) chl[!typ]->fa = f;
  f->chl[typ] = chl[!typ];
  chl[!typ] = f;
}
void SplayNode::splay()
{
  while (!isRoot()) {
    SplayNode* const ff = fa->fa;
    if (!fa->isRoot()) {
      if ((ff->chl[1] == fa) ^ (fa->chl[1] == this))
        this->rotate();
      else
        fa->rotate();
    }
    rotate();
  }
}
void access(SplayNode* x)
{
  for (SplayNode* lst = nullptr; x; x = x->fa) {
    x->splay();
    x->chl[1] = lst;
    lst = x;
  }
}

void link(SplayNode* const rt, SplayNode* const father)
{
  access(rt);
  rt->fa = father;
}
void cut(SplayNode* const x)
{
  access(x);
  x->splay();
  x->chl[0]->fa = nullptr;
  x->chl[0] = nullptr;
}
SplayNode* lca(SplayNode* const x, SplayNode* const y)
{
  access(x);
  access(y);
  x->splay();
  return x->fa ? x->fa : x;
}

int main()
{
#ifndef APTEST
  ios_base::sync_with_stdio(false);
#endif
  unsigned int n, m;
  cin >> n >> m;
  for (unsigned int i = 0; i < m; ++i) {
    static char op[10];
    unsigned int a, b;
    cin >> op;
    switch (op[1]) {
      case 'i':  // link
        cin >> a >> b;
        link(vertex + a, vertex + b);
        break;
      case 'u':  // cut
        cin >> a;
        cut(vertex + a);
        break;
      case 'c':  // lca
        cin >> a >> b;
        cout << (lca(vertex + a, vertex + b) - vertex) << "\n";
        break;
    }
  }
  return 0;
}
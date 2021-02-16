#ifdef APTEST
#include "debug_tools/program.h"
#endif
#include <algorithm>
#include <climits>
#include <iostream>
#include <iterator>
#include <numeric>
#include <utility>
using namespace std;
const unsigned int maxn = 5e4, maxm = 1e5;
constexpr unsigned int inf = UINT_MAX >> 1;

struct Edge {
  unsigned int from, to;
  unsigned int a, b;
} ed[maxm + 10];
namespace LinkCutTree {
typedef pair<unsigned int, const Edge*> MPos;
struct SplayNode {
  inline void pushRev();
  inline void pushdown();
  inline void pushup();
  inline bool isRoot();
  void pushPath();
  void rotate();
  void splay();

  bool rev = false;
  MPos val, maxVal;
  SplayNode *fa = nullptr, *chl[2] = {};
};
SplayNode vertex[maxn + 10], virt[maxm + 10], *cur = virt;

static SplayNode* maybe(SplayNode* const x)
{
  static SplayNode nil;
  return x ? x : &nil;
}

inline void SplayNode::pushRev()
{
  swap(chl[0], chl[1]);
  rev ^= 1;
}
inline void SplayNode::pushdown()
{
  if (!rev) return;
  maybe(chl[0])->pushRev();
  maybe(chl[1])->pushRev();
  rev = false;
}
inline void SplayNode::pushup()
{
  maxVal = max({val, maybe(chl[0])->maxVal, maybe(chl[1])->maxVal});
}
inline bool SplayNode::isRoot()
{
  return fa ? fa->chl[0] != this && fa->chl[1] != this : true;
}
void SplayNode::pushPath()
{
  if (!isRoot()) fa->pushPath();
  pushdown();
}
void SplayNode::rotate()
{
  SplayNode *const ff = fa->fa, *const f = fa;
  if (!f->isRoot()) ff->chl[ff->chl[1] == f] = this;
  const bool typ = fa->chl[1] == this;
  f->fa = this;
  fa = ff;
  if (chl[!typ]) chl[!typ]->fa = f;
  f->chl[typ] = chl[!typ];
  chl[!typ] = f;
  f->pushup();
  this->pushup();
}
void SplayNode::splay()
{
  pushPath();
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
    x->pushup();
    lst = x;
  }
}
void makeRoot(SplayNode* const x)
{
  access(x);
  x->splay();
  x->pushRev();
}
void split(SplayNode* const x, SplayNode* const rt)
{
  makeRoot(x);
  access(rt);
  rt->splay();
}
void link(SplayNode* const x, SplayNode* const y, const Edge* id)
{
  makeRoot(x);
  cur->val = MPos(id->b, id);
  cur->maxVal = cur->val;
  cur->fa = y;
  x->fa = cur;
  ++cur;
}
void cut(SplayNode* const x, SplayNode* const y)
{
  split(x, y);
  x->fa = x->chl[1] = nullptr;
  y->chl[0] = nullptr;
  x->pushup();
  y->pushup();
}
SplayNode* findRoot(SplayNode* x)
{
  access(x);
  x->splay();
  while (x->chl[0])
    x = x->chl[0];
  x->splay();
  return x;
}
MPos maxPath(SplayNode* const x, SplayNode* const y)
{
  split(x, y);
  return y->maxVal;
}

inline void link(const Edge& e)
{
  SplayNode *const px = vertex + e.from, *const py = vertex + e.to;
  if (findRoot(px) == findRoot(py)) {
    const auto mv = maxPath(px, py);
    if (mv.first > e.b)
      cut(vertex + mv.second->from, vertex + mv.second->to);
    else
      return;
  }
  link(px, py, &e);
}
unsigned int maxPath(const unsigned int p)
{
  if (findRoot(vertex + 1) != findRoot(vertex + p)) return inf;
  return maxPath(vertex + 1, vertex + p).first;
}
}  // namespace LinkCutTree

unsigned int solve(const unsigned int n, const unsigned int m)
{
  sort(ed, ed + m, [](const Edge& a, const Edge& b) { return a.a < b.a; });
  if (n == 1) return 0;
  unsigned int ans = inf;
  for (Edge* i = ed; i < ed + m; ++i) {
    if (i->from == i->to) continue;
    LinkCutTree::link(*i);
    ans = min(ans, i->a + LinkCutTree::maxPath(n));
  }
  return ans;
}

int main()
{
#ifndef APTEST
  ios_base::sync_with_stdio(false);
#endif
  unsigned int n, m;
  cin >> n >> m;
  for (Edge* i = ed; i < ed + m; ++i)
    cin >> i->from >> i->to >> i->a >> i->b;
  const unsigned int v = solve(n, m);
  if (v == inf)
    cout << "-1\n";
  else
    cout << v << "\n";
  return 0;
}
#ifdef APTEST
#include "debug_tools/judge.h"
#endif
#include <algorithm>
#include <iostream>
#include <vector>
const int maxn = 1e5, maxm = maxn, maxc = 2;

char str[maxn + 10];

namespace FTree {
#define lowbit(x) ((x) & -(x))
int val[maxn + 10];
unsigned int n;

void init(const int n) { FTree::n = n; }
void update(unsigned int pos, const int v)
{
  pos = n + 1 - pos;
  for (; pos <= n; pos += lowbit(pos))
    val[pos] = std::max(val[pos], v);
}
int suffixMax(unsigned int pos)
{
  int ret = 0;
  pos = n + 1 - pos;
  for (; pos; pos -= lowbit(pos))
    ret = std::max(ret, val[pos]);
  return ret;
}
#undef lowbit
}  // namespace FTree

namespace SAM {
struct Node {
  int len;
  Node *link, *next[maxc];
} nodes[maxn * 3 + 10], *cur = nodes + 1, *root = nodes;

inline Node *insertChar(Node *const last, const int ch)
{
  Node *const ptr = cur++, *from = last;
  ptr->len = last->len + 1;
  while (from && !from->next[ch]) {
    from->next[ch] = ptr;
    from = from->link;
  }
  if (!from)
    ptr->link = root;
  else {
    Node *const fNxt = from->next[ch];
    if (fNxt->len == from->len + 1)
      ptr->link = fNxt;
    else {
      Node *const cloned = cur++;
      *cloned = *fNxt;
      cloned->len = from->len + 1;
      for (Node *i = from; i && i->next[ch] == fNxt; i = i->link)
        i->next[ch] = cloned;
      fNxt->link = ptr->link = cloned;
    }
  }
  return ptr;
}
}  // namespace SAM

namespace LCT {
struct Node {
  Node *child[2], *fa;
  int tag;
  bool dirty;

  inline void pushTag(const int x);
  inline void pushDown();
  inline void pushPath();
  inline bool isRoot() const;
  inline void rotate();
  void splay();
} nodes[maxn * 3 + 10];

inline void Node::pushTag(const int x)
{
  tag = x;
  dirty = true;
}
inline void Node::pushDown()
{
  if (!dirty) return;
  if (child[0]) child[0]->pushTag(tag);
  if (child[1]) child[1]->pushTag(tag);
  dirty = false;
}
inline void Node::pushPath()
{
  if (!isRoot()) fa->pushPath();
  pushDown();
}
inline bool Node::isRoot() const
{
  return !fa || (fa->child[0] != this && fa->child[1] != this);
}
inline void Node::rotate()
{
  Node *const f = fa, *const ff = fa->fa;
  if (!f->isRoot()) ff->child[ff->child[1] == f] = this;
  const bool typ = f->child[1] == this;
  fa = ff;
  f->fa = this;
  if (child[!typ]) child[!typ]->fa = f;
  f->child[typ] = child[!typ];
  child[!typ] = f;
}
void Node::splay()
{
  pushPath();
  while (!isRoot()) {
    if (!fa->isRoot()) {
      Node *const ff = fa->fa;
      if ((ff->child[1] == fa) ^ (fa->child[1] == this))
        rotate();
      else
        fa->rotate();
    }
    rotate();
  }
}

void access(Node *x)
{
  for (Node *last = nullptr; x; x = x->fa) {
    x->splay();
    x->child[1] = last;
    last = x;
  }
}
void accessWithTag(const int id, const int tag)
{
  Node *x = nodes + id, *last;
  for (last = nullptr; x; x = x->fa) {
    x->splay();
    if (x->tag) FTree::update(x->tag, SAM::nodes[x - nodes].len);
    x->child[1] = last;
    last = x;
  }
  last->pushTag(tag);
}
void link(const int from, const int to)
{
  Node *const pto = nodes + to;
  access(pto);
  pto->fa = nodes + from;
}
}  // namespace LCT

struct Query {
  int l, ans;
} qry[maxm + 10];
std::vector<Query *> seq[maxn + 10];
SAM::Node *prefix[maxn + 10];

void build(const char str[], const int len)
{
  prefix[0] = SAM::root;
  for (int i = 1; i <= len; ++i)
    prefix[i] = SAM::insertChar(prefix[i - 1], str[i] - '0');
  using SAM::nodes;
  for (SAM::Node *i = nodes + 1; i < SAM::cur; ++i)
    LCT::link(i->link - nodes, i - nodes);
  FTree::init(len);
}
void answerQuery(const int len)
{
  for (int i = 1; i <= len; ++i) {
    LCT::accessWithTag(prefix[i] - SAM::nodes, i);
    for (Query *j : seq[i])
      j->ans = FTree::suffixMax(j->l);
  }
}

int main()
{
  std::ios::sync_with_stdio(false);
  static char str[maxn + 10];
  int n, m;
  std::cin >> n >> m;
  std::cin >> (str + 1);
  for (Query *i = qry; i < qry + m; ++i) {
    int r;
    std::cin >> i->l >> r;
    seq[r].push_back(i);
  }
  build(str, n);
  answerQuery(n);
  for (const Query *i = qry; i < qry + m; ++i)
    std::cout << i->ans << "\n";
  return 0;
}
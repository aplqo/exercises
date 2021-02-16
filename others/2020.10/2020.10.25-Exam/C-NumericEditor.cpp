#ifdef APTEST
#include "debug_tools/program.h"
#endif
#include <algorithm>
#include <climits>
#include <iostream>
using namespace std;
const unsigned int maxn = 300000;
constexpr long long inf = LLONG_MAX / 2, ninf = LLONG_MIN / 2;

class Splay {
 public:
  void build();
  void insert(unsigned int pos, const long long val);
  void erase(const unsigned int pos);
  long long sum(const unsigned int l, const unsigned int r);
  void modify(const unsigned int pos, const long long val);
  unsigned int size() const;

  // test code
  void print()
  {
    clog << "root: " << &root << endl;
    root.chl[1]->print(nodes);
    clog << endl;
  }

 private:
  struct Node {
    void rotate();
    void splay(Node* const dest);
    void pushup();
    void pushChain();

    // test code
    void print(Node* base)
    {
      const auto prt = [](Node* n, Node* base) {
        if (n)
          clog << n - base;
        else
          clog << "(null)";
      };
      if (chl[0]) chl[0]->print(base);
      clog << this << ": " << chl[0] << " " << chl[1] << " " << fa << " "
           << " " << val << " " << sum << endl;
      if (chl[1]) chl[1]->print(base);
    }

    long long sum, val, siz;
    Node *chl[2], *fa;
  } nodes[maxn + 10], root, *cur;
  static const Node* maybe(const Node* p);
  Node* kth(unsigned int val);
  Node* extract(Node* const l, Node* const r);  // return father node
} spl;
void Splay::build()
{
  cur = nodes;
  new (&root) Node{ninf + inf, 0, 3, {nullptr, cur}, nullptr};
  new (cur) Node{ninf + inf, ninf, 2, {nullptr, cur + 1}, &root};
  ++cur;
  new (cur) Node{inf, inf, 1, {}, cur - 1};
  ++cur;
}
void Splay::insert(unsigned int pos, const long long val)
{
  Node* const ptr = extract(kth(pos + 1), kth(pos + 2));
  new (cur) Node{val, val, 1, {}, ptr};
  ptr->chl[0] = cur++;
  ptr->chl[0]->pushChain();
  ptr->chl[0]->splay(&root);
}
void Splay::erase(const unsigned int pos)
{
  Node* const ptr = extract(kth(pos), kth(pos + 2));
  ptr->chl[0] = nullptr;
  ptr->pushChain();
}
long long Splay::sum(const unsigned int l, const unsigned int r)
{
  Node* const v = extract(kth(l), kth(r + 2))->chl[0];
  const long long ret = v->sum;
  v->splay(&root);
  return ret;
}
void Splay::modify(const unsigned int pos, const long long val)
{
  Node* const p = kth(pos + 1);
  p->splay(&root);
  p->val = val;
  p->pushChain();
}
unsigned int Splay::size() const { return root.siz - 3; }
void Splay::Node::rotate()
{
  Node *const f = fa, *const ff = fa->fa;
  if (ff) ff->chl[ff->chl[1] == f] = this;
  const bool typ = f->chl[1] == this;
  fa = ff;
  f->fa = this;
  if (chl[!typ]) chl[!typ]->fa = f;
  f->chl[typ] = chl[!typ];
  chl[!typ] = f;
  f->pushup();
  this->pushup();
}
void Splay::Node::splay(Splay::Node* const dest)
{
  while (fa != dest) {
    Node* const ff = fa->fa;
    if (ff != dest) {
      if ((ff->chl[1] == fa) ^ (fa->chl[1] == this))
        rotate();
      else
        fa->rotate();
    }
    this->rotate();
  }
}
void Splay::Node::pushup()
{
  sum = maybe(chl[0])->sum + maybe(chl[1])->sum + this->val;
  siz = maybe(chl[0])->siz + maybe(chl[1])->siz + 1;
}
void Splay::Node::pushChain()
{
  pushup();
  if (fa) fa->pushChain();
}
const Splay::Node* Splay::maybe(const Splay::Node* p)
{
  static const Node nil{};
  return p ? p : &nil;
}
Splay::Node* Splay::kth(unsigned int k)
{
  Node* cur = root.chl[1];
  while (k) {
    if (maybe(cur->chl[0])->siz >= k)
      cur = cur->chl[0];
    else if (maybe(cur->chl[0])->siz + 1 == k)
      break;
    else {
      k -= maybe(cur->chl[0])->siz + 1;
      cur = cur->chl[1];
    }
  }
  cur->splay(&root);
  return cur;
}
Splay::Node* Splay::extract(Splay::Node* const l, Splay::Node* const r)
{
  l->splay(&root);
  r->splay(l);
  return r;
}

int main()
{
#ifndef APTEST
  ios_base::sync_with_stdio(false);
#endif
  unsigned int n;
  cin >> n;
  spl.build();
  unsigned int pos = 0;
  for (unsigned int i = 0; i < n; ++i) {
    char op;
    long long x;
    unsigned int l, r;
    cin >> op;
    switch (op) {
      case 'I':
        cin >> x;
        spl.insert(pos, x);
        ++pos;
        break;
      case 'D':
        spl.erase(pos);
        --pos;
        break;
      case 'L':
        if (pos >= 1) --pos;
        break;
      case 'R':
        if (pos < spl.size()) ++pos;
        break;
      case 'Q':
        cin >> l >> r;
        cout << spl.sum(l, r) << "\n";
        break;
      case 'C':
        cin >> l >> x;
        spl.modify(l, x);
        break;
    }
  }
  return 0;
}

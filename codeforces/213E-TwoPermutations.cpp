#ifdef APTEST
#include "debug_tools/program.h"
#endif
#include <algorithm>
#include <climits>
#include <iostream>
#include <iterator>
#include <numeric>
using namespace std;
const unsigned int maxn = 2e5;
constexpr unsigned long long base = 200003;  // min prime greater than 2e5

unsigned long long po[maxn + 1] = {1};
class Splay {
 public:
  void insert(const unsigned int k, const unsigned int x);
  void remove(const unsigned int x);
  unsigned long long hash() const;

 private:
  struct node {
    node() = default;
    node(const unsigned int k, const unsigned int v, node* f)
        : key(k), val(v), siz(1), h(v), fa(f){};
    void rotate();
    void splay(node* const dest);
    void pushup();

    unsigned int key, val, siz = 0;
    unsigned long long h;
    node *fa = nullptr, *chl[2] = {};
  } SNodes[maxn + 1], root, *cur = SNodes;
  node* pre(const unsigned int x);
  node* nxt(const unsigned int x);
  node* extract(const unsigned int x);
} spl;
void Splay::node::rotate()
{
  Splay::node *f = fa, *ff = fa->fa;
  if (ff) ff->chl[ff->chl[1] == fa] = this;
  const bool typ = fa->chl[1] == this;
  fa = ff;
  f->fa = this;
  if (chl[!typ]) chl[!typ]->fa = f;
  f->chl[typ] = chl[!typ];
  chl[!typ] = f;
  f->pushup();
  this->pushup();
}
void Splay::node::splay(Splay::node* const dest)
{
  while (fa != dest) {
    Splay::node* const ff = fa->fa;
    if (ff != dest) {
      if ((ff->chl[1] == fa) ^ (fa->chl[1] == this))
        rotate();
      else
        fa->rotate();
    }
    rotate();
  }
}
void Splay::node::pushup()
{
  h = 0;
  const unsigned int ls = chl[0] ? chl[0]->siz : 0,
                     rs = chl[1] ? chl[1]->siz : 0;
  if (chl[0]) h = chl[0]->h * po[rs + 1];
  h += val * po[rs];
  if (chl[1]) h += chl[1]->h;
  siz = 1 + ls + rs;
}
void Splay::insert(const unsigned int k, const unsigned int x)
{
  if (!root.chl[1]) {
    new (cur) node(k, x, &root);
    root.chl[1] = cur++;
    return;
  }
  Splay::node* c = root.chl[1];
  while (c) {
    const bool typ = c->key < k;
    if (!c->chl[typ]) {
      new (cur) node(k, x, c);
      c->chl[typ] = cur++;
      c = c->chl[typ];
      break;
    }
    c = c->chl[typ];
  }
  auto t = c;
  while (t) {
    t->pushup();
    t = t->fa;
  }
  c->splay(&root);
}
void Splay::remove(const unsigned int x)
{
  Splay::node *ptr = extract(x), *f = ptr->fa;
  f->chl[f->chl[1] == ptr] = nullptr;
  while (f) {
    f->pushup();
    f = f->fa;
  }
}
unsigned long long Splay::hash() const { return root.chl[1]->h; }
Splay::node* Splay::pre(const unsigned int x)
{
  Splay::node *c = root.chl[1], *ret = nullptr;
  unsigned int v = 0;
  while (c) {
    if (c->key < x && c->key >= v) {
      v = c->key;
      ret = c;
    }
    c = c->chl[c->key < x];
  }
  return ret;
}
Splay::node* Splay::nxt(const unsigned int x)
{
  Splay::node *c = root.chl[1], *ret = nullptr;
  unsigned int v = UINT_MAX;
  while (c) {
    if (c->key > x && c->key < v) {
      v = c->key;
      ret = c;
    }
    c = c->chl[c->key <= x];
  }
  return ret;
}
Splay::node* Splay::extract(const unsigned int x)
{
  Splay::node *p = pre(x), *n = nxt(x);
  if (p && n) {
    p->splay(&root);
    n->splay(p);
    return n->chl[0];
  }
  else if (!p && n) {
    n->splay(&root);
    return n->chl[0];
  }
  else if (p && !n) {
    p->splay(&root);
    return p->chl[1];
  }
  return root.chl[1];
}

unsigned int a[maxn + 1], mp[maxn + 1];
static void getLog(const unsigned int n)
{
  for (unsigned int i = 1; i <= n; ++i)
    po[i] = po[i - 1] * base;
}
unsigned long long getHash(const unsigned int a[], const unsigned int l)
{
  unsigned long long ret = 0;
  for (unsigned int i = 0; i < l; ++i)
    ret = ret * base + a[i];
  return ret;
}
unsigned int count(const unsigned int n, const unsigned int m)
{
  unsigned int ret = 0;
  for (unsigned int i = 1; i <= n; ++i)
    spl.insert(mp[i], i);
  const unsigned long long stp = accumulate(po, po + n, 0ull);
  unsigned long long nHash = getHash(a, n);
  if (nHash == spl.hash()) ++ret;
  for (unsigned int x = 1; x <= m - n; ++x) {
    spl.remove(mp[x]);
    spl.insert(mp[n + x], n + x);
    nHash += stp;
    ret += spl.hash() == nHash;
  }
  return ret;
}
int main()
{
#ifndef APTEST
  ios_base::sync_with_stdio(false);
#endif
  unsigned int n, m;
  cin >> n >> m;
  copy_n(istream_iterator<unsigned int>(cin), n, a);
  for (unsigned int i = 0; i < m; ++i) {
    unsigned int v;
    cin >> v;
    mp[v] = i;
  }
  getLog(m);
  cout << count(n, m) << endl;
  return 0;
}

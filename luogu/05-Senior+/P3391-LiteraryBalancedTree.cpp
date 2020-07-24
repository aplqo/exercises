#ifdef APTEST
#include "debug_tools/program.h"
#endif
#include <algorithm>
#include <iostream>
#include <iterator>
#include <numeric>
using namespace std;
const unsigned int maxn = 100000;

class Splay
{
public:
    void print(unsigned int*(&buf));
    static void reverse(const unsigned int l, const unsigned int r);
    static void build(const unsigned int n, const unsigned int val[]);

private:
    Splay* find(const unsigned int k);
    void pushdown();
    void pushup();
    void rotate();
    void splay(Splay* const dest);

    bool rev = false;
    unsigned int val, siz = 0;
    Splay *fa, *child[2];

} nodes[maxn + 10], root;
void Splay::build(const unsigned int n, const unsigned int val[])
{
    root.child[1] = nodes;
    root.child[0] = nullptr;
    root.fa = nullptr;
    root.siz = n + 3;
    for (Splay* i = nodes; i < nodes + 1 + n; ++i)
    {
        i->fa = i - 1;
        i->val = val[i - nodes];
        i->siz = n + 2 - (i - nodes);
        i->child[0] = nullptr;
        i->child[1] = i + 1;
    }
    nodes[0].fa = &root;
    nodes[n + 1].siz = 1;
    nodes[n + 1].fa = nodes + n;
    nodes[n + 1].child[0] = nodes[n + 1].child[1] = nullptr;
}
void Splay::reverse(const unsigned int l, const unsigned int r)
{
    Splay *const ln = root.find(2 + l - 1), *const rn = root.find(2 + r + 1);
    ln->splay(&root);
    rn->splay(ln);
    rn->child[0]->rev ^= true;
}
void Splay::rotate()
{
    fa->pushdown();
    pushdown();
    Splay *const ff = fa->fa, *f = fa;
    if (ff)
        ff->child[ff->child[1] == f] = this;
    const bool typ = fa->child[1] == this;
    fa = ff;
    f->fa = this;
    if (child[!typ])
        child[!typ]->fa = f;

    f->child[typ] = child[!typ];
    child[!typ] = f;
    f->pushup();
    pushup();
}
void Splay::splay(Splay* const dest)
{
    while (fa != dest)
    {
        Splay* const ff = fa->fa;
        if (ff != dest)
        {
            if ((ff->child[1] == fa) ^ (fa->child[1] == this))
                rotate();
            else
                fa->rotate();
        }
        rotate();
    }
}
void Splay::pushdown()
{
    if (!rev)
        return;
    swap(child[0], child[1]);
    if (child[0])
        child[0]->rev ^= true;
    if (child[1])
        child[1]->rev ^= true;
    rev = false;
}
void Splay::pushup()
{
    siz = 1;
    if (child[0])
        siz += child[0]->siz;
    if (child[1])
        siz += child[1]->siz;
}
void Splay::print(unsigned int*(&buf))
{
    pushdown();
    if (child[0])
        child[0]->print(buf);
    *(buf++) = val;
    if (child[1])
        child[1]->print(buf);
}
Splay* Splay::find(const unsigned int k)
{
    pushdown();
    const unsigned int lsiz = (child[0] ? child[0]->siz : 0);
    if (lsiz >= k)
        return child[0]->find(k);
    else if (k == lsiz + 1)
        return this;
    return child[1]->find(k - lsiz - 1);
}

unsigned int val[maxn + 10];

int main()
{
#ifndef APTEST
    ios_base::sync_with_stdio(false);
#endif
    unsigned int n, m;
    cin >> n >> m;
    iota(val + 1, val + 1 + n, 1);
    Splay::build(n, val);
    for (unsigned int i = 0; i < m; ++i)
    {
        unsigned int l, r;
        cin >> l >> r;
        Splay::reverse(l, r);
    }
    {
        unsigned int* c = val;
        root.print(c);
    }
    copy_n(val + 2, n, ostream_iterator<unsigned int>(cout, " "));
    cout << endl;
    return 0;
}
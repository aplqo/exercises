#ifdef APTEST
#include "debug_tools/program.h"
#endif
#include <algorithm>
#include <iostream>
#include <iterator>
using namespace std;
const unsigned int maxn = 2e5, maxm = 1e5;

class LinkCutTree
{
public:
    void link(const unsigned int x, const unsigned int y);
    void cut(const unsigned int x, const unsigned int y);
    unsigned int depth(const unsigned int x, const unsigned int rt);

private:
    struct SplayNode
    {
        inline void pushRev();
        inline void pushup();
        inline void pushdown();
        inline bool isRoot();
        void pushPath();
        void rotate();
        void splay();

        unsigned int siz = 1;
        bool rev = false;
        SplayNode *fa = nullptr, *chl[2] = {};
    } snodes[maxn + 10];

    void access(SplayNode* x);
    void makeRoot(SplayNode* x);
    void split(SplayNode* x, SplayNode* rt);
} lct;
unsigned int k[maxn + 10];

void LinkCutTree::link(const unsigned int x, const unsigned int y)
{
    SplayNode* const px = snodes + x;
    makeRoot(px);
    px->fa = snodes + y;
}
void LinkCutTree::cut(const unsigned int x, const unsigned int y)
{
    SplayNode *const px = snodes + x, *const py = snodes + y;
    split(px, py);
    px->fa = py->chl[0] = nullptr;
    py->pushup();
}
unsigned int LinkCutTree::depth(const unsigned int x, const unsigned int y)
{
    SplayNode *const px = snodes + x, *const py = snodes + y;
    split(px, py);
    return py->siz;
}
void LinkCutTree::access(SplayNode* x)
{
    for (SplayNode* lst = nullptr; x; x = x->fa)
    {
        x->splay();
        x->chl[1] = lst;
        x->pushup();
        lst = x;
    }
}
void LinkCutTree::makeRoot(SplayNode* x)
{
    access(x);
    x->splay();
    x->pushRev();
}
void LinkCutTree::split(SplayNode* x, SplayNode* rt)
{
    makeRoot(x);
    access(rt);
    rt->splay();
}
inline void LinkCutTree::SplayNode::pushRev()
{
    swap(chl[0], chl[1]);
    rev ^= 1;
}
inline void LinkCutTree::SplayNode::pushup()
{
    siz = 1;
    for (const SplayNode* i : chl)
        siz += i ? i->siz : 0;
}
inline void LinkCutTree::SplayNode::pushdown()
{
    if (!rev)
        return;
    for (SplayNode* i : chl)
        if (i)
            i->pushRev();
    rev = false;
}
inline bool LinkCutTree::SplayNode::isRoot()
{
    return fa ? fa->chl[0] != this && fa->chl[1] != this : true;
}
void LinkCutTree::SplayNode::pushPath()
{
    if (!isRoot())
        fa->pushPath();
    pushdown();
}
void LinkCutTree::SplayNode::rotate()
{
    SplayNode *const ff = fa->fa, *const f = fa;
    if (!f->isRoot())
        ff->chl[ff->chl[1] == f] = this;
    const bool typ = f->chl[1] == this;
    f->fa = this;
    fa = ff;
    if (chl[!typ])
        chl[!typ]->fa = f;
    f->chl[typ] = chl[!typ];
    chl[!typ] = f;
    f->pushup();
    this->pushup();
}
void LinkCutTree::SplayNode::splay()
{
    pushPath();
    while (!isRoot())
    {
        SplayNode* const ff = fa->fa;
        if (!fa->isRoot())
        {
            if ((ff->chl[1] == fa) ^ (fa->chl[1] == this))
                this->rotate();
            else
                fa->rotate();
        }
        rotate();
    }
}

static void init(const unsigned int n)
{
    for (unsigned int i = 0; i < n; ++i)
    {
        k[i] = min(i + k[i], n);
        lct.link(i, k[i]);
    }
}
void modify(const unsigned int x, const unsigned int n, const unsigned int nk)
{
    lct.cut(x, k[x]);
    k[x] = min(n, x + nk);
    lct.link(x, k[x]);
}

int main()
{
#ifndef APTEST
    ios_base::sync_with_stdio(false);
#endif
    unsigned int n, m;
    cin >> n;
    copy_n(istream_iterator<unsigned int>(cin), n, k);
    init(n);
    cin >> m;
    for (unsigned int i = 0; i < m; ++i)
    {
        unsigned int op, x;
        cin >> op >> x;
        if (op == 1)
            cout << lct.depth(x, n) - 1 << "\n";
        else
        {
            unsigned int nk;
            cin >> nk;
            modify(x, n, nk);
        }
    }
    return 0;
}
#ifdef APTEST
#include "debug_tools/program.h"
#endif
#include <iomanip>
#include <iostream>
using namespace std;
const unsigned int maxn = 1e5;

class LinkCutTree
{
public:
    unsigned long long pathCount(const unsigned int x, const unsigned int y);
    void link(const unsigned int x, const unsigned int y);

private:
    struct SplayNode
    {
        inline void pushup();
        inline void pushRev();
        inline void pushdown();
        inline bool isRoot();
        void pushPath();
        void rotate();
        void splay();

        unsigned int realSiz = 1, virtSiz = 0;
        bool rev = false;
        SplayNode *fa = nullptr, *chl[2] = {};
    } snodes[maxn + 10];

    void access(SplayNode* x);
    void split(SplayNode* x, SplayNode* rt);
    void makeRoot(SplayNode* x);
    static inline SplayNode* maybe(SplayNode* x)
    {
        static SplayNode nil { 0, 0, false, nullptr, { nullptr, nullptr } };
        return x ? x : &nil;
    }
} lct;
unsigned long long LinkCutTree::pathCount(const unsigned int x, const unsigned int y)
{
    SplayNode *const px = snodes + x, *const py = snodes + y;
    split(px, py);
    return static_cast<unsigned long long>(px->virtSiz + 1) * (py->virtSiz + 1);
}
void LinkCutTree::link(const unsigned int x, const unsigned int y)
{
    SplayNode *const px = snodes + x, *const py = snodes + y;
    makeRoot(px);
    makeRoot(py);
    px->fa = py;
    py->virtSiz += px->realSiz;
    py->pushup();
}
void LinkCutTree::access(LinkCutTree::SplayNode* x)
{
    for (SplayNode* lst = nullptr; x; x = x->fa)
    {
        x->splay();
        x->virtSiz = x->virtSiz + maybe(x->chl[1])->realSiz - maybe(lst)->realSiz;
        x->chl[1] = lst;
        x->pushup();
        lst = x;
    }
}
void LinkCutTree::split(LinkCutTree::SplayNode* x, LinkCutTree::SplayNode* rt)
{
    makeRoot(x);
    access(rt);
    rt->splay();
}
void LinkCutTree::makeRoot(LinkCutTree::SplayNode* x)
{
    access(x);
    x->splay();
    x->pushRev();
}
inline void LinkCutTree::SplayNode::pushup()
{
    realSiz = virtSiz + maybe(chl[0])->realSiz + maybe(chl[1])->realSiz + 1;
}
inline void LinkCutTree::SplayNode::pushRev()
{
    swap(chl[0], chl[1]);
    rev ^= 1;
}
inline void LinkCutTree::SplayNode::pushdown()
{
    if (!rev)
        return;
    maybe(chl[0])->pushRev();
    maybe(chl[1])->pushRev();
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
    SplayNode *const f = fa, *const ff = fa->fa;
    if (!f->isRoot())
        ff->chl[ff->chl[1] == f] = this;
    const bool typ = f->chl[1] == this;
    fa = ff;
    f->fa = this;
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

int main()
{
#ifndef APTEST
    ios_base::sync_with_stdio(false);
#endif
    unsigned int n, q;
    cin >> n >> q;
    for (unsigned int i = 0; i < q; ++i)
    {
        char op;
        unsigned int x, y;
        cin >> op >> x >> y;
        if (op == 'A')
            lct.link(x, y);
        else
            cout << lct.pathCount(x, y) << "\n";
    }
    return 0;
}
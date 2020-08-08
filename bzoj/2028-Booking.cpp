#ifdef APTEST
#include "debug_tools/program.h"
#endif
#include <climits>
#include <iostream>
using namespace std;
const unsigned int maxn = 200000;

class Splay
{
public:
    void build();
    void insert(const unsigned int l, const unsigned int r);
    void remove(const unsigned int l, const unsigned int r);
    unsigned int size() const;

private:
    struct node
    {
        node() = default;
        node(unsigned int l, unsigned int r, node* f)
            : siz(1)
            , lval(l)
            , rval(r)
            , fa(f) {};
        void pushup();
        void rotate();
        void splay(node* const dest);

        unsigned int siz = 0;
        unsigned int lval, rval;
        node *chl[2] = {}, *fa = nullptr;
    } SNodes[maxn + 1], *cur = SNodes, root, *mnv, *mxv;
    node* pre(const unsigned int r);
    node* nxt(const unsigned int l);
    node* extract(const unsigned int lv, const unsigned int rv);
} spl;
void Splay::build()
{
    new (cur) node(UINT_MAX, UINT_MAX, &root);
    mxv = cur;
    root.chl[1] = cur++;

    new (cur) node(0, 0, cur - 1);
    ++((cur - 1)->siz);
    mnv = cur;
    root.chl[1]->chl[0] = cur++;

    root.siz = 3;
}
void Splay::insert(const unsigned int l, const unsigned int r)
{
    Splay::node* c = root.chl[1];
    while (true)
    {
        ++(c->siz);
        const bool typ = c->lval < l;
        if (!c->chl[typ])
        {
            new (cur) node(l, r, c);
            c->chl[typ] = cur;
            c = cur++;
            break;
        }
        c = c->chl[typ];
    }
    ++root.siz;
    c->splay(&root);
}
void Splay::remove(const unsigned int l, const unsigned int r)
{
    Splay::node* c = extract(l, r);
    if (!c)
        return;
    Splay::node* f = c->fa;
    f->chl[f->chl[1] == c] = nullptr;
    while (f)
    {
        f->pushup();
        f = f->fa;
    }
}
unsigned int Splay::size() const { return root.siz - 3; }
void Splay::node::pushup()
{
    siz = 1;
    if (chl[0])
        siz += chl[0]->siz;
    if (chl[1])
        siz += chl[1]->siz;
}
void Splay::node::rotate()
{
    Splay::node *f = fa, *ff = fa->fa;
    if (ff)
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
void Splay::node::splay(Splay::node* const dest)
{
    while (fa != dest)
    {
        Splay::node* ff = fa->fa;
        if (ff != dest)
        {
            if ((ff->chl[1] == fa) ^ (fa->chl[1] == this))
                rotate();
            else
                fa->rotate();
        }
        rotate();
    }
}
Splay::node* Splay::pre(const unsigned int r)
{
    Splay::node *ret = mnv, *c = root.chl[1];
    unsigned int pv = 0;
    while (c)
    {
        if (c->rval < r && c->rval > pv)
        {
            pv = c->rval;
            ret = c;
        }
        c = c->chl[c->rval < r];
    }
    ret->splay(&root);
    return ret;
}
Splay::node* Splay::nxt(const unsigned int l)
{
    Splay::node *ret = mxv, *c = root.chl[1];
    unsigned int pv = UINT_MAX;
    while (c)
    {
        if (c->lval > l && c->lval < pv)
        {
            pv = c->lval;
            ret = c;
        }
        c = c->chl[c->lval <= l];
    }
    ret->splay(&root);
    return ret;
}
Splay::node* Splay::extract(const unsigned int lv, const unsigned int rv)
{
    Splay::node *n = nxt(rv), *p = pre(lv);
    p->splay(&root);
    n->splay(p);
    return n->chl[0];
}

int main()
{
#ifndef APTEST
    ios_base::sync_with_stdio(false);
#endif
    unsigned int n;
    cin >> n;
    spl.build();
    for (unsigned int i = 0; i < n; ++i)
    {
        char op;
        cin >> op;
        if (op == 'A')
        {
            const unsigned int os = spl.size();
            unsigned int l, r;
            cin >> l >> r;
            spl.remove(l, r);
            spl.insert(l, r);
            cout << os + 1 - spl.size() << endl;
        }
        else
            cout << spl.size() << endl;
    }
    return 0;
}

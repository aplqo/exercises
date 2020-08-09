#ifdef APTEST
#include "debug_tools/program.h"
#endif
#include <algorithm>
#include <climits>
#include <cmath>
#include <iostream>
using namespace std;
const unsigned int maxn = 1e5;

class Splay
{
public:
    void build(const unsigned int n, const char str[]);
    inline void assign(const unsigned int l, const unsigned int r, const char typ)
    {
        node* t = extract(l, r);
        t->pushAss(typ == '(' ? 1 : -1);
        update(t);
    }
    inline void inverse(const unsigned int l, const unsigned int r)
    {
        node* t = extract(l, r);
        t->pushInv();
        update(t);
    }
    inline void swap(const unsigned int l, const unsigned int r)
    {
        node* t = extract(l, r);
        t->pushSwp();
        update(t);
    }
    inline unsigned int query(const unsigned int l, const unsigned int r)
    {
        return extract(l, r)->info.answer();
    }

private:
    enum
    {
        Min = 0,
        Max = 1
    };
    struct Value
    {
        int pref[2] = { INT_MAX / 2, 0 }, suff[2] = { INT_MAX / 2, 0 }, sum = 0;
        inline unsigned int answer() { return (-pref[Min] + 1) / 2 + (suff[Max] + 1) / 2; }
    };
    struct node
    {
        void pushdown();
        void pushup();
        void pushAss(const int v);
        void pushSwp();
        void pushInv();
        void rotate();
        void splay(node* const dest);

        /*tags*/
        int tagAss = 0;
        bool tagInv = false, tagSwp = false;
        /*position and value*/
        int val;
        unsigned int siz = 0;
        Value info;
        node *chl[2] = {}, *fa = nullptr;
    } SNodes[maxn + 3], root, *cur = SNodes;
    node* kth(const unsigned int k);
    void update(node* c);
    inline node* extract(const unsigned int l, const unsigned int r);
} spl;
char str[maxn + 10];
void Splay::build(const unsigned int n, const char str[])
{
    root.chl[1] = cur;
    cur->fa = &root;
    cur->val = 0;
    ++cur;
    for (unsigned int i = 1; i <= n; ++i)
    {
        cur->val = str[i] == '(' ? 1 : -1;
        cur->fa = cur - 1;
        (cur - 1)->chl[1] = cur;
        ++cur;
    }
    cur->val = 0;
    cur->fa = cur - 1;
    (cur - 1)->chl[1] = cur;
    ++cur;
    for (Splay::node* c = cur - 1; c; c = c->fa)
        c->pushup();
    for (Splay::node* c = cur - 1; c != &root; c = c->fa)
        c->splay(&root);
}
void Splay::node::pushdown()
{
    if (!tagAss && !tagInv && !tagSwp)
        return;
    for (Splay::node* i : chl)
    {
        if (!i)
            continue;
        if (tagAss)
            i->pushAss(tagAss);
        if (tagInv)
            i->pushInv();
        if (tagSwp)
            i->pushSwp();
    }
    tagInv = tagSwp = false;
    tagAss = 0;
}
void Splay::node::pushup()
{
    static const Value nil;
    const Value &lv = chl[0] ? chl[0]->info : nil, &rv = chl[1] ? chl[1]->info : nil;
    info.sum = val;
    siz = 1;
    info.pref[Min] = min({ rv.pref[Min] + val, val, 0 });
    info.pref[Max] = max({ rv.pref[Max] + val, val, 0 });
    info.suff[Min] = min({ lv.suff[Min] + val, val, 0 });
    info.suff[Max] = max({ lv.suff[Max] + val, val, 0 });
    if (chl[0])
    {
        info.pref[Min] = min({ lv.pref[Min], lv.sum + info.pref[Min], 0 });
        info.pref[Max] = max({ lv.pref[Max], lv.sum + info.pref[Max], 0 });
        siz += chl[0]->siz;
        info.sum += chl[0]->info.sum;
    }
    if (chl[1])
    {
        info.suff[Min] = min({ rv.suff[Min], rv.sum + info.suff[Min], 0 });
        info.suff[Max] = max({ rv.suff[Max], rv.sum + info.suff[Max], 0 });
        siz += chl[1]->siz;
        info.sum += chl[1]->info.sum;
    }
}
void Splay::node::pushAss(const int v)
{
    tagAss = v;
    tagInv = tagSwp = false;
    val = v;
    const unsigned int mv = v > 0 ? Max : Min, lv = v > 0 ? Min : Max;
    info.sum = v * static_cast<int>(siz);
    info.pref[mv] = info.suff[mv] = info.sum;
    info.pref[lv] = info.suff[lv] = 0;
}
void Splay::node::pushInv()
{
    tagInv ^= true;
    val = -val;
    std::swap(info.pref[Min], info.pref[Max]);
    std::swap(info.suff[Min], info.suff[Max]);
    info.pref[Min] *= -1;
    info.pref[Max] *= -1;
    info.suff[Min] *= -1;
    info.suff[Max] *= -1;
    info.sum *= -1;
}
void Splay::node::pushSwp()
{
    tagSwp ^= true;
    std::swap(info.pref, info.suff);
    std::swap(chl[0], chl[1]);
}
void Splay::node::rotate()
{
    Splay::node *ff = fa->fa, *f = fa;
    f->pushdown();
    this->pushdown();
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
Splay::node* Splay::kth(unsigned int k)
{
    Splay::node* ret = root.chl[1];
    while (true)
    {
        ret->pushdown();
        const unsigned int ls = ret->chl[0] ? ret->chl[0]->siz : 0;
        if (k == ls + 1)
        {
            ret->splay(&root);
            return ret;
        }
        if (k > ls + 1)
        {
            k -= ls + 1;
            ret = ret->chl[1];
        }
        else
            ret = ret->chl[0];
    }
    return nullptr;
}
inline Splay::node* Splay::extract(const unsigned int l, const unsigned int r)
{
    Splay::node *p = kth(l), *n = kth(r + 1 + 1);
    p->splay(&root);
    n->splay(p);
    return n->chl[0];
}
void Splay::update(Splay::node* c)
{
    c = c->fa;
    while (c)
    {
        c->pushup();
        c = c->fa;
    }
}

int main()
{
#ifndef APTEST
    ios_base::sync_with_stdio(false);
#endif
    unsigned int n, q;
    cin >> n >> q;
    cin >> (str + 1);
    spl.build(n, str);
    for (unsigned int i = 0; i < q; ++i)
    {
        char str[10], v;
        unsigned int l, r;
        cin >> str >> l >> r;
        switch (str[0])
        {
        case 'R':
            cin >> v;
            spl.assign(l, r, v);
            break;
        case 'S':
            spl.swap(l, r);
            break;
        case 'I':
            spl.inverse(l, r);
            break;
        case 'Q':
            cout << spl.query(l, r) << endl;
            break;
        }
    }
    return 0;
}

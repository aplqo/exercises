#ifdef APTEST
#include "debug_tools/program.h"
#endif
#include <algorithm>
#include <climits>
#include <iostream>
#include <numeric>
using namespace std;
const unsigned int maxn = 1e5;

struct Node
{
    inline void pushup();
    int val;
    unsigned int dist = 1, id;
    Node *lc = nullptr, *rc = nullptr;
} nodes[maxn + 1];
bool notExist[maxn + 1];

void Node::pushup()
{
    if (rc && (!lc || lc->dist < rc->dist))
        swap(lc, rc);
    dist = (rc ? rc->dist : 0) + 1;
}
Node* merge(Node* x, Node* y)
{
    if (!x || !y)
        return x ? x : y;
    if (x->val > y->val || x->val == y->val && x->id > y->id)
        swap(x, y);
    x->rc = merge(x->rc, y);
    x->pushup();
    return x;
}
Node* remove(Node* x)
{
    Node* c = merge(x->lc, x->rc);
    if (c)
        c->pushup();
    return c;
}
struct Dsu
{
    unsigned int fa[maxn + 1], siz[maxn + 1];
    Node* rt[maxn + 1];
    void init(const unsigned int n)
    {
        fill(siz, siz + 1 + n, 1);
        iota(fa, fa + 1 + n, 0);
        for (unsigned int i = 1; i <= n; ++i)
            rt[i] = nodes + i;
    }
    unsigned int getFather(const unsigned int x)
    {
        return fa[x] == x ? x : fa[x] = getFather(fa[x]);
    }
    unsigned int merge(const unsigned int x, const unsigned int y)
    {
        unsigned int fx = getFather(x), fy = getFather(y);
        if (siz[fx] < siz[fy])
            swap(fx, fy);
        fa[fy] = fx;
        siz[fx] += siz[fy];
        return fx;
    }
} d;
inline void merge(const unsigned int x, const unsigned int y)
{
    Node *xc = d.rt[d.getFather(x)], *yc = d.rt[d.getFather(y)];
    if (xc == yc || notExist[x] || notExist[y])
        return;
    d.rt[d.merge(x, y)] = merge(xc, yc);
}
int top(const unsigned int x)
{
    if (notExist[x])
        return -1;
    const unsigned int t = d.getFather(x);
    notExist[d.rt[t]->id] = true;
    const int ret = d.rt[t]->val;
    d.rt[t] = remove(d.rt[t]);
    return ret;
}

int main()
{
#ifndef APTEST
    ios_base::sync_with_stdio(false);
#endif
    unsigned int n, m;
    cin >> n >> m;
    for (unsigned int i = 1; i <= n; ++i)
    {
        cin >> nodes[i].val;
        nodes[i].id = i;
    }
    d.init(n);
    for (unsigned int i = 0; i < m; ++i)
    {
        unsigned int op, x, y;
        cin >> op;
        if (op == 1)
        {
            cin >> x >> y;
            merge(x, y);
        }
        else
        {
            cin >> x;
            cout << top(x) << endl;
        }
    }
    return 0;
}
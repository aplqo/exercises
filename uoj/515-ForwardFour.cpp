#ifdef APTEST
#include "debug_tools/program.h"
#endif
#include <algorithm>
#include <cctype>
#include <climits>
#include <cstdio>
#include <iostream>
#include <iterator>
#include <list>
using namespace std;
const unsigned int maxn = 1e6, maxq = 1e6;
constexpr unsigned int inf = UINT_MAX;

char inbuf[maxn * 20 + maxq * 20 * 3 + 1000], *inpos = inbuf, outbuf[maxq * 20 + 1000], *outpos = outbuf;

unsigned int readUInt()
{
    unsigned int ret = 0;
    char c = *(inpos++);
    while (!isdigit(c))
        c = *(inpos++);
    while (isdigit(c))
    {
        ret = ret * 10 + (c - '0');
        c = *(inpos++);
    }
    return ret;
}
void writeUInt(const unsigned int v)
{
    outpos += sprintf(outpos, "%u\n", v);
}

class SegmentTree
{
public:
    void build(const unsigned int l, const unsigned int r, SegmentTree*(&cur));
    void update(const unsigned int l, const unsigned int r, const unsigned int val);
    unsigned int count(const unsigned int pos);

private:
    template <bool chk>
    inline void pushFirst(const unsigned int v, const unsigned int cnt);
    inline void pushdown();
    inline void pushup();

    SegmentTree *lc, *rc;
    unsigned int lv, mv, rv;
    unsigned int fst, snd;
    unsigned int modify;
} nodes[maxq * 4 + 10], *root = nodes;
struct Modify
{
    unsigned int time, val;
};
struct Query
{
    unsigned int time, *ret;
};
unsigned int ans[maxq + 10], *qcur = ans;
list<Modify> changes[maxn + 10];
list<Query> querys[maxn + 10];

void SegmentTree::build(const unsigned int l, const unsigned int r, SegmentTree*(&cur))
{
    lv = l;
    rv = r;
    mv = (l + r) >> 1;
    fst = inf;
    modify = snd = 0;
    if (r - l == 1)
        return;
    lc = ++cur;
    lc->build(l, mv, cur);
    rc = ++cur;
    rc->build(mv, r, cur);
    pushup();
}
void SegmentTree::update(const unsigned int l, const unsigned int r, const unsigned int val)
{
    if (fst <= val)
        return;
    if (l == lv && r == rv && snd < val)
    {
        pushFirst<false>(val, 1);
        return;
    }
    pushdown();
    if (r <= mv)
        lc->update(l, r, val);
    else if (l >= mv)
        rc->update(l, r, val);
    else
    {
        lc->update(l, mv, val);
        rc->update(mv, r, val);
    }
    pushup();
}
unsigned int SegmentTree::count(const unsigned int pos)
{
    if (rv - lv == 1)
        return modify;
    pushdown();
    return (pos < mv ? lc : rc)->count(pos);
}
template <bool chk>
inline void SegmentTree::pushFirst(const unsigned int v, const unsigned int cnt)
{
    if (chk)
    {
        if (fst <= v)
            return;
    }
    modify += cnt;
    fst = v;
}
inline void SegmentTree::pushdown()
{
    if (!modify)
        return;
    lc->pushFirst<true>(fst, modify);
    rc->pushFirst<true>(fst, modify);
    modify = 0;
}
inline void SegmentTree::pushup()
{
    SegmentTree *mn = lc, *mx = rc;
    if (mn->fst > mx->fst)
        swap(mn, mx);
    fst = mx->fst;
    snd = std::max(mn->fst != mx->fst ? mn->fst : mn->snd, mx->snd);
}

void answerQuery(const unsigned int n, const unsigned int q)
{
    {
        SegmentTree* c = nodes;
        root->build(0, q + 1, c);
    }
    for (unsigned int i = n; i; --i)
    {

        // root->dump();
        {
            auto nxt = next(changes[i].cbegin());
            for (auto it = changes[i].cbegin(); nxt != changes[i].cend(); ++it, ++nxt)
                root->update(it->time, nxt->time, it->val);
            root->update(changes[i].back().time, q + 1, changes[i].back().val);
        }

        for (const auto q : querys[i])
            *q.ret = root->count(q.time);
    }
}

int main()
{
#ifndef APTEST
    ios_base::sync_with_stdio(false);
#endif
    cin.read(inbuf, sizeof(inbuf));
    const unsigned int n = readUInt(), q = readUInt();
    for (unsigned int i = 0; i < n; ++i)
        changes[i + 1].push_back(Modify { 0, readUInt() });
    for (unsigned int i = 1; i <= q; ++i)
    {
        const unsigned int op = readUInt(), x = readUInt();
        if (op == 1)
            changes[x].push_back(Modify { i, readUInt() });
        else
            querys[x].push_back(Query { i, qcur++ });
    }
    answerQuery(n, q);
    for_each(ans, qcur, writeUInt);
    cout.write(outbuf, outpos - outbuf);
    return 0;
}
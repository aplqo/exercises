#ifdef APTEST
#include "debug_tools/program.h"
#endif
#include <algorithm>
#include <cctype>
#include <cstdio>
#include <iostream>
#include <iterator>
using namespace std;
const unsigned int maxn = 1e6, maxm = 1000000;

char inbuf[maxn * 10 + 100 * 40 + maxm * 40 + 1000], *inpos = inbuf, outbuf[maxm * 11 + 1000], *outpos = outbuf;

unsigned int readUint()
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
void writeUint(const unsigned long long v)
{
    outpos += sprintf(outpos, "%llu\n", v);
}

class SegmentTree
{
public:
    void build(const unsigned int l, const unsigned int r, const unsigned int val[], SegmentTree*(&cur));
    void update(const unsigned int l, const unsigned int r, const unsigned int val);
    unsigned long long sum(const unsigned int l, const unsigned int r);
    unsigned int max(const unsigned int l, const unsigned int r);

private:
    inline void pushFirst(const unsigned int val);
    inline void pushdown();
    inline void pushup();

    unsigned int laz;
    unsigned int fst, snd, cnt;
    unsigned long long msum;
    SegmentTree *lc, *rc;
    unsigned int lv, mv, rv;
} nodes[maxn * 4 + 10], *root = nodes;

void SegmentTree::build(const unsigned int l, const unsigned int r, const unsigned int val[], SegmentTree*(&cur))
{
    lv = l;
    rv = r;
    mv = (l + r) >> 1;
    laz = 0;
    if (r - l == 1)
    {
        fst = msum = val[l];
        cnt = 1;
        snd = 0;
        return;
    }
    lc = ++cur;
    lc->build(l, mv, val, cur);
    rc = ++cur;
    rc->build(mv, r, val, cur);
    pushup();
}
void SegmentTree::update(const unsigned int l, const unsigned int r, const unsigned int val)
{
    if (fst <= val)
        return;
    if ((l == lv && r == rv && snd < val) || rv - lv == 1)
    {
        pushFirst(val);
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
unsigned long long SegmentTree::sum(const unsigned int l, const unsigned int r)
{
    if (l == lv && r == rv)
        return msum;
    pushdown();
    if (r <= mv)
        return lc->sum(l, r);
    else if (l >= mv)
        return rc->sum(l, r);
    else
        return lc->sum(l, mv) + rc->sum(mv, r);
}
unsigned int SegmentTree::max(const unsigned int l, const unsigned int r)
{
    if (l == lv && r == rv)
        return fst;
    pushdown();
    if (r <= mv)
        return lc->max(l, r);
    else if (l >= mv)
        return rc->max(l, r);
    else
        return std::max(lc->max(l, mv), rc->max(mv, r));
}
inline void SegmentTree::pushup()
{
    msum = lc->msum + rc->msum;
    SegmentTree *mx = lc, *mn = rc;
    if (mx->fst < mn->fst)
        swap(mx, mn);
    fst = mx->fst;
    cnt = mx->cnt;
    if (mx->fst != mn->fst)
        snd = std::max(mx->snd, mn->fst);
    else
    {
        snd = std::max(mx->snd, mn->snd);
        cnt += mn->cnt;
    }
}
inline void SegmentTree::pushFirst(const unsigned int val)
{
    if (fst <= val)
        return;
    const unsigned long long tp = cnt;
    msum = msum - tp * fst + tp * val;
    laz = fst = val;
}
inline void SegmentTree::pushdown()
{
    if (!laz)
        return;
    lc->pushFirst(laz);
    rc->pushFirst(laz);
    laz = 0;
}

int main()
{
#ifndef APTEST
    ios_base::sync_with_stdio(false);
#endif
    cin.read(inbuf, sizeof(inbuf));
    const unsigned int t = readUint();
    for (unsigned int i = t; i; --i)
    {
        static unsigned int a[maxn + 10];
        const unsigned int n = readUint(), m = readUint();
        generate_n(a + 1, n, readUint);
        {
            SegmentTree* cur = nodes;
            root->build(1, n + 1, a, cur);
        }
        for (unsigned int j = m; j; --j)
        {
            unsigned int op = readUint(), x = readUint(), y = readUint(), t;
            switch (op)
            {
            case 0:
                t = readUint();
                root->update(x, y + 1, t);
                break;
            case 1:
                writeUint(root->max(x, y + 1));
                break;
            case 2:
                writeUint(root->sum(x, y + 1));
                break;
            }
        }
    }
    cout.write(outbuf, outpos - outbuf);
    return 0;
}
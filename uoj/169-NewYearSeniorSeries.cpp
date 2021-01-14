#ifdef APTEST
#include "debug_tools/program.h"
#endif
#include <algorithm>
#include <cctype>
#include <cstdio>
#include <iostream>
#include <limits>
using namespace std;
const unsigned int maxn = 5e5, maxm = 5e5, maxc = 1024 * 1024 * 4;
constexpr int inf = numeric_limits<int>::max() / 2;

char outbuf[maxc + 10], *outpos = outbuf;
char inbuf[maxc + 10], *inpos = inbuf + maxc + 20;
template <class T>
T readInt()
{
    static const auto get = []() -> char {
        if (inpos - inbuf >= sizeof(inbuf))
        {
            cin.read(inbuf, sizeof(inbuf));
            inpos = inbuf;
        }
        return *(inpos++);
    };
    T ret = 0;
    bool neg = false;
    char c = get();
    while (!isdigit(c) && c != '-')
        c = get();
    if (c == '-')
    {
        neg = true;
        c = get();
    }
    while (isdigit(c))
    {
        ret = ret * 10 + (c - '0');
        c = get();
    }
    return neg ? -ret : ret;
}
inline void writeInt(const int v)
{
    if (outpos - outbuf > maxc - 10)
    {
        cout.write(outbuf, outpos - outbuf);
        outpos = outbuf;
    }
    outpos += sprintf(outpos, "%d\n", v);
}

struct Tag
{
    int val, add;
};
class SegmentTree
{
public:
    void build(const unsigned int l, const unsigned int r, const int v[], SegmentTree*(&cur));
    void add(const unsigned int l, const unsigned int r, const int delt);
    void update(const unsigned int l, const unsigned int r, const int delt);
    template <bool History>
    int min(const unsigned int l, const unsigned int r);

private:
    struct
    {
        Tag current, history;
        inline void add(const int delt, const int his)
        {
            history.add = std::min(history.add, current.add + his);
            history.val = std::min(history.val, current.val + his);
            current.add += delt;
            current.val += delt;
        }
        inline bool isDirty() const { return current.add || history.add; }
        inline void clearFlag() { current.add = history.add = 0; }
    } fst, snd;
    unsigned int lv, rv;
    unsigned int lc, rc;

    inline void push(const int fstAdd, const int sndAdd, const int hisAdd);
    inline void pushdown();
    inline void pushup();
} nodes[maxn * 4 + 10], *root = nodes;

void SegmentTree::build(const unsigned int l, const unsigned int r, const int v[], SegmentTree*(&cur))
{
    lv = l;
    rv = r;
    fst = snd = {};
    if (r - l == 1)
    {
        fst.current.val = fst.history.val = v[l];
        snd.current.val = snd.history.val = inf;
        return;
    }
    const unsigned int mv = (l + r) >> 1;
    lc = (++cur) - this;
    this[lc].build(l, mv, v, cur);
    rc = (++cur) - this;
    this[rc].build(mv, r, v, cur);
    pushup();
}
void SegmentTree::add(const unsigned int l, const unsigned int r, const int delt)
{
    if (rv - lv == 1)
    {
        fst.current.val += delt;
        fst.history.val = std::min(fst.history.val, fst.current.val);
        return;
    }
    pushdown();
    if (l == lv && r == rv)
    {
        fst.add(delt, delt);
        snd.add(delt, delt);
        return;
    }
    const unsigned int mv = (lv + rv) >> 1;
    if (r <= mv)
        this[lc].add(l, r, delt);
    else if (l >= mv)
        this[rc].add(l, r, delt);
    else
    {
        this[lc].add(l, mv, delt);
        this[rc].add(mv, r, delt);
    }
    pushup();
}
void SegmentTree::update(const unsigned int l, const unsigned int r, const int delt)
{
    if (delt < fst.current.val)
        return;
    if (rv - lv == 1)
    {
        fst.current.val = delt;
        fst.history.val = std::min(fst.history.val, fst.current.val);
        return;
    }
    pushdown();
    if (l == lv && r == rv && delt < snd.current.val)
    {
        const int v = delt - fst.current.val;
        fst.add(v, v);
        return;
    }
    const unsigned int mv = (lv + rv) >> 1;
    if (r <= mv)
        this[lc].update(l, r, delt);
    else if (l >= mv)
        this[rc].update(l, r, delt);
    else
    {
        this[lc].update(l, mv, delt);
        this[rc].update(mv, r, delt);
    }
    pushup();
}
template <bool History>
int SegmentTree::min(const unsigned int l, const unsigned int r)
{
    if (l == lv && r == rv)
        return History ? fst.history.val : fst.current.val;
    pushdown();
    const unsigned int mv = (lv + rv) >> 1;
    if (r <= mv)
        return this[lc].min<History>(l, r);
    else if (l >= mv)
        return this[rc].min<History>(l, r);
    else
        return std::min(this[lc].min<History>(l, mv), this[rc].min<History>(mv, r));
}
inline void SegmentTree::pushdown()
{
    if (fst.isDirty() || snd.isDirty())
    {
        const auto doPush = [this, mv = std::min(this[lc].fst.current.val, this[rc].fst.current.val)](SegmentTree* p) {
            if (p->fst.current.val <= mv)
                p->fst.add(fst.current.add, fst.history.add);
            else
                p->fst.add(snd.current.add, snd.history.add);
            p->snd.add(snd.current.add, snd.history.add);
        };
        doPush(this + lc);
        doPush(this + rc);
        fst.clearFlag();
        snd.clearFlag();
    }
}
inline void SegmentTree::pushup()
{
    const SegmentTree *mn = this + lc, *mx = this + rc;
    if (mn->fst.current.val > mx->fst.current.val)
        swap(mn, mx);
    fst.current.val = mn->fst.current.val;
    snd.current.val = std::min((mx->fst.current.val == fst.current.val ? &mx->snd : &mx->fst)->current.val, mn->snd.current.val);
    fst.history.val = std::min(mn->fst.history.val, mx->fst.history.val);
}

constexpr unsigned long long ms = sizeof(nodes) / 1024.0 / 1024.0;
static_assert(ms < 96, "Memory limit exceed");

int main()
{
#ifndef APTEST
    ios_base::sync_with_stdio(false);
#endif
    const unsigned int n = readInt<unsigned int>(), m = readInt<unsigned int>();
    {
        static int val[maxn + 10];
        generate_n(val + 1, n, readInt<int>);
        SegmentTree* c = nodes;
        root->build(1, n + 1, val, c);
    }
    for (unsigned int i = m; i; --i)
    {
        const unsigned int op = readInt<unsigned int>(), l = readInt<unsigned int>(), r = readInt<unsigned int>();
        switch (op)
        {
        case 1:
            root->add(l, r + 1, readInt<int>());
            break;
        case 2:
            root->update(l, r + 1, readInt<int>());
            break;
        case 3:
            writeInt(root->min<false>(l, r + 1));
            break;
        case 4:
            writeInt(root->min<true>(l, r + 1));
            break;
        }
    }
    cout.write(outbuf, outpos - outbuf);
    return 0;
}
#ifdef APTEST
#include "debug_tools/program.h"
#endif
#include <algorithm>
#include <iostream>
#include <numeric>
using namespace std;
const unsigned int maxn = 200000, maxv = maxn * 4;

class SegmentTree
{
public:
    void build(const unsigned int l, const unsigned int r, const unsigned int val);
    pair<unsigned long long, unsigned long long> merge(SegmentTree* const old);

    static SegmentTree* cur;

private:
    inline void init(const unsigned int l, const unsigned int r);
    unsigned int lv, rv;
    SegmentTree *lc, *rc;
    unsigned int siz;
} snodes[maxv * 4 * 10];
SegmentTree* SegmentTree::cur = snodes;
unsigned long long ans;

void SegmentTree::build(const unsigned int l, const unsigned int r, const unsigned int val)
{
    init(l, r);
    siz = 1;
    if (rv - lv == 1)
        return;
    const unsigned int mv = (lv + rv) >> 1;
    if (val < mv)
    {
        lc = cur++;
        lc->build(l, mv, val);
    }
    else
    {
        rc = cur++;
        rc->build(mv, r, val);
    }
}
typedef pair<unsigned long long, unsigned long long> MergeStat;
inline MergeStat operator+(const MergeStat a, const MergeStat b)
{
    return MergeStat { a.first + b.first, a.second + b.second };
}
pair<unsigned long long, unsigned long long> SegmentTree::merge(SegmentTree* const old)
{
    static const auto may = [](const SegmentTree* ptr) -> unsigned long long { return ptr ? ptr->siz : 0; };
    static const auto mergeSub = [](SegmentTree*(&a), SegmentTree* b) {
        if (!a)
        {
            a = b;
            return make_pair(0ull, 0ull);
        }
        else
            return a->merge(b);
    };
    if (!old || rv - lv == 1)
        return make_pair(0ull, 0ull);
    const auto p = make_pair(may(this->rc) * may(old->lc), may(this->lc) * may(old->rc));
    const auto ret = p + mergeSub(lc, old->lc) + mergeSub(rc, old->rc);
    siz = may(lc) + may(rc);
    return ret;
}
inline void SegmentTree::init(const unsigned int l, const unsigned int r)
{
    lv = l;
    rv = r;
    lc = rc = nullptr;
}

SegmentTree* dfs(const unsigned int n)
{
    static unsigned int sta = 0;
    const unsigned int x = ++sta;
    {
        unsigned int p;
        cin >> p;
        if (p)
        {
            SegmentTree* const rt = SegmentTree::cur++;
            rt->build(1, n + 1, p);
            return rt;
        }
    }
    SegmentTree* rt = dfs(n);
    const auto [v1, v2] = rt->merge(dfs(n));
    ans += min(v1, v2);
    return rt;
}

int main()
{
#ifndef APTEST
    ios_base::sync_with_stdio(false);
#endif
    unsigned int n;
    cin >> n;
    dfs(n);
    cout << ans << "\n";
    return 0;
}
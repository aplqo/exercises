#ifdef APTEST
#include "debug_tools/program.h"
#endif
#include <climits>
#include <cstring>
#include <iostream>
using namespace std;
const unsigned int maxn = 299995;

struct Edge
{
    unsigned int to;
    const Edge* pre;
} ed[maxn * 2 + 1], *cur = ed;
const Edge* head[maxn + 1];
class FTree
{
public:
    void update(unsigned int pos, const int delt);
    void clear() { memset(val, 0, sizeof(val)); }
    unsigned int query(const unsigned int l, const unsigned int r) const { return querySum(r) - querySum(l - 1); }

private:
    unsigned int querySum(unsigned int pos) const;
    int val[maxn + 1] = {};
} ft, subt;
unsigned int siz[maxn + 1], ms[maxn + 1], cnt[maxn + 1];
unsigned int n;

inline void addEdge(const unsigned int from, const unsigned int to)
{
    new (cur) Edge { to, head[from] };
    head[from] = cur++;
    new (cur) Edge { from, head[to] };
    head[to] = cur++;
}

#define lowbit(x) ((x) & -(x))
void FTree::update(unsigned int pos, const int delt)
{
    for (; pos <= n; pos += lowbit(pos))
        val[pos] += delt;
}
unsigned int FTree::querySum(unsigned int pos) const
{
    unsigned int ret = 0;
    for (; pos; pos -= lowbit(pos))
        ret += val[pos];
    return ret;
}
#undef lowbit

void dfsSize(const unsigned int x, const unsigned int f)
{
    siz[x] = 1;
    ms[x] = 0;
    for (const Edge* i = head[x]; i; i = i->pre)
    {
        const unsigned int to = i->to;
        if (to == f)
            continue;
        dfsSize(to, x);
        ms[x] = max(ms[x], siz[to]);
        siz[x] += siz[to];
    }
}
unsigned int treeCenter(const unsigned int n)
{
    dfsSize(1, 0);
    unsigned int ret = 0, mins = UINT_MAX;
    for (unsigned int i = 1; i <= n; ++i)
    {
        const unsigned int nv = max(ms[i], n - siz[i]);
        if (nv < mins)
        {
            mins = nv;
            ret = i;
        }
    }
    return ret;
}
inline unsigned int excludeSub(const unsigned int x, const unsigned int low)
{
    return x * 2 >= n ? low : n - x * 2;
}
inline unsigned int countTime(const FTree& t, const unsigned int x)
{
    return t.query(excludeSub(siz[x], 1), excludeSub(ms[x], 0));
}
pair<unsigned int, unsigned int> findMaxSub(const unsigned int x)
{
    unsigned int fs = 0, ss = 0;
    for (const Edge* i = head[x]; i; i = i->pre)
    {
        const unsigned int to = i->to;
        if (siz[to] > siz[fs])
        {
            ss = fs;
            fs = to;
        }
        else if (siz[to] > siz[ss])
            ss = to;
    }
    return make_pair(fs, ss);
}
void dfsCntChl(const unsigned int x, const unsigned int f)
{
    ft.update(siz[f], -1);
    ft.update(n - siz[x], 1);

    const unsigned int oldv = countTime(subt, x);
    for (const Edge* i = head[x]; i; i = i->pre)
        if (i->to != f)
            dfsCntChl(i->to, x);

    subt.update(siz[x], 1);
    cnt[x] = countTime(ft, x) - (countTime(subt, x) - oldv);

    ft.update(siz[f], 1);
    ft.update(n - siz[x], -1);
}
void dfsCnt(const unsigned int x)
{
    const static auto countRoot = [](const FTree& f, const unsigned int s) {
        return f.query(1, excludeSub(siz[s], 0));
    };
    const auto [rtFirst, rtSecond] = findMaxSub(x);
    unsigned int vfst = 0;
    for (const Edge* i = head[x]; i; i = i->pre)
    {
        const unsigned int to = i->to;
        if (to == rtFirst)
        {
            const unsigned int ov = countRoot(subt, to), ovs = countRoot(subt, rtSecond);
            dfsCntChl(to, x);
            vfst = countRoot(subt, to) - ov;
            cnt[x] = countRoot(subt, rtSecond) - ovs;
        }
        dfsCntChl(to, x);
    }
    cnt[x] += countRoot(ft, rtFirst) - vfst;
}
unsigned long long sum(const unsigned int n)
{
    unsigned long long ret = 0;
    for (unsigned int i = 1; i <= n; ++i)
        ret += 1ull * i * cnt[i];
    return ret;
}
void solve()
{
    cin >> n;
    for (unsigned int i = 1; i < n; ++i)
    {
        unsigned int a, b;
        cin >> a >> b;
        addEdge(a, b);
    }
    const unsigned int root = treeCenter(n);
    dfsSize(root, 0);
    for (unsigned int i = 1; i <= n; ++i)
        ft.update(siz[i], 1);
    dfsCnt(root);
    cout << sum(n) << "\n";
}

int main()
{
#ifndef APTEST
    ios_base::sync_with_stdio(false);
#endif
    unsigned int t;
    cin >> t;
    for (unsigned int i = 0; i < t; ++i)
    {
        solve();
        memset(head, 0, sizeof(head));
        cur = ed;
        ft.clear();
        subt.clear();
    }
    return 0;
}
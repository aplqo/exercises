#ifdef APTEST
#include "debug_tools/program.h"
#endif
#include <algorithm>
#include <iostream>
#include <iterator>
#include <numeric>
#include <vector>
using namespace std;
const unsigned int maxn = 3e5, maxm = 3e5, maxk = 3e5;

vector<unsigned int> pos[maxn + 1];
unsigned int m;
namespace ftree
{
#define lowbit(x) ((x) & -(x))
    long long val[maxm + 10];
    bool vis[maxm + 10];
    unsigned int stk[maxm + 10], *top = stk;

    void modify(unsigned int p, const long long v)
    {
        if (!vis[p])
        {
            vis[p] = true;
            *(top++) = p;
        }
        for (; p <= m + 1; p += lowbit(p))
            val[p] += v;
    }
    void add(const unsigned int l, const unsigned int r, const long long v)
    {
        modify(l, v);
        modify(r, -v);
    }
    long long query(unsigned int p)
    {
        long long ret = 0;
        for (; p; p -= lowbit(p))
            ret += val[p];
        return ret;
    }
    void clear()
    {
        for (unsigned int* i = stk; i < top; ++i)
        {
            for (unsigned int j = *i; j <= m + 1; j += lowbit(j))
                val[j] = 0;
            vis[*i] = false;
        }
        top = stk;
    }

#undef lowbit
}
struct Rain
{
    unsigned int l, r, a = 0;
} rn[maxk + 5];
unsigned int country[maxn + 1], ans[maxn + 1];
unsigned long long p[maxn + 1];

void addMeteors(const unsigned int vl, const unsigned int vr)
{
    for (Rain* i = rn + vl; i < rn + vr; ++i)
        if (i->l <= i->r)
            ftree::add(i->l, i->r + 1, i->a);
        else
        {
            ftree::add(i->l, m + 1, i->a);
            ftree::add(1, i->r + 1, i->a);
        }
}
inline unsigned long long countMeteors(const unsigned int c)
{
    unsigned long long ret = 0;
    for (unsigned int i : pos[c])
        ret += ftree::query(i);
    return ret;
}
void binary(const unsigned int vl, const unsigned int vr, unsigned int* const ql, unsigned int* const qr)
{
    static unsigned int tmpl[maxn + 1], tmpr[maxn + 1];
    if (vr - vl == 1)
    {
        for (unsigned int* i = ql; i < qr; ++i)
            ans[*i] = vl;
        return;
    }
    const unsigned int mid = (vl + vr + 1) >> 1;
    addMeteors(vl, mid);
    unsigned int *pl = tmpl, *pr = tmpr;
    for (unsigned int* i = ql; i < qr; ++i)
    {
        const unsigned long long c = countMeteors(*i);
        const unsigned int v = *i;
        if (c < p[v])
        {
            *(pr++) = v;
            p[v] -= c;
        }
        else
            *(pl++) = v;
    }
    ftree::clear();
    unsigned int* const mp = ql + (pl - tmpl);
    copy(tmpl, pl, ql);
    copy(tmpr, pr, mp);
    binary(vl, mid, ql, mp);
    binary(mid, vr, mp, qr);
}

int main()
{
#ifndef APTEST
    ios_base::sync_with_stdio(false);
#endif
    unsigned int n, k;
    cin >> n >> m;
    for (unsigned int i = 1; i <= m; ++i)
    {
        unsigned int t;
        cin >> t;
        pos[t].push_back(i);
    }
    copy_n(istream_iterator<unsigned int>(cin), n, p + 1);
    cin >> k;
    for (Rain* i = rn + 1; i < rn + k + 1; ++i)
        cin >> i->l >> i->r >> i->a;
    iota(country, country + n, 1);
    binary(1, k + 2, country, country + n);
    for (unsigned int i = 1; i <= n; ++i)
        if (ans[i] == k + 1)
            cout << "NIE" << endl;
        else
            cout << ans[i] << endl;
    return 0;
}
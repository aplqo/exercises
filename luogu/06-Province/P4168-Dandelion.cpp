#ifdef APTEST
#include "debug_tools/program.h"
#endif
#include <algorithm>
#include <cmath>
#include <iostream>
#include <iterator>
#include <numeric>
using namespace std;
const unsigned int maxn = 40000, maxb = 200;

unsigned int bsize;
unsigned int cnt[maxn + 1][maxb + 1], f[maxb + 1][maxn + 1];
unsigned int a[maxn + 1], orig[maxn + 1];

static void discretize(const unsigned int n)
{
    copy(a, a + n, orig);
    sort(orig, orig + n);
    unsigned int* const e = unique(orig, orig + n);
    for (unsigned int* i = a; i < a + n; ++i)
        *i = lower_bound(orig, e, *i) - orig;
}
static void init(const unsigned int n)
{
    bsize = sqrt(n);
    f[0][0] = a[0];
    cnt[a[0]][0] = 1;
    for (unsigned int i = 1; i < n; ++i)
    {
        const unsigned int cb = i / bsize;
        ++cnt[a[i]][cb];
        if (i % bsize == 0)
        {
            for (unsigned int j = 0; j < n; ++j)
                cnt[j][cb] += cnt[j][cb - 1];
            f[cb][i - 1] = n;
        }
        for (int j = cb; j >= 0; --j)
        {
            static const auto count = [](const unsigned int x, const unsigned int b, const unsigned int e) {
                return cnt[x][e] - (b ? cnt[x][b - 1] : 0);
            };

            const unsigned int t = f[j][i - 1];
            const unsigned int st = count(t, j, cb), sc = count(a[i], j, cb);
            if (st != sc)
                f[j][i] = st > sc ? t : a[i];
            else
                f[j][i] = min(a[i], t);
        }
    }
}
unsigned int query(const unsigned int l, const unsigned int r)
{
    static unsigned int vis[maxn + 1], side[maxn + 1], val[maxn + 1];
    static unsigned int sta = 0, *pos = side;
    static const auto check = [](const unsigned int l, const unsigned int r) {
        for (unsigned int* i = a + l; i < a + r; ++i)
            if (vis[*i] < sta)
            {
                *(pos++) = *i;
                vis[*i] = sta;
                val[*i] = 1;
            }
            else
                ++val[*i];
    };

    const unsigned int indl = (l + bsize) / bsize, indr = (r - 1) / bsize;
    ++sta;
    if (indl < indr)
    {
        check(l, indl * bsize);
        check(indr * bsize, r);
    }
    else
        check(l, r);

    static const auto pred = [](pair<unsigned int, unsigned int> a, pair<unsigned int, unsigned int> b) {
        return a.first != b.first ? a.first < b.first : a.second > b.second;
    };
    static const auto count = [](const unsigned int v, const unsigned int b, const unsigned int e) {
        return cnt[v][e] - cnt[v][b - 1];
    };

    pair<unsigned int, unsigned int> ret(0, 0);
    if (indl < indr)
    {
        ret.second = f[indl][r - 1];
        ret.first = count(ret.second, indl, indr - 1) + (vis[ret.second] == sta ? val[ret.second] : 0);
        for (unsigned int* i = side; i < pos; ++i)
            ret = max(ret, make_pair(count(*i, indl, indr - 1) + val[*i], *i), pred);
    }
    else
    {
        for (unsigned int* i = side; i < pos; ++i)
            ret = max(ret, make_pair(val[*i], *i), pred);
    }
    pos = side;
    return orig[ret.second];
}
int main()
{
#ifndef APTEST
    ios_base::sync_with_stdio(false);
#endif
    unsigned int n, m;
    cin >> n >> m;
    copy_n(istream_iterator<unsigned int>(cin), n, a);
    discretize(n);
    init(n);

    unsigned int lst = 0;
    for (unsigned int i = 0; i < m; ++i)
    {
        unsigned int l, r;
        cin >> l >> r;
#ifndef NoEncode
        l = (static_cast<unsigned long long>(l) + lst - 1) % n + 1;
        r = (static_cast<unsigned long long>(r) + lst - 1) % n + 1;
#endif
        if (l > r)
            swap(l, r);
        cout << (lst = query(l - 1, r)) << endl;
    }
    return 0;
}
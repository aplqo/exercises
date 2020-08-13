#ifdef APTEST
#include "debug_tools/program.h"
#endif
#include <algorithm>
#include <climits>
#include <cstring>
#include <iostream>
#include <iterator>
#include <numeric>
using namespace std;
const unsigned int maxn = 300000;

enum
{
    Min = 0,
    Max = 1
};
char str[maxn + 10];
unsigned int sa[maxn + 1], Rank[maxn + 1], hight[maxn + 1];
long long a[maxn + 10], sel[maxn + 1], mv[2][maxn + 1], maxv[maxn + 1];

namespace dsu
{
    unsigned int fa[maxn + 1], siz[maxn + 1];
    static void init(const unsigned int n)
    {
        fill(siz, siz + 1 + n, 1);
        iota(fa, fa + 1 + n, 0);
    }
    unsigned int getFather(const unsigned int x)
    {
        return fa[x] == x ? x : fa[x] = getFather(fa[x]);
    }
    unsigned int merge(unsigned int x, unsigned int y)
    {
        if (siz[x] < siz[y])
            swap(x, y);
        fa[y] = x;
        siz[x] += siz[y];
        return x;
    }
}
void radixSort(const size_t l, unsigned int rk[], unsigned int tp[])
{
    static unsigned int buk[maxn + 1];
    memset(buk, 0, sizeof(buk));
    for (unsigned int i = 1; i <= l; ++i)
        ++buk[rk[tp[i]]];
    partial_sum(buk, buk + maxn + 1, buk);
    for (unsigned int i = l; i; --i)
        sa[buk[rk[tp[i]]]--] = tp[i];
}
void buildSA(const char str[], const size_t l)
{
    static unsigned int tmp[maxn + 1];
    unsigned int *rk = Rank, *tp = tmp;
    copy(str, str + 1 + l, rk);
    iota(tp, tp + 1 + l, 0);
    radixSort(l, rk, tp);
    for (unsigned int i = 1, p = 0; p < l; i <<= 1)
    {
        {
            unsigned int e = 1;
            for (unsigned int j = l + 1 - i; j <= l; ++j)
                tp[e++] = j;
            for (unsigned int j = 1; j <= l; ++j)
                if (sa[j] > i)
                    tp[e++] = sa[j] - i;
        }
        radixSort(l, rk, tp);
        swap(rk, tp);
        rk[sa[1]] = p = 1;
        for (unsigned int j = 2; j <= l && sa[j]; ++j)
            if (tp[sa[j]] == tp[sa[j - 1]] && tp[sa[j] + i] == tp[sa[j - 1] + i])
                rk[sa[j]] = p;
            else
                rk[sa[j]] = ++p;
    }
    for (unsigned int i = 1; i <= l; ++i)
        Rank[sa[i]] = i;
    for (unsigned int i = 1, k = 0; i <= l; ++i)
    {
        if (k)
            --k;
        while (str[i + k] == str[sa[Rank[i] - 1] + k])
            ++k;
        hight[Rank[i]] = k;
    }
}
void solve(const unsigned int n)
{
    static unsigned int seq[maxn + 1];
    for (unsigned int i = 1; i <= n; ++i)
        mv[Max][i] = mv[Min][i] = a[sa[i]];
    fill(maxv, maxv + n + 1, LLONG_MIN);
    iota(seq, seq + n - 1, 2);
    sort(seq, seq + n - 1, [](unsigned int x, unsigned int y) { return hight[x] > hight[y]; });
    dsu::init(n);
    for (unsigned int i = 0; i < n - 1; ++i)
    {
        const unsigned int h = hight[seq[i]], fx = dsu::getFather(seq[i] - 1), fy = dsu::getFather(seq[i]);
        if (fx == fy)
            continue;
        sel[h] += 1ull * dsu::siz[fx] * dsu::siz[fy];
        maxv[h] = max({ maxv[h], mv[Max][fx] * mv[Max][fy], mv[Min][fx] * mv[Min][fy] });
        long long cm[2];
        cm[Min] = min(mv[Min][fx], mv[Min][fy]);
        cm[Max] = max(mv[Max][fx], mv[Max][fy]);
        const unsigned int m = dsu::merge(fx, fy);
        mv[Min][m] = cm[Min];
        mv[Max][m] = cm[Max];
    }
}
void sum(const int n)
{
    for (int i = n - 1; i >= 0; --i)
    {
        sel[i] += sel[i + 1];
        maxv[i] = max(maxv[i], maxv[i + 1]);
    }
}

int main()
{
#ifndef APTEST
    ios_base::sync_with_stdio(false);
#endif
    unsigned int n;
    cin >> n >> (str + 1);
    copy_n(istream_iterator<long long>(cin), n, a + 1);
    buildSA(str, n);
    solve(n);
    sum(n);
    for (unsigned int i = 0; i < n; ++i)
        cout << sel[i] << " " << (sel[i] ? maxv[i] : 0) << endl;
    return 0;
}
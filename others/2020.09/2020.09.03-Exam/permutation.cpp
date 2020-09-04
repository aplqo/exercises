#ifdef APTEST
#include "debug_tools/program.h"
#endif
#include <algorithm>
#include <cmath>
#include <fstream>
#include <iostream>
#include <numeric>
using namespace std;
const unsigned int maxn = 2000;
const unsigned long long mod = 998244353;

class Number
{
public:
    Number() = default;
    Number(const unsigned long long v)
        : val(v % mod) {};
#define decl(x)                                                                                       \
    friend inline Number operator x(const Number l, const Number r) { return (l.val + mod) x r.val; } \
    inline Number& operator x##=(const Number r) { return *this = *this x r; }
    decl(+);
    decl(*);
    decl(-);
#undef decl
    friend inline ostream& operator<<(ostream& os, const Number v)
    {
        return os << v.val;
    }

private:
    unsigned long long val = 0;
};
Number fac[maxn + 1], g[maxn * 2 + 1], ch[maxn * 2 + 1][maxn * 2 + 1][2];

namespace dsu
{
    unsigned int fa[maxn * 2 + 1], siz[maxn * 2 + 1];

    static void init(const unsigned int n)
    {
        fill(siz, siz + n + 1, 1);
        iota(fa, fa + n + 1, 0);
    }
    unsigned int find(const unsigned int x)
    {
        return fa[x] == x ? x : fa[x] = find(fa[x]);
    }
    void merge(unsigned int x, unsigned int y)
    {
        x = find(x);
        y = find(y);
        if (x == y)
            return;
        if (siz[x] < siz[y])
            swap(x, y);
        fa[y] = x;
        siz[x] += siz[y];
    }
}
static void factor(const unsigned int n)
{
    fac[0] = 1;
    for (unsigned int i = 1; i <= n; ++i)
        fac[i] = fac[i - 1] * i;
}
void ChainDp(Number chain[][maxn * 2 + 1][2], const unsigned int ml)
{
    chain[1][0][0] = 1;
    for (unsigned int i = 2; i <= ml; ++i)
    {
        chain[i][0][0] = 1;
        for (unsigned int j = 1; j <= i; ++j)
        {
            chain[i][j][0] = chain[i - 1][j][0] + chain[i - 1][j][1];
            chain[i][j][1] = chain[i - 1][j - 1][0];
        }
    }
}
void MergeChains(Number chain[][maxn * 2 + 1][2], const unsigned int n)
{
    using dsu::siz;
    unsigned int CurSiz = 0;
    g[0] = 1;
    for (unsigned int i = 1; i <= n * 2; ++i)
    {
        if (dsu::find(i) != i || siz[i] == 1)
            continue;
        CurSiz += siz[i];
        for (unsigned int j = CurSiz - 1; j > 0; --j)
        {
            const unsigned int mk = min(j, dsu::siz[i] - 1);
            Number v = 0;
            for (unsigned int k = 0; k <= mk; ++k)
                v += g[j - k] * (chain[siz[i]][k][0] + chain[siz[i]][k][1]);
            g[j] = v;
        }
    }
}
Number count(const unsigned int n)
{
    Number ret = 0;
    for (unsigned int i = 0; i <= n; ++i)
    {
        if (i & 0x01)
            ret -= g[i] * fac[n - i];
        else
            ret += g[i] * fac[n - i];
    }
    return ret;
}
static void BuildGraph(const unsigned int n, const unsigned int k)
{
    dsu::init(n * 2);
    for (unsigned int i = 1; i <= n; ++i)
    {
        if (i > k)
            dsu::merge(i, n + i - k);
        if (i + k <= n)
            dsu::merge(i, n + i + k);
    }
}

int main()
{
#ifndef APTEST
    ios_base::sync_with_stdio(false);
#endif
    unsigned int n, k;
    cin >> n >> k;
    BuildGraph(n, k);
    factor(n);
    {
        ChainDp(ch, n * 2);
        MergeChains(ch, n);
    }
    cout << count(n) << "\n";
    return 0;
}

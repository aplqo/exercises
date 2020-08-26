#ifdef APTEST
#include "debug_tools/program.h"
#endif
#include <algorithm>
#include <iostream>
#include <iterator>
using namespace std;
const unsigned int maxn = 500, maxk = 500, maxh = 1000000;
constexpr unsigned long long mod = 1000000007;

class Number
{
public:
    Number() = default;
    Number(const unsigned long long v)
        : val(v % mod) {};
#define decl(x)                                                      \
    friend inline Number operator x(const Number l, const Number r); \
    inline Number& operator x##=(const Number r) { return *this = *this x r; }
    decl(+);
    decl(*);
#undef decl
    friend inline ostream& operator<<(ostream& os, const Number n);

private:
    unsigned long long val = 0;
};
#define impl(x) \
    inline Number operator x(const Number l, const Number r) { return l.val x r.val; };
impl(+);
impl(*);
#undef impl
inline ostream& operator<<(ostream& os, const Number n)
{
    return os << n.val;
}

unsigned int lc[maxn + 1], rc[maxn + 1], siz[maxn + 1];
unsigned int h[maxn + 1], k;
Number f[maxn + 1][maxk + 1], fac[maxh + 1], ifac[maxh + 1];

Number quickPow(Number a, unsigned long long e)
{
    Number ret = 1;
    for (; e; e >>= 1)
    {
        if (e & 0x01)
            ret *= a;
        a *= a;
    }
    return ret;
}
static void initFac(const unsigned int n)
{
    ifac[0] = fac[0] = 1;
    for (unsigned int i = 1; i <= n; ++i)
    {
        fac[i] = fac[i - 1] * i;
        ifac[i] = quickPow(fac[i], mod - 2);
    }
}
static unsigned int build(const unsigned int n)
{
    static unsigned int stk[maxn + 1];
    unsigned int *top = stk, root = 1;
    *(top++) = 1;
    for (unsigned int i = 2; i <= n; ++i)
    {
        unsigned int* ptr = top;
        while (ptr > stk && h[*(ptr - 1)] > h[i])
            --ptr;
        if (ptr > stk)
            rc[*(ptr - 1)] = i;
        else
            root = i;
        if (ptr < top)
            lc[i] = *ptr;
        top = ptr;
        *(top++) = i;
    }
    return root;
}
inline Number C(const unsigned int n, const unsigned int k)
{
    return k > n ? 0 : fac[n] * ifac[n - k] * ifac[k];
}
inline Number count(const unsigned int r, const unsigned int c, const unsigned int k)
{
    return fac[k] * C(r, k) * C(c, k);
}
void dfs(const unsigned int x, const unsigned int fh)
{
    static Number g[maxn + 1][maxn + 1];
    if (!x)
        return;
    dfs(lc[x], h[x]);
    dfs(rc[x], h[x]);
    siz[x] = siz[lc[x]] + siz[rc[x]] + 1;

    const unsigned int l = lc[x], r = rc[x];
    for (unsigned int i = 0; i <= siz[x]; ++i)
        for (unsigned int j = 0; j <= siz[l] && j <= i; ++j)
            g[x][i] += f[l][j] * f[r][i - j];
    f[x][0] = 1;
    for (unsigned int i = 1; i <= siz[x]; ++i)
        for (unsigned int j = 0; j <= i; ++j)
            f[x][i] += g[x][j] * count(siz[x] - j, h[x] - fh, i - j);
}

int main()
{
#ifndef APTEST
    ios_base::sync_with_stdio(false);
#endif
    unsigned int n;
    cin >> n >> k;
    if (k > n)
    {
        cout << "0" << endl;
        return 0;
    }
    copy_n(istream_iterator<unsigned int>(cin), n, h + 1);
    const unsigned int root = build(n);
    initFac(max(n, *max_element(h, h + 1 + n)));
    f[0][0] = 1;
    dfs(root, 0);
    cout << f[root][k] << "\n";
    return 0;
}
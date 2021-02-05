#ifdef APTEST
#include "debug_tools/judge.h"
#endif
#include <algorithm>
#include <cstring>
#include <iostream>
#include <numeric>
const unsigned int maxn = 1e5, maxm = 1e5;
constexpr unsigned long long mod = 1e9;

struct Constraint
{
    unsigned int x, y;
    bool val;
} lim[maxn + 10];

unsigned long long quickPow(unsigned long long a, unsigned long long e)
{
    unsigned long long ret = 1;
    for (; e; e >>= 1)
    {
        if (e & 0x01)
            ret = (ret * a) % mod;
        a = (a * a) % mod;
    }
    return ret;
}
namespace Disjoint
{
    unsigned int fa[(maxn + maxm) * 2 + 10], size[(maxn + maxm) * 2 + 10];
    unsigned int n, m, offset;

    static void init(const unsigned int n, const unsigned int m)
    {
        Disjoint::n = n;
        Disjoint::m = m;
        offset = n + m;
        std::fill(size, size + (n + m) * 2, 1);
        std::iota(fa, fa + (n + m) * 2, 0);
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
        if (size[x] < size[y])
            std::swap(x, y);
        fa[y] = x;
        size[x] += size[y];
    }
    inline void set(const unsigned int x, const unsigned int y, const bool diff)
    {
        if (!diff)
        {
            merge(x, y);
            merge(x + offset, y + offset);
        }
        else
        {
            merge(x, y + offset);
            merge(x + offset, y);
        }
    }
    bool check()
    {
        for (unsigned int i = 0; i < offset; ++i)
            if (find(i) == find(i + offset))
                return false;
        return true;
    }
    unsigned int countBlock()
    {
        unsigned int ret = 0;
        for (unsigned int i = 0; i < offset * 2; ++i)
            ret += find(i) == i;
        return ret;
    }
}
template <bool va>
unsigned long long solve(const unsigned int n, const unsigned int m, const unsigned int k)
{
    static bool vis[(maxn + maxm) * 2 + 10];
    Disjoint::init(n, m);
    std::memset(vis, 0, sizeof(vis));
    for (const Constraint* i = lim; i < lim + k; ++i)
        Disjoint::set(i->x, i->y ? i->y + n : 0, (i->x & 0x01) && (i->y & 0x01) ? va ^ 1 ^ i->val : va ^ i->val);
    if (!Disjoint::check())
        return 0;
    unsigned int ret = Disjoint::countBlock() - 2;
    vis[n] = vis[n + m + n] = true;
    auto check = [&ret](unsigned int x) {
        if (const unsigned int rt = Disjoint::find(x); !vis[rt])
        {
            vis[rt] = true;
            --ret;
        }
    };
    for (const Constraint* i = lim; i < lim + k; ++i)
        if (!i->x)
        {
            const unsigned int y = i->y ? i->y + n : 0;
            check(y);
            check(y + n + m);
        }
        else if (!i->y)
        {
            check(i->x);
            check(i->x + n + m);
        }
    check(0);
    return quickPow(2, ret / 2);
}

int main()
{
    std::ios::sync_with_stdio(false);
    unsigned int n, m, k;
    std::cin >> n >> m >> k;
    bool lim1 = false, val1;
    for (Constraint* i = lim; i < lim + k; ++i)
    {
        unsigned int x, y, c;
        std::cin >> x >> y >> c;
        if (x == 1 && y == 1)
        {
            lim1 = true;
            val1 = c;
        }
        *i = Constraint { x - 1, y - 1, c };
    }
    if (lim1)
        std::cout << (val1 ? solve<true>(n, m, k) : solve<false>(n, m, k)) << "\n";
    else
        std::cout << (solve<true>(n, m, k) + solve<false>(n, m, k)) % mod << "\n";
    return 0;
}

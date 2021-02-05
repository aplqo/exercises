#ifdef APTEST
#include "debug_tools/judge.h"
#endif
#include <algorithm>
#include <iostream>
const unsigned int maxn = 100000, maxv = maxn * 3;

struct Object
{
    long long val, l, r;
} a[maxn + 10];

namespace FTree
{
#define lowbit(x) ((x) & -(x))
    unsigned int val[maxv + 10];

    void add(unsigned int p, const unsigned int n)
    {
        for (; p <= n; p += lowbit(p))
            ++val[p];
    }
    unsigned int prefixSum(unsigned int p)
    {
        unsigned int ret = 0;
        for (; p; p -= lowbit(p))
            ret += val[p];
        return ret;
    }
    unsigned int sum(const unsigned int l, const unsigned int r)
    {
        return prefixSum(r) - prefixSum(l - 1);
    }
#undef lowbit
}
unsigned int discretize(const unsigned int n)
{
    static long long tmp[maxv + 10];
    long long* cur = tmp;
    for (const Object* i = a; i < a + 1 + n; ++i)
    {
        *(cur++) = i->val;
        *(cur++) = i->l;
        *(cur++) = i->r;
    }
    std::sort(tmp, cur);
    cur = std::unique(tmp, cur);
    for (Object* i = a; i < a + 1 + n; ++i)
    {
        i->val = std::lower_bound(tmp, cur, i->val) - tmp + 1;
        i->l = std::lower_bound(tmp, cur, i->l) - tmp + 1;
        i->r = std::lower_bound(tmp, cur, i->r) - tmp + 1;
    }
    return cur - tmp;
}
unsigned long long count(const unsigned int n, const unsigned int dcnt)
{
    unsigned long long ret = 0;
    FTree::add(a[0].val, dcnt);
    for (const Object* i = a + 1; i < a + 1 + n; ++i)
    {
        ret += FTree::sum(i->l, i->r);
        FTree::add(i->val, dcnt);
    }
    return ret;
}

int main()
{
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);
    std::cout.tie(nullptr);
    unsigned int n, l, r;
    std::cin >> n >> l >> r;
    long long acc = 0;
    for (Object* i = a + 1; i < a + n + 1; ++i)
    {
        long long v;
        std::cin >> v;
        acc += v;
        i->val = acc;
        i->l = acc - r;
        i->r = acc - l;
    }
    std::cout << count(n, discretize(n)) << "\n";
    return 0;
}
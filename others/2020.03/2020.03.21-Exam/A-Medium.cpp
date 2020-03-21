#ifdef APTEST
#include "debug_tools/program.h"
#endif
#include <algorithm>
#include <iostream>
#define lowbit(x) ((x) & -(x))
using namespace std;
const int maxn = 100000;

unsigned int a[maxn + 10], tree[maxn + 10];
unsigned int orig[maxn + 10];

inline void discretize(const unsigned int n)
{
    copy(a + 1, a + n + 1, orig + 1);
    sort(orig + 1, orig + n + 1);
    unsigned int* en = unique(orig + 1, orig + n + 1);
    transform(a + 1, a + n + 1, a + 1, [en](unsigned int i) -> unsigned int {
        return lower_bound(orig + 1, en, i) - orig;
    });
}
inline void modify(unsigned int p, const unsigned int n)
{
    for (; p < n; p += lowbit(p))
        ++tree[p];
}
inline unsigned int query(unsigned int p)
{
    unsigned int ret = 0;
    for (; p; p -= lowbit(p))
        ret += tree[p];
    return ret;
}
unsigned int find(unsigned int val, const unsigned int n)
{
    unsigned int l = 1, r = n;
    while (l <= r)
    {
        unsigned int m = (l + r) >> 1;
        if (query(m) >= val)
            r = m - 1;
        else
            l = m + 1;
    }
    return orig[l];
}
int main()
{
#ifndef APTEST
    ios_base::sync_with_stdio(false);
#endif
    unsigned int n;
    cin >> n;
    for_each(a + 1, a + 1 + n, [](unsigned int& i) -> void { cin >> i; });
    discretize(n);
    a[n + 1] = 1;
    for (unsigned int i = 1; i <= n; i += 2)
    {
        modify(a[i], n);
        cout << find((i + 1) >> 1, n) << endl;
        modify(a[i + 1], n);
    }
    return 0;
}
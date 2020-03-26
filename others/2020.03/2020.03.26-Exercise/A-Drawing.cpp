#ifdef APTEST
#include "debug_tools/program.h"
#endif
#include <algorithm>
#include <iostream>
#include <numeric>
using namespace std;
const int maxn = 1000, maxm = 100000;

unsigned int degree[maxn + 10], blk[maxn + 10];
namespace find
{
    unsigned int fa[maxn + 10], siz[maxn + 10];
    inline void Init(const unsigned int n)
    {
        iota(fa, fa + n + 1, 0);
        fill(siz, siz + n + 1, 1);
    }
    unsigned int GetFather(const unsigned int x)
    {
        return fa[x] == x ? x : fa[x] = GetFather(fa[x]);
    }
    inline void Merge(const unsigned int x, const unsigned int y)
    {
        unsigned int fx = GetFather(x), fy = GetFather(y);
        if (fx == fy)
            return;
        if (siz[fx] < siz[fy])
            swap(fx, fy);
        fa[fy] = fx;
        siz[fy] += fx;
    }
} // namespace find
int main()
{
#ifndef APTEST
    ios_base::sync_with_stdio(false);
#endif
    unsigned int n, m;
    cin >> n >> m;
    find::Init(n);
    for (unsigned int i = 0; i < m; ++i)
    {
        unsigned int a, b;
        cin >> a >> b;
        find::Merge(a, b);
        ++degree[a];
        ++degree[b];
    }

    bool f = true;
    const unsigned int r = find::GetFather(1);
    unsigned int ans = 0;
    for (unsigned int i = 1; i <= n; ++i)
    {
        const unsigned int fa = find::GetFather(i);
        f &= (fa == r);
        if (degree[i] & 0x01)
        {
            ++blk[fa];
            ++ans;
            f = false;
        }
    }
    if (f)
        ans = 0;
    else
    {
        for (unsigned int i = 1; i <= n; ++i)
            if (find::GetFather(i) == i && !blk[i])
                ans += 2;
    }
    cout << (ans / 2 + (ans & 0x01)) << endl;
    return 0;
}

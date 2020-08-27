#ifdef APTEST
#include "debug_tools/program.h"
#endif
#include <algorithm>
#include <cmath>
#include <iostream>
using namespace std;
const unsigned int maxn = 5000, maxx = 5000, maxy = 5000;
const unsigned int maxv = maxn;
constexpr unsigned long long mod = 1000000007;

struct Point
{
    int x, y;
} p[maxn + 1];
unsigned int n;
unsigned int vis[maxv + 1], vsta;
bool typ[maxv + 1];

inline unsigned int distance(const Point a, const Point b)
{
    return abs(a.x - b.x) + abs(a.y - b.y);
}
bool dfs(const unsigned int x, const unsigned int val)
{
    vis[x] = vsta;
    for (unsigned int i = 0; i < n; ++i)
    {
        if (distance(p[i], p[x]) <= val)
            continue;
        if (vis[i] == vsta)
        {
            if (typ[i] == typ[x])
                return false;
        }
        else
        {
            typ[i] = !typ[x];
            if (!dfs(i, val))
                return false;
        }
    }
    return true;
}
pair<bool, unsigned int> test(const unsigned int n, const unsigned int val)
{
    ++vsta;
    unsigned int blk = 0;
    for (unsigned int i = 0; i < n; ++i)
        if (vis[i] < vsta)
        {
            ++blk;
            if (!dfs(i, val))
                return make_pair(false, 0);
        }
    return make_pair(true, blk);
}
pair<unsigned int, unsigned int> binary(const unsigned int n)
{
    int l = 0, r = maxx + maxy;
    unsigned int cblk = 0;
    while (l <= r)
    {
        const int mid = (l + r) / 2;
        const auto [suc, blk] = test(n, mid);
        if (suc)
        {
            r = mid - 1;
            cblk = blk;
        }
        else
            l = mid + 1;
    }
    return make_pair(l, cblk);
}
unsigned long long quickPow(unsigned int n)
{
    unsigned long long ret = 1;
    for (unsigned long long v = 2; n; n >>= 1)
    {
        if (n & 0x01)
            ret = (ret * v) % mod;
        v = (v * v) % mod;
    }
    return ret;
}
int main()
{
#ifndef APTEST
    ios_base::sync_with_stdio(false);
#endif
    cin >> n;
    for (Point* i = p; i < p + n; ++i)
        cin >> i->x >> i->y;
    const auto [cst, blk] = binary(n);
    cout << cst << "\n"
         << quickPow(blk) << "\n";
    return 0;
}
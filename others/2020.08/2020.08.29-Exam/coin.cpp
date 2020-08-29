#ifdef APTEST
#include "debug_tools/program.h"
#endif
#include <algorithm>
#include <iostream>
#include <numeric>
using namespace std;
const unsigned int maxh = 100, maxw = 100;
constexpr unsigned int maxv = (maxh + maxw) * 2;

bool vis[maxv + 1];
unsigned int fa[maxv + 1], siz[maxv + 1], selCnt[maxv + 1];

static void init(const unsigned int n)
{
    iota(fa, fa + 1 + n, 0);
    fill(vis, vis + 1 + n, false);
    fill(siz, siz + 1 + n, 1);
    for (unsigned int i = 0; i < n; ++i)
        selCnt[i] = i & 0x01;
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
    selCnt[x] += selCnt[y];
}

void readInput(const unsigned int h, const unsigned int w)
{
    for (unsigned int i = 0; i < h; ++i)
    {
        static char buf[maxw + 1];
        cin >> buf;
        for (unsigned int j = 0; j < w; ++j)
            switch (buf[j])
            {
            case 'o':
                merge(i * 2, (h + j) * 2);
                merge(i * 2 + 1, (h + j) * 2 + 1);
                break;
            case 'e':
                break;
            default:
                merge(i * 2 + 1, (h + j) * 2);
                merge(i * 2, (h + j) * 2 + 1);
                break;
            }
    }
}
bool isAllP(const unsigned int h, const unsigned int w)
{
    for (unsigned int i = 0; i < h + w; ++i)
        if (find(i * 2) == find(i * 2 + 1))
            return false;
    return true;
}
bool isSucc(const unsigned int h, const unsigned int w)
{
    unsigned int ret = 0;
    for (unsigned int i = 0; i < h + w; ++i)
    {
        const unsigned int rtn = find(i * 2), rts = find(i * 2 + 1);
        if (vis[rtn] || vis[rts])
            continue;
        vis[rtn] = vis[rts] = true;
        if ((selCnt[rtn] & 0x01) && (selCnt[rts] & 0x01))
            ret ^= 1;
        else if (!(selCnt[rtn] & 0x01) && !(selCnt[rts] & 0x01))
            ret ^= 0;
        else
            ret ^= 2;
    }
    return ret;
}
void solve()
{
    unsigned int h, w;
    cin >> h >> w;
    init((h + w) * 2);
    readInput(h, w);
    if (!isAllP(h, w))
    {
        cout << ((h + w) & 0x01) << "\n";
        return;
    }
    cout << isSucc(h, w) + 2 << "\n";
}
int main()
{
#ifndef APTEST
    ios_base::sync_with_stdio(false);
#endif
    unsigned int t;
    cin >> t;
    for (unsigned int i = 0; i < t; ++i)
        solve();
    return 0;
}
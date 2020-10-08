#ifdef APTEST
#include "debug_tools/program.h"
#endif
#include <algorithm>
#include <climits>
#include <iostream>
using std::cin;
using std::max;
using std::min;
const unsigned int maxn = 3e5;
constexpr int ninf = INT_MIN / 2, inf = INT_MAX / 2;

namespace program
{
    struct Edge
    {
        unsigned int to;
        const Edge* pre;
    } ed[maxn * 2 + 10];
    const Edge* head[maxn + 10];
    bool key[maxn + 10];
    int select[maxn + 10], pend[maxn + 10];

    inline void addEdge(const unsigned int from, const unsigned int to)
    {
        static Edge* cur = ed;
        new (cur) Edge { to, head[from] };
        head[from] = cur++;
        new (cur) Edge { from, head[to] };
        head[to] = cur++;
    }
    unsigned int dfs(const unsigned int x, const unsigned int f, const int val)
    {
        unsigned int ret = 0;
        pend[x] = key[x] ? 0 : ninf;
        select[x] = inf;
        for (const Edge* i = head[x]; i; i = i->pre)
        {
            const unsigned int to = i->to;
            if (to == f)
                continue;
            ret += dfs(to, x, val);
            pend[x] = max(pend[x], pend[to] + 1);
            select[x] = min(select[x], select[to] + 1);
        }
        if (select[x] + pend[x] <= val)
            pend[x] = ninf;
        else if (pend[x] == val)
        {
            pend[x] = ninf;
            select[x] = 0;
            ++ret;
        }
        else if (x == 1 && pend[x] >= 0)
            ++ret;
        return ret;
    }
    unsigned int binarySearch(const unsigned int n, const unsigned int m)
    {
        int l = 0, r = n;
        while (l <= r)
        {
            const unsigned int mid = (l + r) >> 1;
            if (dfs(1, 0, mid) <= m)
                r = mid - 1;
            else
                l = mid + 1;
        }
        return l;
    }
}

int main()
{
#ifndef APTEST
    std::ios_base::sync_with_stdio(false);
#endif
    using namespace program;
    unsigned int n, m;
    cin >> n >> m;
    for (unsigned int i = 1; i <= n; ++i)
    {
        unsigned int d;
        cin >> d;
        key[i] = d;
    }
    for (unsigned int i = 1; i < n; ++i)
    {
        unsigned int a, b;
        cin >> a >> b;
        addEdge(a, b);
    }
    std::cout << binarySearch(n, m) << "\n";
    return 0;
}
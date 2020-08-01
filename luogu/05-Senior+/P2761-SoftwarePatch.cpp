#ifdef APTEST
#include "debug_tools/program.h"
#endif
#include <algorithm>
#include <climits>
#include <iostream>
#include <queue>
using namespace std;
const unsigned int maxn = 20, maxs = (1 << maxn), maxm = 100;

struct patch
{
    unsigned int all, none;
    unsigned int add, remove;
    unsigned int time;
    inline bool test(const unsigned int s) const
    {
        return (s & all) == all && !(s & none) && (s & remove);
    }
    inline unsigned int apply(const unsigned int s) const
    {
        return (s & (~remove)) | add;
    }
} pat[maxm + 1];

unsigned int dijkstra(const unsigned int n, const unsigned int m)
{
    static bool vis[maxs + 1];
    static unsigned int dis[maxs + 1];
    using pair_t = pair<unsigned int, unsigned int>;
    priority_queue<pair_t, vector<pair_t>, greater<pair_t>> q;
    const unsigned int s = (1 << n) - 1;
    fill(dis, dis + s + 1, UINT_MAX);
    dis[s] = 0;
    q.emplace(0, s);
    while (!q.empty())
    {
        const pair_t cur = q.top();
        q.pop();
        if (vis[cur.second])
            continue;
        vis[cur.second] = true;
        for (patch* i = pat; i < pat + m; ++i)
            if (i->test(cur.second))
            {
                const unsigned int ns = i->apply(cur.second);
                if (dis[ns] > cur.first + i->time)
                {
                    dis[ns] = cur.first + i->time;
                    q.emplace(dis[ns], ns);
                }
            }
    }
    return dis[0];
}
int main()
{
#ifndef APTEST
    ios_base::sync_with_stdio(false);
#endif
    unsigned int n, m;
    cin >> n >> m;
    for (patch* i = pat; i < pat + m; ++i)
    {
        static char str[maxn + 1];
        cin >> i->time;
        cin >> str;
        for (unsigned int j = 0; j < n; ++j)
            switch (str[j])
            {
            case '+':
                i->all |= 1 << j;
                break;
            case '-':
                i->none |= 1 << j;
            }
        cin >> str;
        for (unsigned int j = 0; j < n; ++j)
            switch (str[j])
            {
            case '+':
                i->add |= 1 << j;
                break;
            case '-':
                i->remove |= 1 << j;
            }
    }
    const unsigned int t = dijkstra(n, m);
    cout << (t == UINT_MAX ? 0 : t) << endl;
    return 0;
}
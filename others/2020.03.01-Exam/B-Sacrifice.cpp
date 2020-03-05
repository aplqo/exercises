#ifdef APTEST
#include "debug_tools/program.h"
#endif
#include <algorithm>
#include <functional>
#include <iostream>
#include <limits>
#include <queue>
#include <utility>
#include <vector>
using namespace std;
using num_t = unsigned int;
constexpr num_t inf = numeric_limits<num_t>::max();
const int maxn = 1000000, maxm = 10;

unsigned int digit[maxm + 10];
num_t ans[maxn + 10];
bool vis[maxn + 10];

inline void dijkstra(const unsigned int n, const unsigned int m)
{
    using pair_t = pair<num_t, unsigned int>;
    priority_queue<pair_t, vector<pair_t>, greater<pair_t>> q;
    for (unsigned int* i = digit; i < digit + m; ++i)
        if (*i)
        {
            ans[(*i) % n] = *i;
            q.push(make_pair(*i, (*i) % n));
        }
    while (!q.empty())
    {
        pair_t cur = q.top();
        q.pop();
        if (vis[cur.second])
            continue;
        vis[cur.second] = true;
        for (unsigned int* i = digit; i < digit + m; ++i)
        {
            num_t to = (cur.second * 10 + *i) % n;
            if (ans[to] > (cur.first * 10 + *i))
            {
                ans[to] = cur.first * 10 + *i;
                q.push(make_pair(ans[to], to));
            }
        }
    }
}
inline void solve()
{
    unsigned int n, m;
    cin >> n >> m;
    for_each(digit, digit + m, [](unsigned int& i) { cin >> i; });
    fill(ans, ans + n + 1, inf);
    fill(vis, vis + n + 1, false);
    dijkstra(n, m);
    cout << (ans[0] == inf ? 0 : ans[0]) << endl;
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

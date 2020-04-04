/* 
* Luogu team T127823 Generate data
*/
#ifdef APTEST
#include "debug_tools/program.h"
#endif
#include <algorithm>
#include <iostream>
#include <limits>
using namespace std;
using num_t = unsigned long long;
constexpr num_t inf = numeric_limits<num_t>::max();
const int maxn = 20000;

num_t minev[maxn + 10], tmp[maxn + 10];
struct edge
{
    unsigned int to;
    unsigned int from;
    num_t val;
} ed[maxn + 10];

inline void Solve()
{
    unsigned int n;
    cin >> n;
    for (edge* i = ed; i < ed + n - 1; ++i)
        cin >> i->from >> i->to >> i->val;

    fill(minev, minev + n + 1, inf);
    for (edge* i = ed; i < ed + n - 1; ++i)
    {
        minev[i->from] = min(minev[i->from], i->val + 1);
        minev[i->to] = min(minev[i->to], i->val + 1);
    }

    num_t ans = 0;
    copy(minev, minev + n + 1, tmp);
    sort(tmp + 1, tmp + 1 + n);
    for (unsigned int i = 1; i <= n; ++i)
        ans += (i - 1) * tmp[i];

    for (edge* i = ed; i < ed + n - 1; ++i)
    {
        ans -= max(minev[i->from], minev[i->to]);
        ans += i->val;
    }

    cout << ans << endl;
}

int main()
{
#ifndef APTEST
    ios_base::sync_with_stdio(false);
#endif
    unsigned int t;
    cin >> t;
    for (unsigned int i = 0; i < t; ++i)
        Solve();
    return 0;
}
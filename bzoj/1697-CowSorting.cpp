#ifdef APTEST
#include "debug_tools/program.h"
#endif
#include <algorithm>
#include <climits>
#include <iostream>
using namespace std;
using num = unsigned long long;
const int maxn = 1e5;

unsigned int a[maxn + 10], to[maxn + 10];
bool vis[maxn + 10];
struct
{
    unsigned int cnt;
    unsigned int low = UINT_MAX;
    num sum;
} ring[maxn + 10];
num sum[maxn + 10];

void create(const unsigned int n)
{
    static unsigned int tmp[maxn + 10];
    copy(a, a + n, tmp);
    sort(tmp, tmp + n);
    for (unsigned int i = 0; i < n; ++i)
        to[i] = lower_bound(tmp, tmp + n, a[i]) - tmp;
}
unsigned int find(const unsigned int n)
{
    auto* r = ring;
    for (unsigned int i = 0; i < n; ++i)
    {
        if (vis[i])
            continue;
        unsigned int p = i;
        while (!vis[p])
        {
            r->sum += a[p];
            r->low = min(r->low, a[p]);
            ++(r->cnt);
            vis[p] = true;
            p = to[p];
        }
        ++r;
    }
    return r - ring;
}
num solve(const unsigned int n, const unsigned int cntr)
{
    num ans = 0;
    const unsigned int mv = *min_element(a, a + n);
    for (auto* i = ring; i < ring + cntr; ++i)
    {
        if (i->cnt == 1)
            continue;
        ans += min((i->cnt - num(2)) * i->low, (i->cnt + num(1)) * mv + i->low) + i->sum;
    }
    return ans;
}
int main()
{
#ifndef APTEST
    ios_base::sync_with_stdio(false);
#endif
    unsigned int n;
    cin >> n;
    for_each(a, a + n, [](unsigned int& i) -> void { cin >> i; });
    create(n);
    cout << solve(n, find(n)) << endl;
    return 0;
}

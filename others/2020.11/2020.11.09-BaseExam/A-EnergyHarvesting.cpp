#ifdef APTEST
#include "debug_tools/program.h"
#endif
#include <algorithm>
#include <iostream>
#include <iterator>
#include <queue>
using namespace std;
const unsigned int maxn = 1e5;

long long d[maxn + 10];

pair<bool, unsigned long long> greedy(const unsigned int n, const unsigned int x, const int y, const unsigned long long e, long long h)
{
    priority_queue<long long> q;
    unsigned long long acc = 0, mx = 0;
    for (unsigned int i = 1; i <= n; ++i)
    {
        acc += x;
        if (acc >= e)
            return make_pair(true, i);
        q.push(max<long long>(d[i], y));
        h -= d[i];
        mx = max(mx, acc);
        while (h <= 0 && !q.empty())
        {
            acc -= x;
            h += q.top();
            q.pop();
        }
        if (h <= 0)
            return make_pair(false, mx);
    }
    return make_pair(false, mx);
}
int main()
{
#ifndef APTEST
    ios_base::sync_with_stdio(false);
#endif
    unsigned int n, x, y, h, e;
    cin >> n >> x >> y >> h >> e;
    copy_n(istream_iterator<unsigned int>(cin), n, d + 1);
    const auto [suc, val] = greedy(n, x, y, e, h);
    if (suc)
        cout << "YES\n"
             << val << "\n";
    else
        cout << "NO\n"
             << val << "\n";
    return 0;
}
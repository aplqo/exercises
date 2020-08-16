#ifdef APTEST
#include "debug_tools/program.h"
#endif
#include <algorithm>
#include <iostream>
using namespace std;
const unsigned int maxn = 1e6, maxm = 1e3;

struct Edge
{
    unsigned int from, val;
    const Edge* pre;
} ed[maxm + 1];
const Edge* head[maxn + 1];

void addEdge(const unsigned int from, const unsigned int to, const unsigned int val)
{
    static Edge* cur = ed;
    cur->from = from;
    cur->val = val;
    cur->pre = head[to];
    head[to] = cur++;
}
unsigned long long dp(const unsigned int n, const unsigned int r)
{
    static unsigned long long f[maxn + 1];
    for (unsigned int i = 1; i <= n; ++i)
    {
        f[i] = max(f[i], f[i - 1]);
        for (const Edge* j = head[i]; j; j = j->pre)
            f[i] = max(f[i], f[j->from >= r ? j->from - r : 0] + j->val);
    }
    return *max_element(f, f + n + 1);
}
int main()
{
#ifndef APTEST
    ios_base::sync_with_stdio(false);
#endif
    unsigned int n, m, r;
    cin >> n >> m >> r;
    for (unsigned int i = 0; i < m; ++i)
    {
        unsigned int s, e, f;
        cin >> s >> e >> f;
        addEdge(s, e, f);
    }
    cout << dp(n, r) << endl;
    return 0;
}
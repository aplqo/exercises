#ifdef APTEST
#include "debug_tools/program.h"
#endif
#include <algorithm>
#include <iostream>
#include <vector>
using namespace std;
const unsigned int maxn = 1e5;

vector<unsigned int> rang[maxn + 1];

void dfsRead(const unsigned int l, const unsigned int r)
{
    if (r > l)
    {
        unsigned int m;
        cin >> m;
        dfsRead(l, m);
        dfsRead(m + 1, r);
    }
    rang[l].push_back(r);
}
unsigned int solve(const unsigned int l, const unsigned int r)
{
    unsigned int p = l, ret = 0;
    while (p <= r)
    {
        ++ret;
        p = *(upper_bound(rang[p].begin(), rang[p].end(), r) - 1) + 1;
    }
    return ret;
}

int main()
{
#ifndef APTEST
    ios_base::sync_with_stdio(false);
#endif
    unsigned int n, m;
    cin >> n >> m;
    dfsRead(1, n);
    for (unsigned int i = 0; i < m; ++i)
    {
        unsigned int l, r;
        cin >> l >> r;
        cout << solve(l, r) << endl;
    }
    return 0;
}
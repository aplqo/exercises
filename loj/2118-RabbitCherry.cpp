#ifdef APTEST
#include "debug_tools/program.h"
#endif
#include <algorithm>
#include <iostream>
#include <iterator>
#include <vector>
using namespace std;
const unsigned int maxn = 2e6;

vector<unsigned int> chl[maxn + 1];
unsigned int w[maxn + 1], cost[maxn + 1];
unsigned int m;

unsigned int dfs(const unsigned int x)
{
    unsigned int ret = 0;
    for (unsigned int i : chl[x])
        ret += dfs(i);
    cost[x] = w[x] + chl[x].size();
    sort(chl[x].begin(), chl[x].end(), [](unsigned int a, unsigned int b) { return cost[a] < cost[b]; });
    for (auto it = chl[x].cbegin(); it < chl[x].cend(); ++it)
        if (cost[x] + cost[*it] - 1 <= m)
        {
            cost[x] = cost[x] + cost[*it] - 1;
            ++ret;
        }
        else
            break;
    return ret;
}
int main()
{
#ifndef APTEST
    ios_base::sync_with_stdio(false);
#endif
    unsigned int n;
    cin >> n >> m;
    copy_n(istream_iterator<unsigned int>(cin), n, w);
    for (unsigned int i = 0; i < n; ++i)
    {
        unsigned int k;
        cin >> k;
        if (!k)
            continue;
        chl[i].reserve(k);
        copy_n(istream_iterator<unsigned int>(cin), k, back_inserter(chl[i]));
    }
    cout << dfs(0) << endl;
    return 0;
}
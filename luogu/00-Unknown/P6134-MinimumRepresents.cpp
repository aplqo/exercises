#ifdef APTEST
#include "debug_tools/program.h"
#endif
#include <algorithm>
#include <bitset>
#include <iostream>
#include <queue>
#include <vector>
using namespace std;
const unsigned int maxn = 3e4, maxm = 1e5;
typedef bitset<maxn + 10> Bitset;

vector<unsigned int> gra[maxn + 10];
unsigned int seq[maxn + 10];
unsigned int degree[maxn + 10], dist[maxn + 10];
Bitset connects[maxn + 10];

inline void addEdge(const unsigned int from, const unsigned int to)
{
    gra[from].emplace_back(to);
    ++degree[to];
}
void topological(const unsigned int n)
{
    queue<unsigned int> q;
    for (unsigned int i = 1; i <= n; ++i)
        if (!degree[i])
            q.push(i);
    unsigned int* pos = seq;
    while (!q.empty())
    {
        const unsigned int cur = q.front();
        q.pop();
        *(pos++) = cur;
        for (unsigned int to : gra[cur])
        {
            --degree[to];
            if (!degree[to])
                q.push(to);
        }
    }
}
static unsigned int count(const unsigned int n)
{
    unsigned int ret = 0;
    for (unsigned int* i = seq + n; i >= seq; --i)
    {
        const unsigned int cur = *i;
        connects[cur].set(cur);
        for (unsigned int to : gra[cur])
            dist[cur] = max(dist[cur], dist[to] + 1);
        sort(gra[cur].begin(), gra[cur].end(), [](unsigned int a, unsigned int b) { return dist[a] > dist[b]; });
        for (unsigned int to : gra[cur])
        {
            const Bitset&& bs = connects[to] | connects[cur];
            if (bs == connects[cur])
                ++ret;
            else
                connects[cur] = bs;
        }
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
    for (unsigned int i = 0; i < m; ++i)
    {
        unsigned int u, v;
        cin >> u >> v;
        addEdge(u, v);
    }
    topological(n);
    cout << count(n) << "\n";
    return 0;
}
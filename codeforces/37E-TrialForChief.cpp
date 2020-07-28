#ifdef APTEST
#include "debug_tools/program.h"
#endif
#include <algorithm>
#include <climits>
#include <deque>
#include <iostream>
using namespace std;
const unsigned int maxn = 50;
const unsigned int maxv = (maxn + 2) * (maxn + 2), maxe = maxv * 4;

struct edge
{
    unsigned int to, val;
    const edge* pre;
} ed[maxe + 1];
const edge* head[maxv + 1];
bool tab[maxn + 10][maxn + 10], vis[maxv + 10];
unsigned int dis[maxv + 10];

void addEdge(const unsigned int from, const unsigned int to, const unsigned int val)
{
    static edge* cur = ed;
    cur->to = to;
    cur->val = val;
    cur->pre = head[from];
    head[from] = cur;
    ++cur;
}
static void init(const int n, const int m)
{
    const int dx[] = { -1, 1, 0, 0 }, dy[] = { 0, 0, -1, 1 };
    const int p = m + 2;
    for (int i = 1; i <= n; ++i)
        for (int j = 1; j <= m; ++j)
            for (unsigned int k = 0; k < 4; ++k)
            {
                const unsigned int tx = i + dx[k], ty = j + dy[k];
                addEdge(i * p + j, tx * p + ty, tab[i][j] != tab[tx][ty]);
            }
}
void bfs(const unsigned int x, const unsigned int n)
{
    deque<unsigned int> q;
    fill(dis, dis + 1 + n, UINT_MAX);
    fill(vis, vis + 1 + n, false);
    dis[x] = 0;
    q.push_back(x);
    while (!q.empty())
    {
        const unsigned int cur = q.front();
        q.pop_front();
        for (const edge* i = head[cur]; i; i = i->pre)
            if (dis[cur] + i->val < dis[i->to])
            {
                const unsigned int to = i->to;
                dis[to] = dis[cur] + i->val;
                if (!vis[to])
                {
                    vis[to] = true;
                    if (i->val)
                        q.push_back(to);
                    else
                        q.push_front(to);
                }
            }
    }
}
unsigned int findMax(const unsigned int n, const unsigned int m)
{
    const unsigned int p = m + 2;
    unsigned int ret = 0;
    for (unsigned int i = 1; i <= n; ++i)
        for (unsigned int j = 1; j <= m; ++j)
            if (tab[i][j])
                ret = max(ret, dis[i * p + j]);
    return ret;
}
unsigned int solve(const unsigned int n, const unsigned int m)
{
    const unsigned int sz = (n + 2) * (m + 2), p = m + 2;
    unsigned int ret = UINT_MAX;
    for (unsigned int i = 1; i <= n; ++i)
        for (unsigned int j = 1; j <= m; ++j)
        {
            bfs(i * p + j, sz);
            ret = min(ret, findMax(n, m));
        }
    return ret + 1;
}

int main()
{
#ifndef APTEST
    ios_base::sync_with_stdio(false);
#endif
    unsigned int n, m;
    cin >> n >> m;
    bool allWhite = true;
    for (unsigned int i = 1; i <= n; ++i)
    {
        static char str[maxn + 10];
        cin >> (str + 1);
        for (unsigned int j = 1; j <= m; ++j)
        {
            tab[i][j] = str[j] == 'B';
            allWhite &= str[j] == 'W';
        }
    }
    if (allWhite)
    {
        cout << "0" << endl;
        return 0;
    }
    init(n, m);
    cout << solve(n, m) << endl;
    return 0;
}
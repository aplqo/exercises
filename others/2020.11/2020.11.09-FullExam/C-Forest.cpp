#ifdef APTEST
#include "debug_tools/program.h"
#endif
#include <algorithm>
#include <climits>
#include <functional>
#include <iostream>
#include <queue>
using namespace std;
const unsigned int maxn = 2000, maxm = 2000;
const int dx[] = { 1, 1, 1, -1, -1, -1, 0, 0 }, dy[] = { 1, -1, 0, 1, -1, 0, 1, -1 };

struct State
{
    int x, y;

    template <class T>
    inline T& operator[](T dat[maxn + 10][maxm + 10]) const { return dat[x][y]; }
};
enum class Visit
{
    Not = 0,
    Visited,
    Mark
};
bool isForest[maxn + 10][maxm + 10];
Visit vis[maxn + 10][maxm + 10];

unsigned int bfs(const int n, const int m, const int sx, const int sy)
{
    static unsigned int dist[maxn + 10][maxm + 10];
    enum Update
    {
        Not = 0,
        Up = 0b01,
        Down = 0b10
    };
    static unsigned int upd[maxn + 10][maxm + 10];
    queue<State> q;
    q.push(State { sx, sy });
    if (vis[sx][sy] != Visit::Mark)
        q.front()[vis] = Visit::Visited;
    unsigned int ret = UINT_MAX;
    while (!q.empty())
    {
        const State cur = q.front();
        q.pop();
        if (cur[vis] == Visit::Mark)
        {
            if (cur[upd] == (Update::Up | Update::Down))
                ret = min(ret, cur[dist]);
            continue;
        }
        for (unsigned int i = 0; i < 8; ++i)
        {
            State ns { cur.x + dx[i], cur.y + dy[i] };
            const Update typ = cur.x < ns.x ? Update::Down : Update::Up;
            if (ns.x < 1 || ns.y < 1 || ns.x > n || ns.y > m || ns[isForest]
                || ns[vis] == Visit::Visited
                || (ns[vis] == Visit::Mark && (ns[upd] & typ)))
                continue;
            ns[dist] += cur[dist] + 1;
            if (ns[vis] != Visit::Mark)
                ns[vis] = Visit::Visited;
            else
                ns[upd] |= typ;
            q.push(ns);
        }
    }
    return ret;
}
pair<unsigned int, unsigned int> readMatrix(const unsigned int n, const unsigned int m)
{
    static char buf[maxn + 10];
    unsigned int sx, sy;
    for (unsigned int i = 1; i <= n; ++i)
    {
        cin >> (buf + 1);
        for (unsigned int j = 1; j <= m; ++j)
            switch (buf[j])
            {
            case 'X':
                isForest[i][j] = true;
                break;
            case '*':
                sx = i;
                sy = j;
                break;
            }
    }
    return make_pair(sx, sy);
}
unsigned int findMin(const unsigned int n, const unsigned int m, const unsigned int sx, const unsigned int sy)
{
    for (unsigned int i = 1; i <= n; ++i)
    {
        const unsigned int pos = find(isForest[i] + 1, isForest[i] + 1 + m, true) - isForest[i];
        if (pos == 1 + m)
            continue;
        fill(vis[i] + 1, vis[i] + pos, Visit::Mark);
        return bfs(n, m, sx, sy);
    }
    return 0;
}

int main()
{
#ifndef APTEST
    ios_base::sync_with_stdio(false);
#endif
    unsigned int n, m;
    cin >> n >> m;
    const auto [sx, sy] = readMatrix(n, m);
    cout << findMin(n, m, sx, sy) << "\n";
    return 0;
}
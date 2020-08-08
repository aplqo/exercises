#ifdef APTEST
#include "debug_tools/program.h"
#endif
#include <algorithm>
#include <climits>
#include <fstream>
#include <iostream>
#include <queue>
using namespace std;
const unsigned int maxn = 1000, maxm = 2000, maxt = maxn * 2 + 1;

struct Edge
{
    unsigned int to;
    long long val;
    const Edge* pre;
} ed[maxm + 1];
const Edge* head[maxn + 1];
unsigned int mv[maxn + 1];
long long dis[maxn + 1][maxt + 5];

void addEdge(const unsigned int from, const unsigned int to)
{
    static Edge* cur = ed;
    cur->to = to;
    cur->val = mv[to];
    cur->pre = head[from];
    head[from] = cur;
    ++cur;
}
long long spfa(const unsigned int n, const unsigned int mt, const unsigned int c)
{
    static bool vis[maxn + 1][maxt + 5];
    queue<unsigned int> q;
    q.push(0);
    for (unsigned int i = 0; i < n; ++i)
        for (unsigned int j = 0; j <= mt; ++j)
            dis[i][j] = LLONG_MIN / 2;
    dis[0][0] = 0;
    while (!q.empty())
    {
        const unsigned int pos = q.front() % n, lay = q.front() / n;
        q.pop();
        vis[pos][lay] = false;
        if (lay == mt)
            continue;
        for (const Edge* i = head[pos]; i; i = i->pre)
        {
            const unsigned int to = i->to + (lay + 1) * n;
            const long long dt = dis[pos][lay] + i->val - (2 * c * lay + c);
            if (dis[i->to][lay + 1] < dt)
            {
                dis[i->to][lay + 1] = dt;
                if (!vis[i->to][lay + 1])
                {
                    vis[i->to][lay + 1] = true;
                    q.push(to);
                }
            }
        }
    }
    long long ret = 0;
    for (unsigned int i = 0; i <= mt; ++i)
        ret = max(ret, dis[0][i]);
    return ret;
}

int main()
{
#if !defined APTEST && !defined APDEBUG
    ios_base::sync_with_stdio(false);
    ifstream in("time.in");
    ofstream out("time.out");
#else
    istream& in = cin;
    ostream& out = cout;
#endif
    unsigned int n, m, c;
    in >> n >> m >> c;
    for (unsigned int i = 0; i < n; ++i)
        in >> mv[i];
    for (unsigned int i = 0; i < m; ++i)
    {
        unsigned int a, b;
        in >> a >> b;
        addEdge(a - 1, b - 1);
    }
    out << spfa(n, maxt, c) << endl;
    return 0;
}

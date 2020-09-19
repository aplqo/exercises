#ifdef APTEST
#include "debug_tools/program.h"
#endif
#include <climits>
#include <cstring>
#include <functional>
#include <iostream>
#include <queue>
using namespace std;
const unsigned int maxn = 1000, maxm = 1000;
constexpr unsigned long long inf = ULLONG_MAX / 2;
template <class T>
using Grid = T[maxn + 10][maxm + 10];

struct Point
{
    template <class T>
    inline T& operator[](Grid<T> dat) const { return dat[x][y]; }
    template <class T>
    inline T operator[](const Grid<T> dat) const { return dat[x][y]; }
    inline bool operator<(const Point& r) const { return x < r.x; }

    unsigned int x, y;
};
constexpr int dx[] = { 0, 0, 1, -1 }, dy[] = { 1, -1, 0, 0 };

Grid<unsigned int> val;
Grid<unsigned long long> disSource, disB, disC;
unsigned int n, m;

void dijkstra(const Point src, Grid<unsigned long long> res)
{
    using Pair = pair<unsigned long long, Point>;
    static Grid<bool> vis;
    priority_queue<Pair, vector<Pair>, greater<Pair>> q;
    fill(res[0], res[0] + sizeof(Grid<unsigned long long>) / sizeof(unsigned long long), inf);
    memset(vis, 0, sizeof(vis));
    src[res] = src[val];
    q.emplace(src[res], src);
    while (!q.empty())
    {
        const Pair cur = q.top();
        q.pop();
        if (cur.second[vis])
            continue;
        cur.second[vis] = true;
        for (unsigned int i = 0; i < 4; ++i)
        {
            const unsigned int nx = int(cur.second.x) + dx[i], ny = int(cur.second.y) + dy[i];
            if (nx < 1 || ny < 1 || nx > n || ny > m)
                continue;
            const Point np { nx, ny };
            if (np[res] > cur.first + np[val])
            {
                np[res] = cur.first + np[val];
                q.emplace(np[res], np);
            }
        }
    }
}
unsigned long long findPath(const unsigned int n, const unsigned int m)
{
    Point p { 1, 1 };
    unsigned long long ret = inf;
    for (; p.x <= n; ++p.x)
        for (p.y = 1; p.y <= m; ++p.y)
            ret = min(ret, p[disSource] + p[disB] + p[disC] - 2 * p[val]);
    return ret;
}
int main()
{
#ifndef APTEST
    ios_base::sync_with_stdio(false);
#endif
    unsigned int a, b, c;
    cin >> n >> m >> a >> b >> c;
    for (unsigned int i = n; i > 0; --i)
        for (unsigned int j = 1; j <= m; ++j)
            cin >> val[i][j];
    dijkstra(Point { n, a }, disSource);
    dijkstra(Point { 1, b }, disB);
    dijkstra(Point { 1, c }, disC);
    cout << findPath(n, m) << "\n";
    return 0;
}
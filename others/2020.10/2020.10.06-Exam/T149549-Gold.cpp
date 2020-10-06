#ifdef APTEST
#include "debug_tools/program.h"
#endif
#include <algorithm>
#include <iostream>
#include <iterator>
#include <queue>
#include <vector>
using namespace std;
const unsigned int maxn = 6000, maxb = 10000;
constexpr int dx[] = { 0, 0, 1, -1 }, dy[] = { 1, -1, 0, 0 };

struct Point
{
    int x, y;
};
unsigned int val[maxn + 10][maxn + 10];

unsigned int bfs(const Point p, const int n, const int m)
{
    unsigned int ret = 0;
    queue<Point> q;
    q.push(p);
    while (!q.empty())
    {
        const Point cur = q.front();
        q.pop();
        for (unsigned int i = 0; i < 4; ++i)
        {
            const int nx = cur.x + dx[i], ny = cur.y + dy[i];
            if (nx < 1 || ny < 1 || nx > n || ny > m || !val[nx][ny])
                continue;
            ret += val[nx][ny];
            val[nx][ny] = 0;
            q.push(Point { nx, ny });
        }
    }
    return ret;
}
vector<unsigned int> findBlock(const unsigned int n, const unsigned int m)
{
    vector<unsigned int> blk;
    for (unsigned int i = 1; i <= n; ++i)
        for (unsigned int j = 1; j <= m; ++j)
            if (val[i][j])
            {
                blk.push_back(val[i][j]);
                val[i][j] = 0;
                blk.back() += bfs(Point { i, j }, n, m);
            }
    sort(blk.begin(), blk.end(), greater<unsigned int>());
    return blk;
}

int main()
{
#ifndef APTEST
    ios_base::sync_with_stdio(false);
#endif
    unsigned int n, m;
    cin >> m >> n;
    for (unsigned int i = 1; i <= n; ++i)
        for (unsigned int j = 1; j <= m; ++j)
        {
            char ch;
            cin >> ch;
            val[i][j] = ch - '0';
        }
    const vector<unsigned int> blk = findBlock(n, m);
    cout << blk.size() << "\n";
    copy(blk.cbegin(), blk.cend(), ostream_iterator<unsigned int>(cout, " "));
    cout.put('\n');
    return 0;
}
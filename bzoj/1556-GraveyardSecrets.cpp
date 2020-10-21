#ifdef APTEST
#include "debug_tools/program.h"
#endif
#include <algorithm>
#include <bitset>
#include <climits>
#include <deque>
#include <iostream>
#include <utility>
#include <vector>
using namespace std;
const unsigned int maxn = 100, maxm = 100, maxt = 15, maxs = 1 << maxt, maxv = maxt * 4;
constexpr unsigned int inf = UINT_MAX / 2;

struct Step
{
    int dx, dy;
};
constexpr Step nxt[] = { { 0, 1 }, { 0, -1 }, { 1, 0 }, { -1, 0 } };
constexpr unsigned int rev[] = { 1, 0, 3, 2 };
struct Point
{
    int x, y;
    inline Point operator+(const Step p) const { return Point { x + p.dx, y + p.dy }; }
    template <class T>
    inline T& operator[](T dat[maxn + 1][maxm + 1]) const { return dat[x][y]; }
};
struct Position : public Point
{
    unsigned int dir;

    using Point::operator[];
    template <class T>
    inline T& operator[](T dat[maxn + 1][maxm + 1][4]) const { return dat[x][y][dir]; }
};
bool isWall[maxn + 1][maxm + 1];
vector<Position> keyPoint;
unsigned int id[maxn + 1][maxm + 1][4], key[maxv + 1];
unsigned int dis[maxv][maxv], pdis[maxn + 1][maxm + 1][4], iniDis[4][maxv];
int n, m;

template <unsigned int v>
inline void update(deque<Position>& q, const Position np, const unsigned int cdis)
{
    if (np[pdis] > cdis)
    {
        np[pdis] = cdis;
        if (v)
            q.emplace_back(np);
        else
            q.emplace_front(np);
    }
}
#define Fill(a, t, v) fill(reinterpret_cast<t*>(a), reinterpret_cast<t*>(a) + sizeof(a) / sizeof(t), v)
void bfs(const Position ip)
{
    static unsigned int vis[maxn + 1][maxm + 1][4], sta = 0;
    deque<Position> q;
    Fill(pdis, unsigned int, inf);
    q.emplace_front(ip);
    ip[pdis] = 0;
    ++sta;
    while (!q.empty())
    {
        const Position cur = q.front();
        q.pop_front();
        if (cur[vis] == sta)
            continue;
        cur[vis] = sta;
        const unsigned int cdis = cur[pdis];
        {
            const Position np { cur + nxt[cur.dir], cur.dir };
            if (1 <= np.x && np.x <= n && 1 <= np.y && np.y <= m && !np[isWall])
                update<0>(q, np, cdis);
        }
        for (unsigned int i = 0; i < 4; ++i)
            update<1>(q, Position { Point { cur }, i }, cdis + 1);
    }
}
static void initDistance(const Point ini)
{
    static const auto copyDis = [](unsigned int* const res) {
        for (const Position ip : keyPoint)
            res[ip[id]] = ip[pdis];
    };
    for (const Position ip : keyPoint)
    {
        bfs(ip);
        copyDis(dis[ip[id]]);
    }
    Position p { ini };
    for (p.dir = 0; p.dir < 4; ++p.dir)
    {
        bfs(p);
        copyDis(iniDis[p.dir]);
    }
}

unsigned int dp(const unsigned int kcnt, const unsigned int t, const unsigned int iniDis[])
{
    static unsigned int f[maxs + 1][maxv];
    Fill(f, unsigned int, inf);
    for (unsigned int i = 0; i < kcnt; ++i)
        f[key[i]][i] = iniDis[i] + 1;
    const unsigned int msk = (1 << t) - 1;
    for (unsigned int i = 0; i <= msk; ++i)
        for (unsigned int p = 0; p < kcnt; ++p)
        {
            if ((i & key[p]) != key[p] || f[i][p] == inf)
                continue;
            for (unsigned int k = 0; k < kcnt; ++k)
                f[i | key[k]][k] = min(f[i | key[k]][k], f[i][p] + dis[p][k]);
        }
    return *min_element(f[msk], f[msk] + kcnt);
}
inline unsigned int dp(const unsigned int kcnt, const unsigned int t)
{
    unsigned int ret = inf;
    for (unsigned int i = 0; i < 4; ++i)
        ret = min(ret, dp(kcnt, t, iniDis[i]));
    return ret;
}
static unsigned int readKey(const int n, const int m, const unsigned int t)
{
    for (unsigned int i = 0; i < t; ++i)
    {
        int x, y;
        cin >> x >> y;
        const unsigned int kid = 1 << i;
        for (const Step* i = nxt; i < nxt + 4; ++i)
        {
            const Position p { { x + i->dx, y + i->dy }, rev[i - nxt] };
            if (p.x < 1 || p.y < 1 || p.x > n || p.y > m || p[isWall])
                continue;
            const unsigned int pid = keyPoint.size();
            p[id] = pid;
            key[pid] = kid;
            keyPoint.push_back(p);
        }
    }
    return keyPoint.size();
}

int main()
{
#ifndef APTEST
    ios_base::sync_with_stdio(false);
#endif
    unsigned int t;
    cin >> n >> m >> t;
    for (int i = 1; i <= n; ++i)
    {
        static char str[maxm + 10];
        cin >> str;
        for (int j = 0; j < m; ++j)
            if (str[j] == '#')
                isWall[i][j + 1] = true;
    }
    keyPoint.reserve(n * m);
    const unsigned int kcnt = readKey(n, m, t);
    int x, y;
    cin >> x >> y;
    initDistance(Point { x, y });
    cout << dp(kcnt, t) << "\n";
    return 0;
}
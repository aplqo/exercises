#ifdef APTEST
#include "debug_tools/program.h"
#endif
#include <algorithm>
#include <cstring>
#include <iostream>
#include <queue>
using namespace std;
const unsigned int maxn = 9;
const int dx[] = { 0, 0, 1, -1 }, dy[] = { 1, -1, 0, 0 };
template <class T>
using Matrix = T[maxn + 10][maxn + 10];

struct Point
{
    int x, y;

    template <class T>
    inline T& operator[](Matrix<T> dat) const { return dat[x][y]; }
    template <class T>
    inline T operator[](const Matrix<T> dat) const { return dat[x][y]; }
};
struct Block
{
    Point poi[maxn + 10];
    unsigned int val, siz = 0;

    inline void push(const Point p) { poi[siz++] = p; }
} blk[maxn * maxn + 10], *epos = blk;
Matrix<unsigned int> mat, ans, orig;
unsigned int cnt;
bool unavl[2][maxn + 10][10]; // unavaliable numbers row, column
unsigned int n;

inline bool check(const Point i, const unsigned int v)
{
    return !unavl[0][i.x][v] && !unavl[1][i.y][v];
}
template <bool typ>
inline void assign(const Point i, const unsigned int v)
{
    unavl[0][i.x][v] = unavl[1][i.y][v] = typ;
    if constexpr (typ)
        i[mat] = v;
}
bool lexLess(const Matrix<unsigned int> a, const Matrix<unsigned int> b)
{
    for (unsigned int i = 1; i <= n; ++i)
        for (unsigned int j = 1; j <= n; ++j)
            if (a[i][j] != b[i][j])
                return a[i][j] < b[i][j];
    return false;
}

void dfs(const Block*);

static void dfsBlock(const Block* blk, const unsigned int dep, const unsigned int val)
{
    if (dep == blk->siz - 1)
    {
        if (val <= n && check(blk->poi[dep], val))
        {
            assign<true>(blk->poi[dep], val);
            dfs(blk + 1);
            assign<false>(blk->poi[dep], val);
        }
        return;
    }
    for (unsigned int i = 1; i <= n && i <= val; ++i)
    {
        if (val % i || !check(blk->poi[dep], i))
            continue;
        assign<true>(blk->poi[dep], i);
        dfsBlock(blk, dep + 1, val / i);
        assign<false>(blk->poi[dep], i);
    }
}
void dfs(const Block* blk)
{
    if (blk == epos)
    {
        if (!cnt || lexLess(mat, ans))
            memcpy(ans, mat, sizeof(mat));
        ++cnt;
        return;
    }
    if (blk->siz == 1)
        dfs(blk + 1);
    else
        dfsBlock(blk, 0, blk->val);
}
Block* bfs(const Point x)
{
    queue<Point> q;
    q.push(x);
    const unsigned int nv = x[orig];
    x[orig] = 0;
    epos->val = nv;
    while (!q.empty())
    {
        const Point p = q.front();
        q.pop();
        epos->push(p);
        for (unsigned int i = 0; i < 4; ++i)
        {
            const Point np { p.x + dx[i], p.y + dy[i] };
            if (np[orig] != nv)
                continue;
            np[orig] = 0;
            q.push(np);
        }
    }
    return epos++;
}
void initBlock(const unsigned int n)
{
    for (unsigned int i = 1; i <= n; ++i)
        for (unsigned int j = 1; j <= n; ++j)
            if (orig[i][j])
            {
                const Block* b = bfs(Point { i, j });
                if (b->siz == 1)
                    assign<true>(b->poi[0], b->val);
            }
    sort(blk, epos, [](Block& a, Block& b) { return a.siz < b.siz; });
}
int main()
{
#ifndef APTEST
    ios_base::sync_with_stdio(false);
#endif
    cin >> n;
    for (unsigned int i = 1; i <= n; ++i)
        for (unsigned int j = 1; j <= n; ++j)
            cin >> orig[i][j];
    initBlock(n);
    dfs(blk);
    cout << cnt << "\n";
    for (unsigned int i = 1; i <= n; ++i)
    {
        for (unsigned int j = 1; j <= n; ++j)
            cout << ans[i][j] << " ";
        cout.put('\n');
    }
    return 0;
}
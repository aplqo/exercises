/*Luogu team T134371: Fence*/
#ifdef APTEST
#include "debug_tools/program.h"
#endif
#include <algorithm>
#include <climits>
#include <iostream>
#include <utility>
using namespace std;
const unsigned int maxn = 100, maxs = 8;
constexpr unsigned int inf = UINT_MAX >> 2;

enum typ
{
    Left = 0,
    Right = 1,
    Unknown = 0xff
};
enum dir
{
    In = 0 << 1,
    Out = 1 << 1
};
unsigned int f[maxn + 1][maxn + 1];
pair<typ, typ> mat[maxn + 10][maxn + 10];

void init(const unsigned int n)
{
    for (unsigned int i = 0; i < n; ++i)
        for (unsigned int j = 0; j < n; ++j)
            mat[i][j] = make_pair(Unknown, Unknown);
    const unsigned int mp = n * 4;
    for (unsigned int i = 0; i < mp; ++i)
        for (unsigned int j = 0; j < mp; ++j)
            f[i][j] = inf;
    for (unsigned int i = 0; i < mp; ++i)
        f[i][i] = 0;
}
unsigned int toPoint(unsigned int p0, typ lr, dir io)
{
    return p0 * 4 + lr + io;
}
using ed = pair<unsigned int, typ>;
void addEdge(ed l, ed r)
{
    const static auto upd = [](typ& v, typ t) {if(t!=Unknown)v=t; };
    const static auto poi = [](const ed p, dir io) { return toPoint(p.first, p.second, io); };
    if (l.first > r.first)
        swap(l, r);
    auto& e = mat[l.first][r.first];
    upd(e.first, l.second);
    upd(e.second, r.second);
    if (e.first != Unknown && e.second != Unknown)
    {
        f[poi(l, Out)][poi(r, In)] = 0;
        f[poi(r, Out)][poi(l, In)] = 0;
    }
}
void readEdge(const unsigned int n)
{
    const auto read = []() -> unsigned int { unsigned int t; cin>>t; return t-1; };
    for (unsigned int i = 0; i < n; ++i)
    {
        unsigned int s = read(), l, n1, n2;
        cin >> l >> n1 >> n2;
        f[toPoint(s, Left, In)][toPoint(s, Left, Out)] = l;
        f[toPoint(s, Right, In)][toPoint(s, Right, Out)] = l;
        for (unsigned int j = 0; j < n1; ++j)
            addEdge(make_pair(s, Left), make_pair(read(), Unknown));
        for (unsigned int j = 0; j < n2; ++j)
            addEdge(make_pair(s, Right), make_pair(read(), Unknown));
    }
}
void floyd(const unsigned int n)
{
    const unsigned int mp = n * 4;
    for (unsigned int k = 0; k < mp; ++k)
        for (unsigned int i = 0; i < mp; ++i)
            for (unsigned int j = 0; j < mp; ++j)
                f[i][j] = min(f[i][j], f[i][k] + f[k][j]);
}
unsigned int solve(const unsigned int n)
{
    const unsigned int mp = n * 4;
    unsigned int ret = inf;
    for (unsigned int i = 0; i < mp; ++i)
        for (unsigned int j = 0; j < mp; ++j)
            if (j != i)
                ret = min(ret, f[i][j] + f[j][i]);
    return ret;
}
int main()
{
#ifndef APTEST
    ios_base::sync_with_stdio(false);
#endif
    unsigned int n;
    cin >> n;
    init(n);
    readEdge(n);
    floyd(n);
    cout << solve(n) << endl;
    return 0;
}
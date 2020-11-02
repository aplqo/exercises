#ifdef APTEST
#include "debug_tools/program.h"
#endif
#include <algorithm>
#include <iostream>
#include <numeric>
#include <vector>
using namespace std;
const unsigned int maxn = 1000;
template <class T>
using Matrix = vector<vector<T>>;

void build(std::vector<std::vector<int>> b);
Matrix<int> mat, ed;
namespace Disjoint
{
    unsigned int fa[maxn + 10], siz[maxn + 10];

    static void init(const unsigned int n)
    {
        iota(fa, fa + 1 + n, 0);
        fill(siz, siz + 1 + n, 1);
    }
    unsigned int find(const unsigned int x)
    {
        return fa[x] == x ? x : fa[x] = find(fa[x]);
    }
    void merge(unsigned int x, unsigned int y)
    {
        x = find(x);
        y = find(y);
        if (x == y)
            return;
        if (siz[x] < siz[y])
            swap(x, y);
        fa[y] = x;
        siz[x] += siz[y];
    }
    inline bool isConnect(const unsigned int x, const unsigned int y)
    {
        return find(x) == find(y);
    }
}

inline void addEdge(const unsigned int x, const unsigned int y)
{
    if (ed[x][y])
        throw invalid_argument("");
    ed[x][y] = ed[y][x] = 1;
    Disjoint::merge(x, y);
}
void dfsTree(const unsigned int x)
{
    for (unsigned int i = 0; i < mat[x].size(); ++i)
        if (mat[x][i] == 1)
        {
            mat[x][i] = mat[i][x] = 3;
            if (!Disjoint::isConnect(x, i))
            {
                addEdge(x, i);
                dfsTree(i);
            }
        }
}
void dfsCycle(const unsigned int x, const unsigned int rt)
{
    bool vis = false;
    for (unsigned int i = 0; i < mat[x].size(); ++i)
        if (mat[x][i] == 2 && !Disjoint::isConnect(x, i))
        {
            addEdge(x, i);
            dfsCycle(i, rt);
            vis = true;
        }
    if (!vis && x != rt)
        addEdge(x, rt);
}
void check()
{
    for (const auto& i : mat)
        if (find(i.cbegin(), i.cend(), 3) != i.cend())
            throw invalid_argument("");
}
template <unsigned int typ>
void checkConn(const unsigned int n)
{
    for (unsigned int i = 0; i < n; ++i)
        for (unsigned int j = 0; j < n; ++j)
            if (mat[i][j] == typ && Disjoint::isConnect(i, j))
                throw invalid_argument("");
}

int construct(Matrix<int> m)
{
    mat = move(m);
    Disjoint::init(mat.size());
    const unsigned int n = mat.size();
    ed.resize(n);
    for (auto& i : ed)
        i.resize(n);
    try
    {
        check();
        for (unsigned int i = 0; i < n; ++i)
            dfsTree(i);
        checkConn<2>(n);
        for (unsigned int i = 0; i < n; ++i)
            dfsCycle(i, i);
        checkConn<0>(n);
    }
    catch (...)
    {
        return 0;
    }
    build(ed);
    return 1;
}
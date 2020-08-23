#ifdef APTEST
#include "debug_tools/program.h"
#endif
#include <algorithm>
#include <iostream>
#include <iterator>
using namespace std;
const unsigned int maxn = 20000, maxlg = 60, maxln = 15;

struct Edge
{
    unsigned int to;
    const Edge* pre;
} ed[maxn * 2 + 10];
const Edge* head[maxn + 1];
class LinearBasis
{
public:
    void insert(unsigned long long x)
    {
        for (int i = maxlg; i >= 0 && x; --i)
        {
            if (!(x & (1ull << i)))
                continue;
            if (vec[i])
                x ^= vec[i];
            else
            {
                for (unsigned int j = 0; j < i; ++j)
                    if (x & (1ull << j))
                        x ^= vec[j];
                for (unsigned int j = i + 1; j <= maxlg; ++j)
                    if (vec[j] & (1ull << i))
                        vec[j] ^= x;
                vec[i] = x;
                break;
            }
        }
    }
    unsigned long long max() const
    {
        unsigned long long ret = 0;
        for (unsigned int i = 0; i <= maxlg; ++i)
            ret ^= vec[i];
        return ret;
    }
    LinearBasis operator+(const LinearBasis& b) const
    {
        LinearBasis ret = *this;
        for (unsigned int i = 0; i <= maxlg; ++i)
            ret.insert(b.vec[i]);
        return ret;
    }

private:
    unsigned long long vec[maxlg + 1] = {};
};
unsigned long long g[maxn + 1];
unsigned int lg[maxn + 1];
unsigned int fa[maxn + 1][maxln + 1], dep[maxn + 1];
LinearBasis lb[maxn + 1][maxln + 1];

inline void addEdge(const unsigned int from, const unsigned int to)
{
    static Edge* cur = ed;
    cur->to = to;
    cur->pre = head[from];
    head[from] = cur++;
}
static void getLog(const unsigned int n)
{
    for (unsigned int i = 2; i <= n; ++i)
        lg[i] = lg[i >> 1] + 1;
}
void init(const unsigned int x, const unsigned int f)
{
    dep[x] = dep[f] + 1;
    fa[x][0] = f;
    lb[x][0].insert(g[x]);
    for (unsigned int i = 1; i <= lg[dep[x]] + 1; ++i)
    {
        fa[x][i] = fa[fa[x][i - 1]][i - 1];
        lb[x][i] = lb[x][i - 1] + lb[fa[x][i - 1]][i - 1];
    }
    for (const Edge* i = head[x]; i; i = i->pre)
        if (i->to != f)
            init(i->to, x);
}
unsigned int kthFa(unsigned int x, unsigned int k)
{
    for (unsigned int i = 0; k; k >>= 1, ++i)
        if (k & 0x01)
            x = fa[x][i];
    return x;
}
unsigned int lca(unsigned int a, unsigned int b)
{
    if (dep[a] != dep[b])
    {
        if (dep[a] > dep[b])
            swap(a, b);
        b = kthFa(b, dep[b] - dep[a]);
    }
    if (a == b)
        return a;
    for (unsigned int i = lg[dep[a]]; i;)
    {
        if (fa[a][i - 1] == fa[b][i - 1])
            --i;
        else
        {
            a = fa[a][i - 1];
            b = fa[b][i - 1];
        }
    }
    return fa[a][0];
}
LinearBasis chainBasis(unsigned int x, unsigned int fa)
{
    const unsigned int d = dep[x] + 1 - dep[fa], ld = lg[d];
    return lb[x][ld] + lb[kthFa(x, d - (1 << ld))][ld];
}
unsigned long long query(unsigned int x, unsigned int y)
{
    const unsigned int l = lca(x, y);
    return (chainBasis(x, l) + chainBasis(y, l)).max();
}

int main()
{
#ifndef APTEST
    ios_base::sync_with_stdio(false);
#endif
    unsigned int n, q;
    cin >> n >> q;
    copy_n(istream_iterator<unsigned long long>(cin), n, g + 1);
    for (unsigned int i = 1; i < n; ++i)
    {
        unsigned int u, v;
        cin >> u >> v;
        addEdge(u, v);
        addEdge(v, u);
    }
    getLog(n);
    init(1, 0);
    for (unsigned int j = 0; j < q; ++j)
    {
        unsigned int x, y;
        cin >> x >> y;
        cout << (x == y ? g[x] : query(x, y)) << "\n";
    }
    return 0;
}
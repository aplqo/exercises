#ifdef APTEST
#include "debug_tools/program.h"
#endif
#include <algorithm>
#include <iostream>
#include <iterator>
using namespace std;
const unsigned int maxn = 50, maxm = maxn * maxn, maxf = 40;
constexpr unsigned long long mod = 10000;

struct Edge
{
    unsigned int to;
    const Edge* pre;
} ed[maxm * 2];
const Edge* head[maxn + 10];
unsigned int n;
struct Matrix
{
    void unit()
    {
        for (unsigned int i = 0; i < n; ++i)
            val[i][i] = 1;
    }
    inline const unsigned long long* operator[](const unsigned int i) const { return val[i]; }
    inline unsigned long long* operator[](const unsigned int i) { return val[i]; }
    unsigned long long val[maxn + 1][maxn + 1];
};
struct Fish
{
    unsigned int p[4], cnt;
} fs[maxf];
Matrix g[13] = {};

inline void addEdge(const unsigned int from, const unsigned int to)
{
    static Edge* cur = ed;
    new (cur) Edge { to, head[from] };
    head[from] = cur++;
    new (cur) Edge { from, head[to] };
    head[to] = cur++;
}
Matrix operator*(const Matrix& a, const Matrix& b)
{
    Matrix ret {};
    for (unsigned int i = 0; i < n; ++i)
        for (unsigned int j = 0; j < n; ++j)
            for (unsigned int k = 0; k < n; ++k)
                ret[i][k] = (ret[i][k] + a[j][k] * b[i][j]) % mod;
    return ret;
}
Matrix operator^(Matrix a, unsigned long long e)
{
    Matrix ret {};
    ret.unit();
    for (; e; e >>= 1)
    {
        if (e & 0x01)
            ret = ret * a;
        a = a * a;
    }
    return ret;
}
static void buildMatrix(const unsigned int n, const unsigned int nfs)
{
    static unsigned int fish[maxn + 10];
    g[0].unit();
    for (const Fish* i = fs; i < fs + nfs; ++i)
        g[0][i->p[0]][i->p[0]] = 0;
    for (unsigned int i = 0; i < 12; ++i)
    {
        const unsigned int nxt = i + 1;
        for (const Fish* j = fs; j < fs + nfs; ++j)
            fish[j->p[nxt % j->cnt]] = nxt;
        for (unsigned int v = 0; v < n; ++v)
            for (unsigned int p = 0; p < n; ++p)
            {
                if (!g[i][v][p])
                    continue;
                for (const Edge* j = head[p]; j; j = j->pre)
                    if (fish[j->to] != i + 1)
                        g[nxt][v][j->to] = (g[nxt][v][j->to] + g[i][v][p]) % mod;
            }
    }
}
int main()
{
#ifndef APTEST
    ios_base::sync_with_stdio(false);
#endif
    unsigned int m, s, t, nfs;
    unsigned long long k;
    cin >> n >> m >> s >> t >> k;
    for (unsigned int i = 0; i < m; ++i)
    {
        unsigned int x, y;
        cin >> x >> y;
        addEdge(x, y);
    }
    cin >> nfs;
    for (Fish* i = fs; i < fs + nfs; ++i)
    {
        cin >> i->cnt;
        copy_n(istream_iterator<unsigned int>(cin), i->cnt, i->p);
    }
    buildMatrix(n, nfs);
    cout << (g[k % 12] * (g[12] ^ (k / 12)))[s][t] << "\n";
    return 0;
}
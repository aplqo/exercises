/*
* Luogu T127941 : No longer far planet
*/
#ifdef APTEST
#include "debug_tools/program.h"
#endif
#include <algorithm>
#include <cmath>
#include <iostream>
using namespace std;
const int maxn = 200, maxm = 10000, maxq = 20;

unsigned int blocksize;

class block
{
public:
    void init(const unsigned int l)
    {
        fill(dat, dat + blocksize, 0);
        dirt = false;
        lv = l;
    }
    void set(const unsigned int l, const unsigned int r, const unsigned int v)
    {
        if (l == lv && r - l == blocksize)
        {
            dirt = true;
            setv = v;
            return;
        }
        if (dirt)
        {
            dirt = false;
            fill(dat, dat + blocksize, setv);
        }
        fill(dat + l - lv, dat + r - lv, v);
    }
    unsigned int query(const unsigned int p)
    {
        return dirt ? setv : dat[p - lv];
    }

private:
    bool dirt = false;
    unsigned int lv;
    unsigned int dat[maxq + 10], setv;
} blk[maxn + 10];
struct edge
{
    unsigned int from, to;
} ed[maxm + 10];
unsigned int mat[maxn + 10];

bool test(const edge& e, const unsigned int n)
{
    auto try_edge = [](const unsigned int e1, const unsigned int e2) -> bool {
        return blk[e1 / blocksize].query(e1) == blk[e2 / blocksize].query(e2);
    };
    auto lst = [&n](const unsigned int p) -> unsigned int {
        return mat[p] ? mat[p] - 1 : mat[n - 1];
    };
    return try_edge(lst(e.from), mat[e.to]) || try_edge(mat[e.from], lst(e.to))
        || try_edge(e.from, e.to) || try_edge(lst(e.from), lst(e.to));
}
void set(unsigned int l, unsigned int r, const unsigned int v)
{
    if (r < l)
        swap(l, r);
    const unsigned int indl = l / blocksize, indr = r / blocksize;
    if (indl < indr)
    {
        for (unsigned int i = indl + 1; i < indr; ++i)
            blk[i].set(blocksize * i, blocksize * (i + 1), v);
        blk[indl].set(l, (indl + 1) * blocksize, v);
        blk[indr].set(indr * blocksize, r, v);
    }
    else
        blk[indl].set(l, r, v);
}

void solve()
{
    unsigned int n, m;
    cin >> n >> m;
    blocksize = sqrt(n);
    for (unsigned int i = 0; i <= n / blocksize; ++i)
        blk[i].init(i * blocksize);
    for (edge* i = ed; i < ed + m; ++i)
        cin >> i->from >> i->to;
    for (unsigned int i = 0; i < n; ++i)
    {
        unsigned int t;
        cin >> t;
        mat[t] = i;
    }
    for (edge* i = ed; i < ed + m; ++i)
    {
        if (!test(*i, n))
        {
            cout << "NO" << endl;
            return;
        }
        set(mat[i->from], mat[i->to], i - ed);
    }
    cout << "YES" << endl;
}
int main()
{
#ifndef APTEST
    ios_base::sync_with_stdio(false);
#endif
    unsigned int t;
    cin >> t;
    for (unsigned int i = 0; i < t; ++i)
        solve();
    return 0;
}
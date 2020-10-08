#ifdef APTEST
#include "debug_tools/program.h"
#endif
#include <algorithm>
#include <iostream>
#include <iterator>
#include <numeric>
#include <utility>
using namespace std;
const unsigned int maxn = 5000, maxm = 5000;

unsigned int seq[maxn * maxm + 10];
unsigned int lLim[maxn + 10], rLim[maxn + 10], *pos, ans[maxn + maxm + 10];

struct Parm
{
    unsigned int a, b, c, d;
};
void generatePerm(unsigned long long x, const unsigned int len, const Parm p)
{
    unsigned int* xseq = new unsigned int[len + 1];
    for (unsigned int i = 1; i <= len; ++i)
    {
        x = ((x * x * p.a) % p.d + x * p.b % p.d + p.c % p.d) % p.d;
        xseq[i] = x;
    }
    iota(seq + 1, seq + len + 1, 1);
    for (unsigned int i = 1; i <= len; ++i)
        swap(seq[i], seq[xseq[i] % i + 1]);
    delete[] xseq;
}
void Sort(const unsigned int len)
{
    pos = new unsigned int[len + 1];
    for (unsigned int i = 1; i <= len; ++i)
        pos[seq[i]] = i - 1;
}
inline pair<unsigned int, unsigned int> toPos(const unsigned int id, const unsigned int m)
{
    return make_pair(id / m, id % m);
}
void greedy(const unsigned int n, const unsigned int m)
{
    for (unsigned int i = 0; i < n; ++i)
        rLim[i] = m - 1;
    unsigned int cur = 0;
    for (unsigned int i = 1; i <= n * m && cur < n + m; ++i)
    {
        const auto [x, y] = toPos(pos[i], m);
        if (lLim[x] <= y && y <= rLim[x])
        {
            ans[cur++] = i;
            for (unsigned int i = 0; i < x; ++i)
                rLim[i] = min(rLim[i], y);
            for (unsigned int j = x + 1; j < n; ++j)
                lLim[j] = max(lLim[j], y);
        }
    }
}

int main()
{
#ifndef APTEST
    ios_base::sync_with_stdio(false);
#endif
    unsigned int x, a, b, c, d;
    cin >> x >> a >> b >> c >> d;
    unsigned int n, m, q;
    cin >> n >> m >> q;
    generatePerm(x, n * m, Parm { a, b, c, d });
    for (unsigned int i = 0; i < q; ++i)
    {
        unsigned int u, v;
        cin >> u >> v;
        swap(seq[u], seq[v]);
    }
    Sort(n * m);
    greedy(n, m);
    copy(ans, ans + n + m - 1, ostream_iterator<unsigned int>(cout, " "));
    cout << endl;
    delete[] pos;
    return 0;
}
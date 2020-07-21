#ifdef APTEST
#include "debug_tools/program.h"
#endif
#include <algorithm>
#include <cctype>
#include <iostream>
using namespace std;
const unsigned int maxn = 3e6, maxm = 3e6, maxc = 3e6;

char inbuf[maxn * 10 + maxm * 10 + 1000], *inpos = inbuf, outbuf[maxm + maxm * 10 + 1000], *pos = outbuf;
template <class T = unsigned int>
inline T read()
{
    T ret = 0;
    char c = *(inpos++);
    while (!isdigit(c))
        c = *(inpos++);
    while (isdigit(c))
    {
        ret = ret * 10 + c - '0';
        c = *(inpos++);
    }
    return ret;
}
inline void write(unsigned int v)
{
    pos += sprintf(pos, "%u\n", v);
}

struct Query
{
    unsigned int l, r;
    unsigned int ans;
} qry[maxm + 1];
Query* seq[maxm + 1];
unsigned int x[maxn + 1], lst[maxn + 1];

namespace ftree
{
#define lowbit(x) ((x) & -(x))
    int dat[maxn + 2];

    void modify(unsigned int p, const unsigned int n, const int val)
    {
        for (; p <= n; p += lowbit(p))
            dat[p] += val;
    }
    unsigned int query(unsigned int p)
    {
        unsigned int ret = 0;
        for (; p; p -= lowbit(p))
            ret += dat[p];
        return ret;
    }
    inline void increase(const unsigned int l, const unsigned int r, const unsigned int n) // [l,r) +=1
    {
        modify(l, n, 1);
        modify(r, n, -1);
    }

#undef lowbit
}
static void init(const unsigned int n)
{
    static unsigned int pos[maxc + 1];
    for (unsigned int i = 1; i <= n; ++i)
    {
        lst[i] = pos[x[i]];
        pos[x[i]] = i;
    }
}
void solveQuery(const unsigned int n, const unsigned int q)
{
    unsigned int p = 0;
    for (unsigned int i = 1; i <= n; ++i)
    {
        const unsigned int lst1 = lst[i], lst2 = lst[lst[i]];
        if (lst1)
            ftree::increase(lst2 + 1, lst1 + 1, n + 1);
        for (; p < q && seq[p]->r == i; ++p)
            seq[p]->ans = ftree::query(seq[p]->l);
    }
}
int main()
{
    ios_base::sync_with_stdio(false);
    cin.read(inbuf, maxn * 10 + maxm * 10 + 100);
    const auto n = read();
    const auto c = read();
    const auto m = read();
    generate_n(x + 1, n, read<unsigned int>);
    for (Query* i = qry; i < qry + m; ++i)
    {
        i->l = read();
        i->r = read();
    }
    transform(qry, qry + m, seq, [](Query& i) { return &i; });
    sort(seq, seq + m, [](Query* l, Query* r) { return l->r < r->r; });
    init(n);
    solveQuery(n, m);
    for (Query* i = qry; i < qry + m; ++i)
        write(i->ans);
    cout.write(outbuf, pos - outbuf);
    return 0;
}
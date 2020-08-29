#ifdef APTEST
#include "debug_tools/program.h"
#endif
#include <algorithm>
#include <cmath>
#include <iostream>
using namespace std;
const unsigned int maxn = 100000, maxm = 100000, maxv = 1 << 20;

struct Query
{
    unsigned int l, r;
    unsigned long long ans = 0;
} qry[maxm + 10];
Query* seq[maxm + 10];
unsigned int a[maxn + 10], cnt[maxv + 10];
unsigned int bsize;

inline unsigned int add(const unsigned int v, const unsigned int k)
{
    const unsigned int ret = cnt[v ^ k];
    ++cnt[v];
    return ret;
}
inline unsigned int remove(const unsigned int v, const unsigned int k)
{
    --cnt[v];
    return cnt[v ^ k];
}
void MoAlgo(const unsigned int m, const unsigned int k)
{
    transform(qry, qry + m, seq, [](Query& i) { return &i; });
    sort(seq, seq + m, [](const Query* a, const Query* b) {
        return a->l / bsize != b->l / bsize ? a->l / bsize < b->l / bsize : a->r < b->r;
    });
    ++cnt[a[seq[0]->l]];
    unsigned long long ans = 0;
    unsigned int l = seq[0]->l, r = seq[0]->l;
    for (unsigned int i = 0; i < m; ++i)
    {
        Query& cq = *seq[i];
        while (l > cq.l)
            ans += add(a[--l], k);
        while (r < cq.r)
            ans += add(a[++r], k);
        while (l < cq.l)
            ans -= remove(a[l++], k);
        while (r > cq.r)
            ans -= remove(a[r--], k);
        cq.ans = ans;
    }
}
int main()
{
#ifndef APTEST
    ios_base::sync_with_stdio(false);
#endif
    unsigned int n, m, k;
    cin >> n >> m >> k;
    for (unsigned int i = 1; i <= n; ++i)
    {
        unsigned int v;
        cin >> v;
        a[i] = a[i - 1] ^ v;
    }
    for (Query* i = qry; i < qry + m; ++i)
    {
        cin >> i->l >> i->r;
        --(i->l);
    }
    bsize = sqrt(n);
    MoAlgo(m, k);
    for (Query* i = qry; i < qry + m; ++i)
        cout << i->ans << "\n";
    return 0;
}
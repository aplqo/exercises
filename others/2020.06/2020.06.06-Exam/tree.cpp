#ifdef APTEST
#include "debug_tools/program.h"
#endif
#include <algorithm>
#include <cstdio>
#include <cstring>
#include <vector>
using namespace std;
const unsigned int maxn = 2000, maxq = 1000000;

struct edge
{
    unsigned int to;
    edge* pre = nullptr;
} ed[maxn * 2 + 1];
edge* head[maxn + 1];
unsigned int a[maxn + 1];
unsigned int qCnt[maxn + 1], cCnt[maxn + 1];
unsigned int stk[maxn + 1][maxn + 1];
unsigned int tmp[maxn + 1], tCnt = 0;
unsigned int cnt[maxn + 1];

struct query
{
    unsigned int s, t;
} qry[maxq + 1];
unsigned int ans;

void addEdge(const unsigned int from, const unsigned int to)
{
    static edge* cur = ed;
    cur->to = to;
    cur->pre = head[from];
    head[from] = cur;
    ++cur;
}

unsigned int dfsQuery(const unsigned int x, const unsigned int fa)
{
    for (edge* i = head[x]; i; i = i->pre)
        if (i->to != fa)
            qCnt[x] += dfsQuery(i->to, x);
    return qCnt[x] + (cCnt[x] ? 1 : 0);
}
inline void insert(const unsigned int val, unsigned int v[], unsigned int& cnt)
{
    unsigned int* t = lower_bound(v, v + cnt, val);
    if (t == v + cnt)
        ++cnt;
    *t = val;
}
void dfs(const unsigned int x, const unsigned int fa, unsigned int v[], unsigned int& vCnt)
{
    insert(a[x], v, vCnt);
    if (cCnt[x])
        ans += cCnt[x] * vCnt;
    if (qCnt[x] > 1)
    {
        for (edge* i = head[x]; i; i = i->pre)
        {
            if ((!qCnt[i->to] && !cCnt[i->to]) || i->to == fa)
                continue;
            copy(v, v + vCnt, stk[x]);
            cnt[x] = vCnt;
            dfs(i->to, x, stk[x], cnt[x]);
        }
    }
    else if (qCnt[x])
    {
        for (edge* i = head[x]; i; i = i->pre)
            if ((cCnt[i->to] || qCnt[i->to]) && i->to != fa)
            {
                dfs(i->to, x, v, vCnt);
                break;
            }
    }
}
void solveQuery(const unsigned int qn, const unsigned int n)
{
    sort(qry, qry + qn, [](const query& a, const query& b) {
        return a.s != b.s ? a.s < b.s : a.t < b.t;
    });
    query* p = qry;
    for (unsigned int i = 1; i <= n; ++i)
    {
        memset(qCnt, 0, sizeof(qCnt));
        memset(cCnt, 0, sizeof(cCnt));
        while (p->s == i && p < qry + qn)
        {
            ++cCnt[p->t];
            ++p;
        }
        tCnt = 0;
        if (dfsQuery(i, 0))
            dfs(i, 0, tmp, tCnt);
    }
}

int main()
{
    unsigned int n, q;
    scanf("%u", &n);
    for (unsigned int i = 1; i < n; ++i)
    {
        unsigned int s, t;
        scanf("%u%u", &s, &t);
        addEdge(s, t);
        addEdge(t, s);
    }
    for (unsigned int* i = a + 1; i < a + n + 1; ++i)
        scanf("%u", i);
    {
        unsigned int x, y, z;
        scanf("%u%u%u%u%u%u", &q, &qry[0].s, &qry[0].t, &x, &y, &z);
        const auto fun = [x, y, z](const unsigned int v, const unsigned int n) {
            return (v * v * x + v * y + z) % n + 1;
        };
        for (query* i = qry + 1; i < qry + q; ++i)
        {
            i->s = fun((i - 1)->s, n);
            i->t = fun((i - 1)->t, n);
        }
    }
    solveQuery(q, n);
    printf("%d", ans);
    return 0;
}

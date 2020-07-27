#ifdef APTEST
#include "debug_tools/program.h"
#endif
#include <algorithm>
#include <cctype>
#include <climits>
#include <cmath>
#include <cstdio>
#include <cstring>
#include <functional>
#include <iostream>
#include <limits>
#include <numeric>
using namespace std;
const unsigned int maxn = 100000;
using real = double;
constexpr real ninf = numeric_limits<real>::lowest() / 2, eps = 1e-4;

char buf[3 * maxn * 10 + 100], *inpos = buf;
template <class T>
T read()
{
    char c = *(inpos++);
    T ret = 0;
    while (!isdigit(c))
        c = *(inpos++);
    while (isdigit(c))
    {
        ret = ret * 10 + c - '0';
        c = *(inpos++);
    }
    return ret;
}

unsigned int siz[maxn + 1], eds[maxn + 1];
int dep[maxn + 1];
bool dcvis[maxn + 1];
int l, u;
real dis[maxn + 1];
real v, ans;
struct
{
    struct edge
    {
        unsigned int to;
        int val;
        const edge* pre;
    } ed[maxn * 2];
    const edge* head[maxn + 1] = {};
    edge* cur = ed;

    void addEdge(const unsigned int from, const unsigned int to, const int val = 0)
    {
        cur->to = to;
        cur->val = val;
        cur->pre = head[from];
        head[from] = cur;
        ++cur;
    }
} orig, nxt, dc;
template <class T>
class Array
{
public:
    template <class... Args>
    inline void push_back(Args... args) { new (cur++) T(args...); }
    inline T* begin() { return val; }
    inline T* end() { return cur; }
    inline void clear() { cur = val; }

    T val[maxn + 1], *cur = val;
};
Array<unsigned int> subs;
class mqueue
{
public:
    inline void push(const int len, const real val)
    {
        while (l < r && (r - 1)->second < val)
            --r;
        *(r++) = make_pair(len, val);
    }
    inline void pop(const int len)
    {
        while (l < r && l->first > len)
            ++l;
    }
    inline real front() const { return l >= r ? ninf : l->second; }
    inline void clear() { l = r = q; }

private:
    pair<int, real> q[maxn + 1], *l = q, *r = q;
};
class Paths
{
public:
    Paths()
    {
        fill(vals, vals + 1 + maxn, ninf);
        ptr = lens.cur;
    }
    inline void push(const int l, const real v)
    {
        if (!exist[l])
        {
            exist[l] = true;
            lens.push_back(l);
        }
        vals[l] = max(vals[l], v);
    }
    inline void resetPtr() { ptr = lens.cur - 1; }
    inline void clear()
    {
        for (auto i : lens)
        {
            exist[i] = false;
            vals[i] = ninf;
        }
        lens.clear();
        resetPtr();
    }
    inline void merge(Paths& b)
    {
        for (auto l : b.lens)
            push(l, b.vals[l]);
        resetPtr();
    }
    inline void extract(mqueue& q, const int d)
    {
        for (; ptr >= lens.val && *ptr > u - d; --ptr)
            ;
        for (; ptr >= lens.val && *ptr >= l - d; --ptr)
            q.push(*ptr, vals[*ptr]);
    }

private:
    Array<int> lens;
    real vals[maxn + 1];
    bool exist[maxn + 1];
    int* ptr;
} cur, lst;

void dfsPoint(const unsigned int x, const unsigned int f, const unsigned int n, unsigned int& pos, unsigned int& mval)
{
    unsigned int ms = 0;
    siz[x] = 1;
    for (auto i = orig.head[x]; i; i = i->pre)
    {
        const unsigned int to = i->to;
        if (to == f || dcvis[to])
            continue;
        dfsPoint(to, x, n, pos, mval);
        siz[x] += siz[to];
        ms = max(ms, siz[to]);
    }
    ms = max(ms, n - siz[x]);
    if (ms < mval)
    {
        pos = x;
        mval = ms;
    }
}
unsigned int treeCenter(const unsigned int n, const unsigned int x)
{
    unsigned int ret, mv = UINT_MAX;
    dfsPoint(x, 0, n, ret, mv);
    return ret;
}
unsigned int dfsDepth(const unsigned int x, const unsigned int f)
{
    siz[x] = 1;
    unsigned int ret = 1;
    for (auto i = orig.head[x]; i; i = i->pre)
        if (i->to != f && !dcvis[i->to])
        {
            ret = max(ret, dfsDepth(i->to, x) + 1);
            siz[x] += siz[i->to];
        }
    return ret;
}
void dfsDCTree(const unsigned int x)
{
    static int dis[maxn + 1];
    dcvis[x] = true;
    for (auto i = orig.head[x]; i; i = i->pre)
    {
        const unsigned int to = i->to;
        if (dcvis[to])
            continue;
        dep[to] = dfsDepth(to, x);
        dis[to] = i->val;
        subs.push_back(to);
    }
    sort(subs.val, subs.cur, [](unsigned int a, unsigned int b) { return dep[a] > dep[b]; });
    for (auto i : subs)
        nxt.addEdge(x, i, dis[i]);

    subs.clear();
    for (auto i = orig.head[x]; i; i = i->pre)
    {
        if (dcvis[i->to])
            continue;
        const unsigned int w = treeCenter(siz[i->to], i->to);
        dc.addEdge(x, w);
        dfsDCTree(w);
    }
}
void buildDCTree(const unsigned int n)
{
    const unsigned int w = treeCenter(n, 1);
    dc.addEdge(0, w);
    dfsDCTree(w);
    memset(dcvis, 0, sizeof(dcvis));
}
void bfsSubtree(const unsigned int x)
{
    static unsigned int vis[maxn + 1], sta = 0;
    static unsigned int q[maxn + 1], *l = q, *r = q;
    static mqueue mq;
    ++sta;
    mq.clear();
    l = r = q;
    *(r++) = x;
    vis[x] = sta;
    while (l < r)
    {
        const unsigned int c = *l;
        ++l;
        mq.pop(u - dep[c]);
        lst.extract(mq, dep[c]);
        ans = max(ans, mq.front() + dis[c]);
        cur.push(dep[c], dis[c]);
        for (auto i = orig.head[c]; i; i = i->pre)
        {
            const unsigned int to = i->to;
            if (dcvis[to] || vis[to] == sta)
                continue;
            vis[to] = sta;
            dep[to] = dep[c] + 1;
            dis[to] = dis[c] + i->val - v;
            *(r++) = to;
        }
    }
}
void divide(const unsigned int x)
{
    dcvis[x] = true;
    dep[x] = 0;
    dis[x] = 0;
    lst.push(0, dis[x]);
    lst.resetPtr();
    for (auto i = nxt.head[x]; i; i = i->pre)
    {
        const unsigned int to = i->to;
        dep[to] = 1;
        dis[to] = i->val - v;
        bfsSubtree(i->to);
        lst.merge(cur);
        cur.clear();
    }

    lst.clear();
    for (auto j = dc.head[x]; j; j = j->pre)
        divide(j->to);
}
real diviceConquer(const real va)
{
    v = va;
    ans = ninf;
    divide(dc.head[0]->to);
    memset(dcvis, 0, sizeof(dcvis));
    return ans;
}
real binaryFind()
{
    real l = eds[0] / real(u), r = accumulate(eds, eds + u, 0ull) / real(::l);
    while (fabs(r - l) > eps)
    {
        const real m = (l + r) / 2;
        if (diviceConquer(m) >= 0)
            l = m;
        else
            r = m;
    }
    return l;
}

int main()
{
    ios_base::sync_with_stdio(false);
    cin.read(buf, sizeof(buf));
    const unsigned int n = read<unsigned int>();
    l = read<unsigned int>();
    u = read<unsigned int>();
    for (unsigned int i = 1; i < n; ++i)
    {
        unsigned int a = read<unsigned int>(), b = read<unsigned int>();
        int v = read<int>();
        eds[i - 1] = v;
        orig.addEdge(a, b, v);
        orig.addEdge(b, a, v);
    }
    buildDCTree(n);
    partial_sort(eds, eds + u + 1, eds + n - 1, greater<unsigned long long>());
    ios_base::sync_with_stdio(true);
    printf("%.3lf\n", binaryFind());
    return 0;
}
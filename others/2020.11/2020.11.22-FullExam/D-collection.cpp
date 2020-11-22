#ifdef APTEST
#include "debug_tools/program.h"
#endif
#include <algorithm>
#include <iostream>
#include <iterator>
#include <numeric>
#include <vector>
using namespace std;
const unsigned int maxn = 5e5, maxm = 5e5;

struct Operate
{
    void (*eval)(unsigned int x, unsigned int y);
    unsigned int id;
    unsigned int set, ar2;
} ma[maxm + 10];
long long ans[maxm + 10];
struct PartQuery
{
    unsigned int id, x;
    long long *ret, rat;
} pqr[maxm * 2 + 10];
namespace ReplaceSet
{
    struct Tag
    {
        unsigned int time, val;
        inline bool operator<(const Tag& r) const { return time < r.time; }
    };
    Tag tag[maxn * 2 + 10];
    unsigned int fa[maxn * 2 + 10], cur;

    static void init(const unsigned int n)
    {
        iota(fa, fa + 1 + n * 2, 0);
        cur = n + 1;
    }
    unsigned int find(unsigned int x)
    {
        if (fa[x] == x)
            return x;
        const unsigned int rt = find(fa[x]);
        tag[x] = max(tag[x], tag[fa[x]]);
        return fa[x] = rt;
    }
    inline Tag index(unsigned int idx)
    {
        find(idx);
        return tag[idx];
    }
    void merge(unsigned int x, unsigned int y)
    {
        x = find(x);
        y = find(y);
        fa[x] = fa[y] = cur++;
    }
    void replace(const unsigned int time, unsigned int x, const unsigned int val)
    {
        x = find(x);
        tag[x] = Tag { time, val };
    }
}
namespace AddSet
{
    unsigned int fa[maxn + 10];
    long long tag[maxn + 10], c[maxn + 10];
    vector<unsigned int> child[maxn + 10];

    static void init(const unsigned int n)
    {
        iota(fa, fa + 1 + n, 0);
        for (unsigned int i = 1; i <= n; ++i)
            child[i].push_back(i);
    }
    unsigned int find(unsigned int x)
    {
        return fa[x] == x ? x : fa[x] = find(fa[x]);
    }
    inline long long index(const unsigned int x)
    {
        return tag[find(x)] + c[x];
    }
    void merge(unsigned int x, unsigned int y)
    {
        x = find(x);
        y = find(y);
        if (child[x].size() < child[y].size())
            swap(x, y);
        fa[y] = x;
        const long long delt = tag[y] - tag[x];
        for (unsigned int i : child[y])
        {
            c[i] += delt;
            child[x].push_back(i);
        }
        child[y].clear();
    }
    void add(unsigned int x, unsigned int delt)
    {
        tag[find(x)] += delt;
    }

}
void answerQuery(const unsigned int n, const PartQuery* cpq, const Operate* cop)
{
    AddSet::init(n);
    const Operate* ptr = ma;
    for (const PartQuery* i = pqr; i < cpq; ++i)
    {
        for (const unsigned int cid = i->id; ptr < cop && ptr->id < cid; ++ptr)
            ptr->eval(ptr->set, ptr->ar2);
        *(i->ret) += i->rat * AddSet::index(i->x);
    }
}

int main()
{
#ifndef APTEST
    ios_base::sync_with_stdio(false);
#endif
    unsigned int n, m;
    cin >> n >> m;
    ReplaceSet::init(n);
    long long* cqr = ans;
    {
        PartQuery* cpq = pqr;
        Operate* pop = ma;
        for (unsigned int i = 1; i <= m; ++i)
        {
            unsigned int op, x, y;
            cin >> op >> x;
            switch (op)
            {
            case 1:
                cin >> y;
                *(pop++) = Operate { AddSet::merge, i, x, y };
                break;
            case 2:
                cin >> y;
                ReplaceSet::merge(x, y);
                break;
            case 3:
                cin >> y;
                *(pop++) = Operate { AddSet::add, i, x, y };
                break;
            case 4:
                cin >> y;
                ReplaceSet::replace(i, x, y);
                break;
            case 5:
            {
                const auto t = ReplaceSet::index(x);
                *cqr = t.val;
                *(cpq++) = PartQuery { t.time, x, cqr, -1 };
                *(cpq++) = PartQuery { i, x, cqr, 1 };
                ++cqr;
                break;
            }
            }
        }
        sort(pqr, cpq, [](const PartQuery& a, const PartQuery& b) { return a.id < b.id; });
        answerQuery(n, cpq, pop);
    }
    copy<const long long*>(ans, cqr, ostream_iterator<long long>(cout, "\n"));
    return 0;
}

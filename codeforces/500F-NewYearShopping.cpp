#ifdef APTEST
#include "debug_tools/program.h"
#endif
#include <algorithm>
#include <iostream>
#include <utility>
#include <vector>
using namespace std;
const unsigned int maxn = 4000, maxt = 2e4, maxq = 2e4, maxb = 4000;
typedef pair<unsigned int, unsigned int> Object; // first cost, second happiness

struct Query
{
    unsigned int time, budget;
    unsigned int ans;
} qry[maxq + 10];
Query* seq[maxq + 10];
unsigned int pos[maxt + 10], q;

namespace Pack
{
    unsigned int val[maxn + 10][maxb + 10];
    unsigned int pos;

    void addObject(const Object obj);
}
class STree
{
public:
    void build(const unsigned int l, const unsigned int r, STree*(&cur));
    void addObject(const unsigned int l, const unsigned int r, const Object obj);
    void dfs() const;

private:
    STree *lc, *rc;
    unsigned int lv, mv, rv;
    vector<Object> val;
} SNodes[maxt * 4 + 10], *root = SNodes;

void Pack::addObject(const Object obj)
{
    ++pos;
    copy(val[pos - 1], val[pos - 1] + maxb + 1, val[pos]);
    unsigned int* const cv = val[pos];
    for (unsigned int i = maxb; i >= obj.first; --i)
        cv[i] = max(cv[i], cv[i - obj.first] + obj.second);
}
void STree::build(const unsigned int l, const unsigned int r, STree*(&cur))
{
    lv = l;
    mv = (l + r) >> 1;
    rv = r;
    if (r - l == 1)
        return;
    lc = ++cur;
    rc = ++cur;
    lc->build(l, mv, cur);
    rc->build(mv, r, cur);
}
void STree::addObject(const unsigned int l, const unsigned int r, const Object obj)
{
    if (l == lv && r == rv)
    {
        val.push_back(obj);
        return;
    }
    if (r <= mv)
        lc->addObject(l, r, obj);
    else if (l >= mv)
        rc->addObject(l, r, obj);
    else
    {
        lc->addObject(l, mv, obj);
        rc->addObject(mv, r, obj);
    }
}
void STree::dfs() const
{
    const unsigned int stat = Pack::pos;
    for (const Object& i : val)
        Pack::addObject(i);
    if (rv - lv == 1)
    {
        for (unsigned int i = pos[lv]; i < q && seq[i]->time == lv; ++i)
            seq[i]->ans = Pack::val[Pack::pos][seq[i]->budget];
    }
    else
    {
        lc->dfs();
        rc->dfs();
    }
    Pack::pos = stat;
}

static void initQuery(const unsigned int q)
{
    transform(qry, qry + q, seq, [](Query& i) { return &i; });
    sort(seq, seq + q, [](const Query* a, const Query* b) { return a->time < b->time; });
    unsigned int ptr = 0;
    for (unsigned int i = 1; i <= maxt && ptr < q; ++i)
    {
        pos[i] = ptr;
        ptr = upper_bound(seq + ptr, seq + q, i, [](const unsigned int t, const Query* a) { return t < a->time; }) - seq;
    }
}

int main()
{
#ifndef APTEST
    ios_base::sync_with_stdio(false);
#endif
    unsigned int n, p;
    cin >> n >> p;
    {
        STree* c = SNodes;
        root->build(1, maxt + 1, c);
    }
    for (unsigned int i = 0; i < n; ++i)
    {
        unsigned int c, h, t;
        cin >> c >> h >> t;
        root->addObject(t, t + p, Object { c, h });
    }
    cin >> q;
    for (Query* i = qry; i < qry + q; ++i)
        cin >> i->time >> i->budget;
    initQuery(q);
    root->dfs();
    for (const Query* i = qry; i < qry + q; ++i)
        cout << i->ans << "\n";
    return 0;
}
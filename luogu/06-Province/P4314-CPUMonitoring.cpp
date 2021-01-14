#ifdef APTEST
#include "debug_tools/program.h"
#endif
#include <algorithm>
#include <climits>
#include <iostream>
#include <iterator>
using namespace std;
const unsigned int maxn = 2e5, maxm = 1e5, maxl = 31, maxp = 1u << maxl;
constexpr unsigned int maxNode = maxn * maxl * 4;
constexpr int ninf = INT_MIN / 2;

enum Operate
{
    Max,
    HistoryMax,
    Add,
    Assign
};
class SegmentTree
{
public:
    void build(const unsigned int l, const unsigned int r, const int val[], SegmentTree*(&cur));
    template <Operate op>
    int query(const unsigned int l, const unsigned int r);
    template <Operate op>
    void modify(const unsigned int l, const unsigned int r, const int delt);

private:
    unsigned int lv, mv, rv;
    SegmentTree *lc, *rc;
    struct Tag
    {
        int add, assign, max;
    } current, history;
    bool assigned;

    inline void pushAdd(const int delt, const int history);
    inline void pushAssign(const int val, const int history);
    inline void pushdown();
    inline void pushup();
} nodes[maxn * 4 + 10], *root = nodes;

void SegmentTree::build(const unsigned int l, const unsigned int r, const int val[], SegmentTree*(&cur))
{
    lv = l;
    rv = r;
    mv = (l + r) >> 1;
    assigned = false;
    current.add = history.add = 0;
    if (r - l == 1)
    {
        current.max = history.max = val[l];
        return;
    }
    lc = ++cur;
    lc->build(l, mv, val, cur);
    rc = ++cur;
    rc->build(mv, r, val, cur);
    pushup();
}
template <Operate op>
int SegmentTree::query(const unsigned int l, const unsigned int r)
{
    static_assert(op == Max || op == HistoryMax, "Invalid query");
    if (l == lv && r == rv)
        return op == HistoryMax ? history.max : current.max;
    pushdown();
    if (r <= mv)
        return lc->query<op>(l, r);
    else if (l >= mv)
        return rc->query<op>(l, r);
    else
        return std::max(lc->query<op>(l, mv), rc->query<op>(mv, r));
}
template <Operate op>
void SegmentTree::modify(const unsigned int l, const unsigned int r, const int delt)
{
    static_assert(op == Add || op == Assign, "Invalid operate");
    if (rv - lv == 1)
    {
        if constexpr (op == Add)
            current.max += delt;
        else
            current.max = delt;
        history.max = max(history.max, current.max);
        return;
    }
    pushdown();
    if (l == lv && r == rv)
    {
        if constexpr (op == Add)
            pushAdd(delt, delt);
        else
            pushAssign(delt, delt);
        return;
    }
    if (r <= mv)
        lc->modify<op>(l, r, delt);
    else if (l >= mv)
        rc->modify<op>(l, r, delt);
    else
    {
        lc->modify<op>(l, mv, delt);
        rc->modify<op>(mv, r, delt);
    }
    pushup();
}
inline void SegmentTree::pushAdd(const int delt, const int his)
{
    if (rv - lv == 1)
    {
        history.max = max(history.max, current.max + his);
        current.max += delt;
        return;
    }
    if (assigned)
    {
        history.assign = max(history.assign, current.assign + his);
        history.max = max(history.max, history.assign);
        current.assign += delt;
        current.max += delt;
    }
    else
    {
        history.add = max(history.add, current.add + his);
        history.max = max(history.max, current.max + his);
        current.add += delt;
        current.max += delt;
    }
}
inline void SegmentTree::pushAssign(const int delt, const int his)
{
    if (rv - lv == 1)
    {
        history.max = max(history.max, his);
        current.max = delt;
        return;
    }
    history.max = max(history.max, his);
    history.assign = max(history.assign, his);
    current.max = current.assign = delt;
    assigned = true;
}
inline void SegmentTree::pushdown()
{
    if (current.add || history.add)
    {
        lc->pushAdd(current.add, history.add);
        rc->pushAdd(current.add, history.add);
        current.add = history.add = 0;
    }
    if (assigned)
    {
        lc->pushAssign(current.assign, history.assign);
        rc->pushAssign(current.assign, history.assign);
        assigned = false;
    }
}
inline void SegmentTree::pushup()
{
    history.max = max(lc->history.max, rc->history.max);
    current.max = max(lc->current.max, rc->current.max);
}

int main()
{
#ifndef APTEST
    ios_base::sync_with_stdio(false);
#endif
    unsigned int t, e;
    cin >> t;
    {
        static int v[maxn + 19];
        copy_n(istream_iterator<int>(cin), t, v + 1);
        SegmentTree* c = nodes;
        root->build(1, t + 1, v, c);
    }
    cin >> e;
    for (unsigned int i = e; i; --i)
    {
        char op;
        unsigned int l, r;
        int z;
        cin >> op >> l >> r;
        switch (op)
        {
        case 'Q':
            cout << root->query<Max>(l, r + 1) << "\n";
            break;
        case 'A':
            cout << root->query<HistoryMax>(l, r + 1) << "\n";
            break;
        case 'P':
            cin >> z;
            root->modify<Add>(l, r + 1, z);
            break;
        case 'C':
            cin >> z;
            root->modify<Assign>(l, r + 1, z);
            break;
        }
    }
    return 0;
}
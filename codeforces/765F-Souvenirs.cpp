#ifdef APTEST
#include "debug_tools/program.h"
#endif
#include <vector>
#include <algorithm>
#include <iterator>
#include <climits>
#include <iostream>
using namespace std;
const unsigned int maxn = 100000, maxq = 300000;
constexpr unsigned long long ullInf = ULLONG_MAX;

template <class T, class Pred>
class SegmentTree
{
public:
    template <T ini>
    void build(const unsigned int l, const unsigned int r, SegmentTree*(&cur));
    T at(const unsigned int pos) const;
    T extremum(const unsigned int l, const unsigned int r) const;
    void update(const unsigned int l, const unsigned int r, const T val);

private:
    static const Pred prd;
    unsigned int lv, mv, rv;
    SegmentTree *lc, *rc;
    T val, laz;
};
struct Query
{
    unsigned int l, r;
    unsigned long long ans;
} qry[maxq + 10];
struct PartQuery
{
    unsigned int l;
    Query* ret;
};
unsigned long long w[maxn + 10], orig[maxn + 10];
vector<PartQuery> seq[maxn + 10];

template <class T, class Pred>
template <T ini>
void SegmentTree<T, Pred>::build(const unsigned int l, const unsigned int r, SegmentTree*(&cur))
{
    lv = l;
    rv = r;
    mv = (l + r) >> 1;
    val = laz = ini;
    if (r - l == 1)
        return;
    lc = ++cur;
    lc->build<ini>(l, mv, cur);
    rc = ++cur;
    rc->build<ini>(mv, r, cur);
}
template<class T, class Pred> 
T SegmentTree<T, Pred>::at(const unsigned int pos) const
{
    if (rv - lv == 1)
        return val;
    return min((pos < mv? lc : rc)->at(pos), laz, prd);
}
template<class T, class Pred>
T SegmentTree<T, Pred>::extremum(const unsigned int l, const unsigned int r) const
{
    if(l == lv && r == rv)
        return val;
    if(r <= mv)
        return min(lc->extremum(l, r), laz, prd);
    else if(l >= mv)
        return min(rc->extremum(l, r), laz, prd);
    else
        return min({lc->extremum(l, mv), rc->extremum(mv, r), laz}, prd);
}
template<class T, class Pred>
void SegmentTree<T, Pred>::update(const unsigned int l, const unsigned int r, const T val)
{
    if(l == lv && r == rv)
    {
        laz = min(laz, val, prd);
        this->val = min(this->val, val, prd);
        return;
    }
    if(r <= mv)
        lc->update(l, r, val);
    else if(l >= mv)
        rc->update(l, r, val);
    else
    {
        lc->update(l, mv, val);
        rc->update(mv, r, val);
    }
    this->val = min({lc->val, rc->val, laz}, prd);
}
template<class T, class Seg, T ini>
Seg* buildTree(const unsigned int l, const unsigned int r, Seg buf[])
{
    Seg* c = buf, *root = buf;
    root->template build<ini>(l, r, c);
    return root;
}

static unsigned int discretize(const unsigned int n)
{
    copy(w + 1, w + n + 1, orig);
    sort(orig, orig + n);
    unsigned long long* cur = unique(orig, orig + n);
    for (unsigned long long* i = w + 1; i < w + 1 + n; ++i)
        *i = lower_bound(orig, cur, *i) - orig;
    return cur - orig;
}
void answerPart(const unsigned int n, const unsigned int dcnt)
{
    typedef SegmentTree<unsigned int, greater<unsigned int>> idxTree;
    typedef SegmentTree<unsigned long long, less<unsigned long long>> ansTree;
    static idxTree idxNodes[maxn * 4 + 10];
    static ansTree ansNodes[maxn * 4 + 10];
    idxTree *const idxRoot = buildTree<unsigned int, idxTree, 0>(0, dcnt, idxNodes);
    ansTree *const ansRoot = buildTree<unsigned long long, ansTree, ullInf>(0, n + 1, ansNodes);
    for(unsigned int i = 1; i <= n; ++i)
    {
        {
            unsigned int l = 0;
            while(l < w[i])
            {
                const unsigned int p = idxRoot->extremum(l, w[i] + 1);
                if(!p)
                    break;
                ansRoot->update(1, p + 1, orig[w[i]] - orig[w[p]]);
                l = lower_bound(orig, orig + dcnt, (orig[w[i]] + orig[w[p]] + 1) / 2) - orig;
            }
            if(const unsigned int p = idxRoot->at(w[i]); p)
                ansRoot->update(1, p + 1, 0);
            idxRoot->update(w[i], w[i] + 1, i);
        }
        for(auto i : seq[i])
            i.ret->ans = min(i.ret->ans, ansRoot->at(i.l));
        seq[i].clear();
    }
}
void answerQuery(const unsigned int n, const unsigned int q)
{
    const unsigned int dcnt = discretize(n);
    for(Query *i = qry; i != qry + q; ++i)
        seq[i->r].push_back(PartQuery{i->l, i});
    answerPart(n, dcnt);
    reverse(w + 1, w + 1 + n);
    for(Query *i = qry; i != qry + q; ++i)
        seq[n + 1 - i->l].push_back(PartQuery{n + 1 - i->r, i});
    answerPart(n, dcnt);
}

int main()
{
#ifndef APTEST
    ios_base::sync_with_stdio(false);
#endif
    unsigned int n, q;
    cin >> n;
    copy_n(istream_iterator<unsigned long long>(cin), n, w + 1);
    cin >> q;
    for(Query *i = qry; i < qry + q; ++i)
    {
        cin >> i->l >> i->r;
        i->ans = ullInf;
    }
    answerQuery(n, q);
    for(const Query *i = qry; i < qry + q; ++i)
        cout << i->ans << "\n";
    return 0;
}

#ifdef APTEST
#include "debug_tools/program.h"
#endif
#include <algorithm>
#include <climits>
#include <cstring>
#include <iostream>
#include <numeric>
#include <queue>
using namespace std;
const unsigned int maxsn = 5e4, maxm = 5e4, maxc = 26, maxl = 21, maxs = 5e5, maxq = 5e5, maxt = 5e4;
constexpr unsigned int maxNode = (maxsn + maxs) * 2 + 1;

struct ValFrom
{
    unsigned int val, from;
    inline bool operator<(const ValFrom& r) const { return val != r.val ? val < r.val : from > r.from; }
};
constexpr ValFrom vfInf { 0, UINT_MAX };
class SegmentTree
{
public:
    void increase(const unsigned int pos);
    void merge(SegmentTree* const other);
    ValFrom max(const unsigned int l, const unsigned int r) const;

    static SegmentTree nodes[maxNode * 17 + 10], *cur;
    static inline SegmentTree* create(const unsigned int l, const unsigned int r);

private:
    inline void pushup();

    unsigned int lv, mv, rv;
    SegmentTree *lc, *rc;
    ValFrom val;
};
SegmentTree SegmentTree::nodes[maxNode * 17 + 10], *SegmentTree::cur = SegmentTree::nodes;
unsigned int m;
void SegmentTree::increase(const unsigned int pos)
{
    if (rv - lv == 1)
    {
        ++val.val;
        return;
    }
    static const auto orCreate = [](SegmentTree*& p, const unsigned int l, const unsigned int r) {
        return p ? p : p = create(l, r);
    };
    if (pos < mv)
        orCreate(lc, lv, mv)->increase(pos);
    else
        orCreate(rc, mv, rv)->increase(pos);
    pushup();
}
void SegmentTree::merge(SegmentTree* const other)
{
    if (!other)
        return;
    if (rv - lv == 1)
    {
        val.val += other->val.val;
        return;
    }
    static const auto merg = [](SegmentTree* chl, SegmentTree* other) {
        if (!chl)
            return other;
        chl->merge(other);
        return chl;
    };
    lc = merg(lc, other->lc);
    rc = merg(rc, other->rc);
    pushup();
}
ValFrom SegmentTree::max(const unsigned int l, const unsigned int r) const
{
    if (l == lv && r == rv)
        return val;
    static const auto maybe = [](SegmentTree* p, const unsigned int l, const unsigned int r) {
        return p ? p->max(l, r) : ValFrom { 0, l };
    };
    if (r <= mv)
        return maybe(lc, l, r);
    else if (l >= mv)
        return maybe(rc, l, r);
    else
        return std::max(maybe(lc, l, mv), maybe(rc, mv, r));
}
inline SegmentTree* SegmentTree::create(const unsigned int l, const unsigned int r)
{
    SegmentTree* const ret = cur++;
    ret->lv = l;
    ret->rv = r;
    ret->mv = (l + r) >> 1;
    ret->lc = ret->rc = nullptr;
    ret->val = ValFrom { 0, l };
    return ret;
}
inline void SegmentTree::pushup()
{
    val = ValFrom { 0, lv };
    if (lc)
        val = std::max(val, lc->val);
    if (rc)
        val = std::max(val, rc->val);
}
namespace GSAM
{
    struct SAMNode
    {
        unsigned int len, dep;
        SAMNode *link, *next[maxc];
        SegmentTree* count;
    } nodes[maxNode + 10] = {}, *cur = nodes + 1, * const root = nodes;
    SAMNode* seq[maxNode + 10]; // reversed topological order

    void insertTrie(const char str[], const size_t len, const unsigned int sid)
    {
        SAMNode* ptr = root;
        for (const char* i = str; i < str + len; ++i)
        {
            const unsigned int ch = *i - 'a';
            if (!ptr->next[ch])
            {
                cur->count = SegmentTree::create(0, m + 1);
                ptr->next[ch] = cur++;
            }
            ptr->count->increase(sid);
            ptr = ptr->next[ch];
        }
        ptr->count->increase(sid);
    }
    void insertSAM(SAMNode* const x, const unsigned int ch, SAMNode* last)
    {
        x->len = last->len + 1;
        SAMNode* from = last->link;
        while (from && !from->next[ch])
        {
            from->next[ch] = x;
            from = from->link;
        }
        if (!from)
            x->link = root;
        else
        {
            SAMNode* const fNxt = from->next[ch];
            if (fNxt->len == from->len + 1)
                x->link = fNxt;
            else
            {
                SAMNode* const cloned = cur++;
                new (cloned) SAMNode { from->len + 1, 0, fNxt->link, {}, SegmentTree::create(0, m + 1) };
                for (unsigned int i = 0; i < maxc; ++i)
                    if (fNxt->next[i] && fNxt->next[i]->len)
                        cloned->next[i] = fNxt->next[i];
                for (SAMNode* i = from; i && i->next[ch] == fNxt; i = i->link)
                    i->next[ch] = cloned;
                fNxt->link = x->link = cloned;
            }
        }
    }
    void buildGSAM()
    {
        queue<pair<SAMNode*, unsigned int>> q;
        for (unsigned int i = 0; i < maxc; ++i)
            if (root->next[i])
                q.emplace(root, i);
        while (!q.empty())
        {
            const auto cur = q.front();
            q.pop();
            SAMNode* const ptr = cur.first->next[cur.second];
            insertSAM(ptr, cur.second, cur.first);
            for (unsigned int i = 0; i < maxc; ++i)
                if (ptr->next[i])
                    q.emplace(ptr, i);
        }
    }
    void topological()
    {
        const unsigned int cnt = cur - nodes - 1;
        iota(seq, seq + cnt, nodes + 1);
        sort(seq, seq + cnt, [](SAMNode* a, SAMNode* b) { return a->len > b->len; });
        for (unsigned int i = cnt; i; --i)
        {
            SAMNode* const ptr = seq[i - 1];
            ptr->dep = ptr->link->dep + 1;
        }
    }
}
namespace Substr
{
    unsigned int lg[maxNode + 10], fa[maxNode + 10][maxl + 1], begin[maxs + 10];

    static void getLog(const unsigned int n)
    {
        for (unsigned int i = 2; i <= n; ++i)
            lg[i] = lg[i >> 1] + 1;
    }
    static void initBegin(const char s0[], const size_t len)
    {
        using GSAM::SAMNode;
        const SAMNode* ptr = GSAM::root;
        for (unsigned int i = 0; i < len; ++i)
        {
            ptr = ptr->next[s0[i] - 'a'];
            begin[i] = ptr - GSAM::nodes;
        }
    }
    static void initFa(const unsigned int n)
    {
        using GSAM::SAMNode, GSAM::nodes;
        for (const SAMNode* i = nodes + 1; i < nodes + n + 1; ++i)
            fa[i - nodes][0] = i->link - nodes;
        for (unsigned int i = 1; i <= lg[n]; ++i)
            for (unsigned int j = 1; j <= n; ++j)
                fa[j][i] = fa[fa[j][i - 1]][i - 1];
    }
    static void init(const unsigned int n, const char s0[], const size_t len)
    {
        getLog(n);
        initBegin(s0, len);
        initFa(n);
    }
    unsigned int find(const unsigned int r, const unsigned int l)
    {
        using GSAM::nodes;
        const unsigned int len = r + 1 - l;
        unsigned int x = begin[r];
        for (unsigned int i = lg[nodes[x].dep] + 1; i; --i)
        {
            const unsigned int fp = fa[x][i - 1];
            if (nodes[fp].len >= len)
                x = fp;
        }
        return x;
    }
}
struct Query
{
    unsigned int l, r;
    ValFrom ans;
} que[maxq + 10], *cur = que;
vector<Query*> qry[maxNode + 10];

static inline void init(const char s0[])
{
    const size_t l = strlen(s0);
    GSAM::insertTrie(s0, l, 0);
    GSAM::buildGSAM();
    GSAM::topological();
    Substr::init(GSAM::cur - GSAM::nodes - 1, s0, l);
}
void answerQuery()
{
    using GSAM::seq;
    const unsigned int cnt = GSAM::cur - 1 - GSAM::nodes;
    for (unsigned int i = 0; i < cnt; ++i)
    {
        const GSAM::SAMNode* const ptr = seq[i];
        SegmentTree* const root = ptr->count;
        for (auto i : qry[ptr - GSAM::nodes])
            i->ans = root->max(i->l, i->r);
        ptr->link->count->merge(root);
    }
}
void addQuery(const unsigned int l, const unsigned int r, const unsigned int pl, const unsigned int pr)
{
    new (cur) Query { l, r + 1, {} };
    qry[Substr::find(pr - 1, pl - 1)].push_back(cur++);
}

int main()
{
#ifndef APTEST
    ios_base::sync_with_stdio(false);
#endif
    static char s0[maxs + 10];
    unsigned int q;
    cin >> s0 >> m;
    GSAM::root->count = SegmentTree::create(0, m + 1);
    for (unsigned int i = 1; i <= m; ++i)
    {
        static char str[maxt + 10];
        cin >> str;
        GSAM::insertTrie(str, strlen(str), i);
    }
    init(s0);
    cin >> q;
    for (unsigned int i = 0; i < q; ++i)
    {
        unsigned int l, r, pl, pr;
        cin >> l >> r >> pl >> pr;
        addQuery(l, r, pl, pr);
    }
    answerQuery();
    for (const Query* i = que; i < cur; ++i)
        cout << i->ans.from << " " << i->ans.val << "\n";
    return 0;
}
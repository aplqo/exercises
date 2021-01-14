#ifdef APTEST
#include "debug_tools/program.h"
#endif
#if !defined APTEST && !defined APDEBUG
#define NDEBUG
#endif
#include <algorithm>
#include <cassert>
#include <cmath>
#include <cstring>
#include <iostream>
#include <iterator>
#include <numeric>
#include <vector>
using namespace std;
const unsigned int maxn = 1e5, maxl = 20, maxc = 26, maxk = 1e5, maxm = 1e5, maxq = 1e5;

struct Range
{
    unsigned int l, r, length;
} rng[maxm + 10];
namespace SAM
{
    struct Node
    {
        unsigned int length, count, depth;
        Node *next[maxc], *link;
    } nodes[maxn * 2 + 10] = {}, *root = nodes, *last = root, *cur = nodes + 1;
    struct NodePtr
    {
        const Node* ptr;
        unsigned int valid;
    };
    unsigned int fa[maxn * 2 + 10][maxl + 1], lg[maxn * 2 + 19];

    void insertChar(const unsigned int c)
    {
        Node *from = last, *const ptr = cur++;
        new (ptr) Node { last->length + 1, 1, 0, {}, nullptr };
        while (from && !from->next[c])
        {
            from->next[c] = ptr;
            from = from->link;
        }
        if (!from)
            ptr->link = root;
        else
        {
            Node* const fNxt = from->next[c];
            if (fNxt->length == from->length + 1)
                ptr->link = fNxt;
            else
            {
                Node* const cloned = cur++;
                *cloned = *fNxt;
                cloned->length = from->length + 1;
                cloned->count = 0;
                for (Node* i = from; i && i->next[c] == fNxt; i = i->link)
                    i->next[c] = cloned;
                fNxt->link = ptr->link = cloned;
            }
        }
        last = ptr;
    }
    static void initLg(const unsigned int n)
    {
        for (unsigned int i = 2; i <= n; ++i)
            lg[i] = lg[i >> 1] + 1;
    }
    static void init()
    {
        static Node* ptr[maxn * 2 + 10];
        const unsigned int n = cur - nodes;
        iota(ptr, ptr + n, nodes);
        sort(ptr, ptr + n, [](Node* a, Node* b) { return a->length < b->length; });
        for (unsigned int i = 1; i < n; ++i)
        {
            Node* const cptr = ptr[i];
            cptr->depth = cptr->link->depth + 1;
            const unsigned int lim = lg[cptr->depth + 1], cid = cptr - nodes;
            fa[cid][0] = cptr->link - nodes;
            for (unsigned int j = 1; j <= lim; ++j)
                fa[cid][j] = fa[fa[cid][j - 1]][j - 1];
        }
        for (unsigned int i = n - 1; i; --i)
        {
            Node* const cur = ptr[i];
            cur->link->count += cur->count;
        }
    }
    void buildSAM(const char* str)
    {
        const size_t len = strlen(str);
        for (const char* i = str; i < str + len; ++i)
            insertChar(*i - 'a');
        initLg(cur - nodes);
        init();
    }
    unsigned long long countSuffix(const NodePtr r, const unsigned int len)
    {
        assert(r.valid >= len);
        unsigned int cid = r.ptr - nodes;
        for (unsigned int i = lg[nodes[cid].length] + 1; i; --i)
        {
            const unsigned int fp = fa[cid][i - 1];
            if (nodes[fp].length >= len)
                cid = fp;
        }
        return nodes[cid].count;
    }
    void run(const char* str, const unsigned int k, NodePtr dest[])
    {
        static constexpr NodePtr nil { nullptr, 0 };
        const Node* cur = root;
        unsigned int valid = 0;
        for (unsigned int i = 0; i < k; ++i)
        {
            const unsigned int ch = str[i] - 'a';
            while (!cur->next[ch])
            {
                cur = cur->link;
                if (cur)
                    valid = cur->length;
                else
                    break;
            }
            if (!cur)
            {
                cur = root;
                valid = 0;
                dest[i] = nil;
            }
            else
            {
                cur = cur->next[ch];
                ++valid;
                dest[i] = NodePtr { cur, valid };
            }
        }
    }
} // namespace SAM
namespace LessK
{
    const unsigned int maxk = 500;
    vector<unsigned int> pos[maxk + 10][maxk + 10];
    unsigned int substr[maxk + 10][maxk + 10];

    static void init(const unsigned int m)
    {
        for (unsigned int i = 0; i < m; ++i)
            pos[rng[i].l][rng[i].r].push_back(i);
    }
    unsigned long long count(const char* str, const unsigned int k, const unsigned int a, const unsigned int b)
    {
        static SAM::NodePtr ptr[maxq + 10];
        SAM::run(str, k, ptr);
        unsigned long long ret = 0;
        for (unsigned int i = 0; i < k; ++i)
            for (unsigned int j = i + 1 - ptr[i].valid; j <= i; ++j)
                ret += SAM::countSuffix(ptr[i], i + 1 - j) * (ranges::lower_bound(pos[j][i], b) - ranges::lower_bound(pos[j][i], a));
        return ret;
    }
} // namespace LessK
namespace LessQ
{
    unsigned long long count(const char* str, const unsigned int k, const unsigned int a, const unsigned int b)
    {
        static SAM::NodePtr ptr[maxq + 10];
        SAM::run(str, k, ptr);
        unsigned long long ret = 0;
        for (const Range* i = rng + a; i < rng + b; ++i)
            if (i->length <= ptr[i->r].valid)
                ret += SAM::countSuffix(ptr[i->r], i->length);
        return ret;
    }
}

int main()
{
#ifndef APTEST
    ios::sync_with_stdio(false);
#endif
    unsigned int n, m, k, q;
    cin >> n >> m >> q >> k;
    static char s[maxn + 10], str[maxn + 10];
    cin >> s;
    SAM::buildSAM(s);
    for (Range* i = rng; i < rng + m; ++i)
    {
        cin >> i->l >> i->r;
        i->length = i->r + 1 - i->l;
    }
    const auto count = k > q ? LessQ::count : LessK::count;
    if (k <= q)
        LessK::init(m);
    for (unsigned int i = q; i; --i)
    {
        unsigned int a, b;
        cin >> str >> a >> b;
        cout << count(str, k, a, b + 1) << "\n";
    }
    return 0;
}
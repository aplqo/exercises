#ifdef APTEST
#include "debug_tools/judge.h"
#endif
#include <algorithm>
#include <iostream>
#include <iterator>
#include <queue>
const unsigned int maxn = 5e5, maxl = 31;

struct ValFrom
{
    unsigned int val, from;
};
unsigned int a[maxn + 10];
namespace Trie
{
    struct Node
    {
        unsigned int size, from;
        const Node* chl[2];
    } nodes[(maxn + 10) * (maxl + 2) + 10], *cur = nodes;
    const Node nil { 0, 0, { &nil, &nil } }, *root[maxn + 10] = { &nil, &nil };

    void insert(const unsigned int cid, const unsigned int v)
    {
        const Node* old = root[cid - 1];
        Node* ptr = cur++;
        root[cid] = ptr;
        for (unsigned int i = 1u << maxl; i; i >>= 1)
        {
            const unsigned int b = v & i ? 1 : 0;
            ptr->size = old->size + 1;
            ptr->chl[!b] = old->chl[!b];
            old = old->chl[b];
            ptr->chl[b] = cur;
            ptr = cur++;
        }
        ptr->size = old->size + 1;
        ptr->from = cid;
    }
    ValFrom maxXor(const unsigned int l, const unsigned int r, const unsigned int v)
    {
        const Node *pl = root[l], *pr = root[r];
        unsigned int ret = 0;
        for (unsigned int i = 1u << maxl; i; i >>= 1)
        {
            unsigned int b = v & i ? 1 : 0;
            if (pr->chl[!b]->size > pl->chl[!b]->size)
            {
                ret |= i;
                b = !b;
            }
            pl = pl->chl[b];
            pr = pr->chl[b];
        }
        return ValFrom { ret, pr->from };
    }
}
struct Node
{
    unsigned int right;
    unsigned int leftMin, leftMax;
    ValFrom max;
};
struct NodeLess
{
    inline bool operator()(const Node& l, const Node& r)
    {
        if (l.max.val != r.max.val)
            return l.max.val < r.max.val;
        else if (l.max.from != r.max.from)
            return l.max.from < r.max.from;
        else if (l.right != r.right)
            return l.right < r.right;
        else if (l.leftMin != r.leftMin)
            return l.leftMin < r.leftMin;
        else
            return l.leftMax < r.leftMax;
    }
};

Node getMax(const unsigned int ll, const unsigned int lr, const unsigned int right)
{
    ValFrom fr = Trie::maxXor(ll - 2, lr - 1, a[right]);
    return Node { right, ll, lr, ValFrom { fr.val, fr.from + 1 } };
}
unsigned long long greedy(const unsigned int n, unsigned int k)
{
    std::priority_queue<Node, std::vector<Node>, NodeLess> q;
    for (unsigned int i = 2; i <= n + 1; ++i)
    {
        Trie::insert(i, a[i]);
        q.push(getMax(2, i, i));
    }
    unsigned long long ret = 0;
    for (unsigned int i = k; i; --i)
    {
        const Node tp = q.top();
        q.pop();
        ret += tp.max.val;
        if (tp.leftMin + 1 <= tp.max.from)
            q.push(getMax(tp.leftMin, tp.max.from - 1, tp.right));
        if (tp.max.from + 1 <= tp.leftMax)
            q.push(getMax(tp.max.from + 1, tp.leftMax, tp.right));
    }
    return ret;
}

int main()
{
    std::ios::sync_with_stdio(false);
    unsigned int n, k;
    std::cin >> n >> k;
    for (unsigned int i = 2; i <= n + 1; ++i)
    {
        unsigned int v;
        std::cin >> v;
        a[i] = a[i - 1] ^ v;
    }
    std::cout << greedy(n + 1, k) << "\n";
    return 0;
}
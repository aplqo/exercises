#ifdef APTEST
#include "debug_tools/program.h"
#endif
#include <algorithm>
#include <cstdio>
#include <vector>

#include <iostream>

using namespace std;
const unsigned int maxl = 32, maxn = 1e6;

struct Node
{
    bool end;
    unsigned int sta, dep;
    Node* chl[2];
} nod[maxl * maxn + 10];
Node root {};

void insert(const unsigned int ip, const unsigned int len)
{
    static Node* cur = nod;
    static unsigned int sta = 0;
    Node* ptr = &root;
    for (unsigned int i = 0, msk = 1 << (maxl - 1); i < len; ++i, msk >>= 1)
    {
        const bool typ = ip & msk;
        if (!ptr->chl[typ])
        {
            new (cur) Node {};
            cur->dep = ptr->dep + 1;
            ptr->chl[typ] = cur++;
        }
        ptr = ptr->chl[typ];
    }
    ptr->end = true;
    ptr->sta = ++sta;
}
vector<const Node*> match(const unsigned int ip)
{
    vector<const Node*> ret;
    ret.reserve(32);
    Node* ptr = &root;
    for (unsigned int msk = 1 << (maxl - 1); ptr; msk >>= 1)
    {
        const bool typ = ip & msk;
        if (ptr->end)
            ret.push_back(ptr);
        ptr = ptr->chl[typ];
    }
    return ret;
}
unsigned int query(const unsigned int ip, const unsigned int a, const unsigned int b)
{
    vector<const Node*> v = match(ip);
    sort(v.begin(), v.end(), [](const Node* a, const Node* b) { return a->sta < b->sta; });
    unsigned int l = 0, ret = 0;
    for (auto it = v.cbegin(); it != v.cend(); ++it)
    {
        const Node* ptr = *it;
        if (ptr->sta < a)
            l = max(l, ptr->dep);
        else if (ptr->sta <= b && ptr->dep > l)
        {
            l = ptr->dep;
            ++ret;
        }
    }
    return ret;
}

int main()
{
    unsigned int m;
    scanf("%u", &m);
    for (unsigned int i = 0; i < m; ++i)
    {
        static char op[3];
        scanf("%s", op);
        if (op[0] == 'A')
        {
            unsigned int a, b, c, d, msk;
            scanf("%u.%u.%u.%u/%u", &a, &b, &c, &d, &msk);
            insert((a << 24) | (b << 16) | (c << 8) | d, msk);
        }
        else
        {
            unsigned int a, b, c, d, l, r;
            scanf("%u.%u.%u.%u%u%u", &a, &b, &c, &d, &l, &r);
            printf("%u\n", query((a << 24) | (b << 16) | (c << 8) | d, l, r));
        }
    }
    return 0;
}
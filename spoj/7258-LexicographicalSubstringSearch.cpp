#ifdef APTEST
#include "debug_tools/program.h"
#endif
#include <algorithm>
#include <cstring>
#include <iostream>
using namespace std;
const unsigned int maxn = 5e5, maxc = 26;

struct SAMNode
{
    SAMNode *link, *next[maxc + 10];
    unsigned long long count, size;
    unsigned int len;
    bool isClone;
} nodes[maxn * 2 + 10], *cur = nodes, root {};

void insertChar(const unsigned int ch)
{
    static SAMNode* last = &root;
    SAMNode* const ptr = cur++;
    new (ptr) SAMNode { nullptr, {}, 0, 0, last->len + 1, false };
    SAMNode* from = last;
    while (from && !from->next[ch])
    {
        from->next[ch] = ptr;
        from = from->link;
    }
    if (!from)
        ptr->link = &root;
    else
    {
        SAMNode* const fNxt = from->next[ch];
        if (fNxt->len == from->len + 1)
            ptr->link = fNxt;
        else
        {
            SAMNode* const cloned = cur++;
            *cloned = *fNxt;
            cloned->len = from->len + 1;
            cloned->isClone = true;
            for (SAMNode* i = from; i && i->next[ch] == fNxt; i = i->link)
                i->next[ch] = cloned;
            fNxt->link = ptr->link = cloned;
        }
    }
    last = ptr;
}
void buildSAM()
{
    static SAMNode* seq[maxn * 2 + 10];
    const unsigned int cnt = cur - nodes;
    for (unsigned int i = 0; i < cnt; ++i)
        seq[i] = nodes + i;
    sort(seq, seq + cnt, [](const SAMNode* a, const SAMNode* b) { return a->len > b->len; });

    for_each(nodes, cur, [](SAMNode& i) { i.size = 1; });
    for (unsigned int i = 0; i < cnt; ++i)
    {
        SAMNode* const p = seq[i];
        for (auto i : p->next)
            if (i)
                p->count += i->count + i->size;
    }
    for (auto i : root.next)
        if (i)
            root.count += i->count + i->size;
}
void kthSustr(unsigned long long k)
{
    if (root.count < k)
    {
        cout << "-1\n";
        return;
    }
    SAMNode* ptr = &root;
    while (k)
    {
        SAMNode** const nxt = ptr->next;
        for (unsigned int i = 0; i < maxc; ++i)
        {
            if (!nxt[i])
                continue;
            k -= nxt[i]->size;
            if (nxt[i]->count >= k)
            {
                ptr = nxt[i];
                cout.put('a' + i);
                break;
            }
            else
                k -= nxt[i]->count;
        }
    }
    cout.put('\n');
}

int main()
{
#ifndef APTEST
    ios_base::sync_with_stdio(false);
#endif
    static char str[maxn + 10];
    cin >> str;
    {
        const size_t len = strlen(str);
        for (const char* i = str; i < str + len; ++i)
            insertChar(*i - 'a');
    }
    buildSAM();
    unsigned int t;
    cin >> t;
    for (unsigned int i = t; i; --i)
    {
        unsigned int k;
        cin >> k;
        kthSustr(k);
    }
    return 0;
}
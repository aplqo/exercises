#ifdef APTEST
#include "debug_tools/program.h"
#endif
#include <algorithm>
#include <cstring>
#include <iostream>
#include <queue>
#include <unordered_map>
using namespace std;
const unsigned int maxn = 200000, maxc = 26;

struct SAMNode
{
    unsigned int count[2];
    unsigned int len;
    SAMNode* link;
    unordered_map<unsigned int, SAMNode*> next;
} nodes[maxn * 2 * 2 + 10], *cur = nodes, root {};

void insertTrie(const char* str, const unsigned int id)
{
    const size_t len = strlen(str);
    SAMNode* ptr = &root;
    for (const char* i = str; i < str + len; ++i)
    {
        const unsigned int ch = *i - 'a';
        auto it = ptr->next.find(ch);
        if (it == ptr->next.end())
        {
            cur->next.~unordered_map();
            new (cur) SAMNode {};
            it = ptr->next.insert({ ch, cur++ }).first;
        }
        ptr->count[id] = 1;
        ptr = it->second;
    }
    ptr->count[id] = 1;
}
void insertSAM(SAMNode* const x, const unsigned int ch, SAMNode* const last)
{
    SAMNode* from = last->link;
    x->len = last->len + 1;
    while (from && !from->next.count(ch))
    {
        from->next[ch] = x;
        from = from->link;
    }
    if (!from)
        x->link = &root;
    else
    {
        SAMNode* const fNxt = from->next[ch];
        if (fNxt->len == from->len + 1)
            x->link = fNxt;
        else
        {
            SAMNode* const cloned = cur++;

            cloned->link = fNxt->link;
            cloned->len = from->len + 1;
            cloned->count[0] = cloned->count[1] = 0;
            for (auto i : fNxt->next)
                if (i.second->len)
                    cloned->next.insert(i);
            for (SAMNode* i = from; i; i = i->link)
                if (const auto it = i->next.find(ch); it != i->next.end() && it->second == fNxt)
                    it->second = cloned;
                else
                    break;
            fNxt->link = x->link = cloned;
        }
    }
}
void buildGSAM()
{
    queue<pair<SAMNode*, unsigned int>> q;
    for (auto i : root.next)
        q.emplace(&root, i.first);
    while (!q.empty())
    {
        const auto cur = q.front();
        q.pop();
        SAMNode* const ptr = cur.first->next[cur.second];
        insertSAM(ptr, cur.second, cur.first);
        for (auto i : ptr->next)
            q.emplace(ptr, i.first);
    }
}
unsigned long long count()
{
    static SAMNode* seq[maxn * 2 * 2 + 10];
    unsigned long long ret = 0;
    const unsigned int cnt = cur - nodes;
    for (unsigned int i = 0; i < cnt; ++i)
        seq[i] = nodes + i;
    sort(seq, seq + cnt, [](const SAMNode* a, const SAMNode* b) { return a->len > b->len; });
    for (unsigned int i = 0; i < cnt; ++i)
    {
        SAMNode* const p = seq[i];
        p->link->count[0] += p->count[0];
        p->link->count[1] += p->count[1];
        ret += static_cast<unsigned long long>(p->count[0]) * p->count[1] * (p->len - p->link->len);
    }
    return ret;
}

int main()
{
#ifndef APTEST
    ios_base::sync_with_stdio(false);
#endif
    static char str[maxn + 19];
    cin >> str;
    insertTrie(str, 0);
    cin >> str;
    insertTrie(str, 1);
    buildGSAM();
    cout << count() << "\n";
    return 0;
}
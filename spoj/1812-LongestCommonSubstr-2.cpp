#ifdef APTEST
#include "debug_tools/program.h"
#endif
#include <algorithm>
#include <cstring>
#include <iostream>
#include <queue>
#include <unordered_map>
#include <utility>
using namespace std;
const unsigned int maxsn = 1e6, maxk = 11, maxc = 26;

struct SAMNode
{
    SAMNode *link = nullptr, *next[maxc] {};
    unsigned int len = 0, strMask = 0;
} nodes[maxsn * 2 + 10], *cur = nodes, root {};

void insertTrie(const char* str, const unsigned int id)
{
    const size_t len = strlen(str);
    SAMNode* ptr = &root;
    for (const char* i = str; i < str + len; ++i)
    {
        const unsigned int ch = *i - 'a';
        if (!ptr->next[ch])
            ptr->next[ch] = cur++;
        ptr->strMask |= id;
        ptr = ptr->next[ch];
    }
    ptr->strMask |= id;
}
void insertChar(SAMNode* const x, const unsigned int ch, SAMNode* const last)
{
    SAMNode* from = last->link;
    x->len = last->len + 1;
    while (from && !from->next[ch])
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
            SAMNode* cloned = cur++;
            cloned->link = fNxt->link;
            cloned->len = from->len + 1;
            cloned->strMask = fNxt->strMask;
            for (unsigned int i = 0; i < maxc; ++i)
                if (fNxt->next[i] && fNxt->next[i]->len)
                    cloned->next[i] = fNxt->next[i];
            for (SAMNode* i = from; i; i = i->link)
                if (i->next[ch] && i->next[ch] == fNxt)
                    i->next[ch] = cloned;
                else
                    break;
            fNxt->link = x->link = cloned;
        }
    }
}
void buildGSAM()
{
    queue<pair<SAMNode*, unsigned int>> q;
    for (unsigned int i = 0; i < maxc; ++i)
        if (root.next[i])
            q.emplace(&root, i);
    while (!q.empty())
    {
        const auto cur = q.front();
        q.pop();
        SAMNode* const ptr = cur.first->next[cur.second];
        insertChar(ptr, cur.second, cur.first);
        for (unsigned int i = 0; i < maxc; ++i)
            if (ptr->next[i])
                q.emplace(ptr, i);
    }
}
unsigned int longestCommon(const unsigned int strCnt)
{
    static SAMNode* seq[maxsn * 2 + 10];
    const unsigned int cnt = cur - nodes, fullMask = (1u << strCnt) - 1;
    for (SAMNode* i = nodes; i < cur; ++i)
        seq[i - nodes] = i;
    sort(seq, seq + cnt, [](const SAMNode* a, const SAMNode* b) { return a->len > b->len; });
    for (unsigned int i = 0; i < cnt; ++i)
    {
        SAMNode* const p = seq[i];
        if (p->strMask == fullMask)
            return p->len;
        p->link->strMask |= p->strMask;
    }
    return 0;
}

int main()
{
#ifndef APTEST
    ios_base::sync_with_stdio(false);
#endif
    unsigned int id = 0;
    while (cin)
    {
        static char str[maxsn + 10];
        cin >> str;
        insertTrie(str, 1u << (id++));
    }
    buildGSAM();
    cout << longestCommon(id) << "\n";
    return 0;
}
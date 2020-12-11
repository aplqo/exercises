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
const unsigned int maxsn = 1e6;

struct SAMNode
{
    unordered_map<char, SAMNode*> next;
    SAMNode* link = nullptr;
    unsigned int len = 0;
} nodes[maxsn * 2 + 10], *cur = nodes, root {};

void insertTrie(const char* str)
{
    const size_t len = strlen(str);
    SAMNode* ptr = &root;
    for (const char* i = str; i < str + len; ++i)
    {
        auto it = ptr->next.find(*i);
        if (it == ptr->next.end())
            it = ptr->next.insert({ *i, cur++ }).first;
        ptr = it->second;
    }
}
void insertChar(SAMNode* const x, const char ch, SAMNode* const last)
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
        SAMNode* const fNxt = from->next.at(ch);
        if (fNxt->len == from->len + 1)
            x->link = fNxt;
        else
        {
            SAMNode* cloned = cur++;
            cloned->link = fNxt->link;
            cloned->len = from->len + 1;
            for (auto i : fNxt->next)
                if (i.second->len)
                    cloned->next.insert(i);
            for (SAMNode* i = from; i; i = i->link)
            {
                const auto it = i->next.find(ch);
                if (it != i->next.end() && it->second == fNxt)
                    it->second = cloned;
                else
                    break;
            }
            fNxt->link = x->link = cloned;
        }
    }
}
void buildGSAM()
{
    queue<pair<SAMNode*, char>> q;
    for (auto i : root.next)
        q.emplace(&root, i.first);
    while (!q.empty())
    {
        const auto cur = q.front();
        q.pop();
        SAMNode* const ptr = cur.first->next[cur.second];
        insertChar(ptr, cur.second, cur.first);
        for (auto i : ptr->next)
            q.emplace(ptr, i.first);
    }
}
unsigned long long countSubstr()
{
    unsigned long long ret = 0;
    for (const SAMNode* i = nodes; i < cur; ++i)
        ret += i->len - i->link->len;
    return ret;
}

int main()
{
#ifndef APTEST
    ios_base::sync_with_stdio(false);
#endif
    unsigned int n;
    cin >> n;
    for (unsigned int i = 0; i < n; ++i)
    {
        static char str[maxsn + 10];
        cin >> str;
        insertTrie(str);
    }
    buildGSAM();
    cout << countSubstr() << "\n";
    return 0;
}
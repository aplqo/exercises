#ifdef APTEST
#include "debug_tools/program.h"
#endif
#include <algorithm>
#include <cstring>
#include <iostream>
using namespace std;
const unsigned int maxn = 300000, maxc = 26;

struct EERNode
{
    EERNode *link, *next[maxc];
    int len;
    unsigned int count;
} nodes[maxn + 10] {}, *cur = nodes;
EERNode oddRoot { &oddRoot, {}, -1, 0 }, evenRoot { &oddRoot, {}, 0, 0 }, *last = &evenRoot;
unsigned int str[maxn + 10] = { maxc + 1 };

void insertChar(const unsigned int ch, const int pos)
{
    EERNode* fa = last;
    str[pos] = ch;
    while (str[pos - 1 - fa->len] != ch)
        fa = fa->link;
    if (!fa->next[ch])
    {
        EERNode* const ptr = cur++;
        EERNode* lk = fa->link;
        while (str[pos - lk->len - 1] != ch)
            lk = lk->link;
        ptr->link = lk->next[ch] ? lk->next[ch] : &evenRoot;
        ptr->len = fa->len + 2;
        fa->next[ch] = ptr;
    }
    last = fa->next[ch];
    ++(last->count);
}
unsigned long long findMax()
{
    unsigned long long ret = 0;
    for (EERNode* i = cur - 1; i >= nodes; --i)
    {
        ret = max(ret, static_cast<unsigned long long>(i->len) * i->count);
        i->link->count += i->count;
    }
    return ret;
}

int main()
{
#ifndef APTEST
    ios_base::sync_with_stdio(false);
#endif
    static char str[maxn + 10];
    cin >> (str + 1);
    const size_t len = strlen(str + 1);
    for (unsigned int i = 1; i <= len; ++i)
        insertChar(str[i] - 'a', i);
    cout << findMax() << "\n";
    return 0;
}
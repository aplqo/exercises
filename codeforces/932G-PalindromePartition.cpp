#ifdef APTEST
#include "debug_tools/program.h"
#endif
#include <algorithm>
#include <climits>
#include <cstring>
#include <iostream>
using namespace std;
const unsigned int maxn = 1e6, maxc = 26;
constexpr unsigned long long mod = 1e9 + 7;
constexpr unsigned int inf = UINT_MAX;

struct EERNode
{
    EERNode *link, *slink, *next[maxc];
    int len;
    unsigned int diff;
    unsigned long long sum;
} nodes[maxn + 10] {}, *cur = nodes;
EERNode oddRoot { &oddRoot, nullptr, {}, -1, inf, 0ull }, evenRoot { &oddRoot, nullptr, {}, 0, inf, 0 }, *last = &evenRoot;
unsigned int str[maxn + 10] = { inf };

void insertChar(const unsigned int ch, const int pos)
{
    EERNode* fa = last;
    while (str[pos - 1 - fa->len] != ch)
        fa = fa->link;
    if (!fa->next[ch])
    {
        EERNode* const ptr = cur++;
        EERNode* lk = fa->link;
        while (str[pos - 1 - lk->len] != ch)
            lk = lk->link;
        lk = lk->next[ch] ? lk->next[ch] : &evenRoot;
        ptr->len = fa->len + 2;
        ptr->link = lk;
        ptr->diff = ptr->len - lk->len;
        ptr->slink = lk->diff == ptr->diff ? lk->slink : lk;
        fa->next[ch] = ptr;
    }
    last = fa->next[ch];
}
static size_t initArray(const char buf[])
{
    const size_t len = strlen(buf + 1);
    const char* iPtr1 = buf + 1;
    for (unsigned int* i = str + 1; i < str + len + 1; i += 2, ++iPtr1)
        *i = *iPtr1 - 'a';
    for (unsigned int* i = str + len; iPtr1 < buf + len + 1; i -= 2, ++iPtr1)
        *i = *iPtr1 - 'a';
    return len;
}
unsigned long long dp(const size_t len)
{
    static unsigned long long f[maxn + 10];
    f[0] = 1;
    for (unsigned int i = 1; i <= len; ++i)
    {
        insertChar(str[i], i);
        for (EERNode* j = last; j->len; j = j->slink)
        {
            j->sum = f[i - (j->slink->len + j->diff)];
            if (j->link->diff == j->diff)
                j->sum = (j->sum + j->link->sum) % mod;
        }
        if ((!(i & 0x01)))
            for (const EERNode* j = last; j; j = j->slink)
                f[i] = (f[i] + j->sum) % mod;
    }
    return f[len];
}

int main()
{
#ifndef APTEST
    ios_base::sync_with_stdio(false);
#endif
    static char buf[maxn + 10];
    cin >> (buf + 1);
    cout << dp(initArray(buf)) << "\n";
    return 0;
}
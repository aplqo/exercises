#ifdef APTEST
#include "debug_tools/program.h"
#endif
#include <algorithm>
#include <cstring>
#include <iostream>
#include <vector>
using namespace std;
const unsigned int maxl = 510000, maxn = 100000;

struct Node
{
    unsigned int siz, dfn;
    bool end;
    Node* chl[26];
} nod[maxl + 10] = {}, *root = nod;
vector<unsigned int> chl[maxl + 10];
unsigned int siz[maxl + 10];

void insert(const char* str, const size_t len)
{
    static Node* cur = nod + 1;
    Node* ptr = root;
    for (unsigned int i = 0; i < len; ++i)
    {
        const unsigned int c = str[i] - 'a';
        if (!ptr->chl[c])
        {
            new (cur) Node {};
            ptr->chl[c] = cur++;
        }
        ++(ptr->siz);
        ptr = ptr->chl[c];
    }
    ++(ptr->siz);
    ptr->end = true;
}
void dfsRebuild(Node* const x, const unsigned int f)
{
    if (x->end)
    {
        chl[f].push_back(x - nod);
        siz[x - nod] = x->siz;
    }
    const unsigned int nf = x->end ? x - nod : f;
    for (Node** i = x->chl; i < x->chl + 26; ++i)
        if (*i)
            dfsRebuild(*i, nf);
}
unsigned long long dfs(const unsigned int x, const unsigned int f)
{
    static unsigned int dfn[maxl + 10], sta = 0;
    dfn[x] = ++sta;
    sort(chl[x].begin(), chl[x].end(), [](const unsigned int a, const unsigned int b) { return siz[a] < siz[b]; });
    unsigned long long ret = dfn[x] - dfn[f];
    for (unsigned int to : chl[x])
        ret += dfs(to, x);
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
        static char str[maxl + 10];
        cin >> str;
        const size_t l = strlen(str);
        reverse(str, str + l);
        insert(str, l);
    }
    dfsRebuild(root, 0);
    cout << dfs(0, 0) << "\n";
    return 0;
}
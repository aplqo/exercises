#ifdef APTEST
#include "debug_tools/program.h"
#endif
#include <cstring>
#include <iostream>
using namespace std;
const unsigned int maxl = 1000000;
const unsigned long long mod = 1e9 + 7;

struct edge
{
    unsigned int to;
    const edge* pre;
} ed[maxl + 1], *cur = ed;
const edge* head[maxl + 1];
unsigned long long *sum, *unrest;
char str[maxl + 1];

void addEdge(const unsigned int from, const unsigned int to)
{
    cur->to = to;
    cur->pre = head[from];
    head[from] = cur;
    ++cur;
}
void pre(const size_t l)
{
    static unsigned int nxt[maxl + 1];
    addEdge(0, 1);
    unsigned int j = 0;
    for (unsigned int i = 1; i < l; ++i)
    {
        while (j && str[i + 1] != str[j + 1])
            j = nxt[j];
        if (str[i + 1] == str[j + 1])
            ++j;
        nxt[i + 1] = j;
        addEdge(j, i + 1);
        unrest[i + 1] = j ? unrest[j] + 1 : 0;
    }
}
void dfs(const unsigned int x)
{
    static unsigned int stk[maxl + 1], top = 0, *ptr = stk;
    unsigned int* const optr = ptr;
    stk[top++] = x;
    for (; ptr < stk + top && *ptr <= x / 2; ++ptr)
        ;
    if (*ptr > x / 2)
        --ptr;
    sum[x] = *ptr ? unrest[*ptr] + 1 : 0;
    for (const edge* i = head[x]; i; i = i->pre)
        dfs(i->to);
    --top;
    ptr = optr;
}
unsigned long long solve()
{
    const size_t l = strlen(str + 1);
    pre(l);
    dfs(0);
    unsigned long long ret = 1;
    for (unsigned int i = 1; i <= l; ++i)
        ret = (ret * (sum[i] + 1)) % mod;
    return ret;
}
int main()
{
#ifndef APTEST
    ios_base::sync_with_stdio(false);
#endif
    sum = new unsigned long long[maxl + 1];
    unrest = new unsigned long long[maxl + 1];
    unrest[0] = 1;
    unsigned int n;
    cin >> n;
    for (unsigned int i = 0; i < n; ++i)
    {
        cin >> str + 1;
        cout << solve() << endl;
        memset(head, 0, sizeof(head));
        cur = ed;
    }
    return 0;
}
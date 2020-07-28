#ifdef APTEST
#include "debug_tools/program.h"
#endif
#include <algorithm>
#include <cctype>
#include <cstdio>
#include <iostream>
#include <vector>
using namespace std;
const unsigned int maxn = 500000, maxm = 500000;

char inbuf[maxn * 10 + maxm * 10 * 2 + 1000], *inpos = inbuf, outbuf[maxm * 5 + 1000], *outpos = outbuf;
template <class T = unsigned int>
inline T read()
{
    T ret = 0;
    char c = *(inpos++);
    while (!isdigit(c))
        c = *(inpos++);
    while (isdigit(c))
    {
        ret = ret * 10 + c - '0';
        c = *(inpos++);
    }
    return ret;
}
void read(char* str)
{
    inpos += sscanf(inpos, "%s", str);
}
inline void write(const bool v)
{
    outpos += sprintf(outpos, "%s\n", (v ? "Yes" : "No"));
}
struct edge
{
    unsigned int to;
    const edge* pre;
} ed[maxn + 1];
const edge* head[maxn + 1];
struct Query
{
    unsigned int v, h;
    bool ans;
} qry[maxm + 1];
unsigned int son[maxn + 1], dep[maxn + 1] = { 0, 1 }, maxDep[maxn + 1];
vector<Query*> seq[maxn + 1];
unsigned int ch[maxn + 1], oddCnt[maxn + 1];
bool odd[maxn + 1][26];

void addEdge(const unsigned int from, const unsigned int to)
{
    static edge* cur = ed;
    cur->to = to;
    cur->pre = head[from];
    head[from] = cur;
    ++cur;
}
unsigned int dfsSon(const unsigned int x)
{
    unsigned int ret = 1, ms = 0;
    maxDep[x] = dep[x];
    for (const edge* i = head[x]; i; i = i->pre)
    {
        const unsigned int to = i->to;
        dep[to] = dep[x] + 1;
        const unsigned int sz = dfsSon(to);
        maxDep[x] = max(maxDep[x], maxDep[to]);
        ret += sz;
        if (sz > ms)
        {
            ms = sz;
            son[x] = to;
        }
    }
    return ret;
}
inline void addLetter(const unsigned int d, const unsigned int c)
{
    oddCnt[d] -= odd[d][c];
    odd[d][c] ^= 1;
    oddCnt[d] += odd[d][c];
}
inline void clearLetter(const unsigned int x)
{
    for (unsigned int i = dep[x]; i <= maxDep[x]; ++i)
        fill(odd[i], odd[i] + 26, false);
    fill(oddCnt + dep[x], oddCnt + maxDep[x] + 1, 0);
}
void dfsCount(const unsigned int x)
{
    addLetter(dep[x], ch[x]);
    for (const edge* i = head[x]; i; i = i->pre)
        dfsCount(i->to);
}
void dfs(const unsigned int x)
{
    for (const edge* i = head[x]; i; i = i->pre)
        if (i->to != son[x])
        {
            dfs(i->to);
            clearLetter(i->to);
        }
    if (son[x])
        dfs(son[x]);

    addLetter(dep[x], ch[x]);
    for (const edge* i = head[x]; i; i = i->pre)
        if (i->to != son[x])
            dfsCount(i->to);
    for (auto i : seq[x])
        if (i->h < dep[x] || i->h > maxDep[x])
            i->ans = true;
        else
            i->ans = oddCnt[i->h] <= 1;
}

int main()
{
    ios_base::sync_with_stdio(false);
    cin.read(inbuf, sizeof(inbuf));
    const unsigned int n = read(), m = read();
    for (unsigned int i = 2; i <= n; ++i)
    {
        const unsigned int f = read();
        addEdge(f, i);
    }
    {
        static char str[maxn + 10];
        read(str + 1);
        for (unsigned int i = 1; i <= n; ++i)
            ch[i] = str[i] - 'a';
    }
    for (Query* i = qry; i < qry + m; ++i)
    {
        i->v = read();
        i->h = read();
        seq[i->v].push_back(i);
    }
    dfsSon(1);
    dfs(1);
    for (Query* i = qry; i < qry + m; ++i)
        write(i->ans);
    cout.write(outbuf, outpos - outbuf);
    return 0;
}
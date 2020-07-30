#ifdef APTEST
#include "debug_tools/program.h"
#endif
#include <cstring>
#include <iostream>
#include <queue>
#include <vector>
using namespace std;
const unsigned int maxl = 1e5, maxn = 1e5, maxm = 1e5;

struct
{
    struct Edge
    {
        unsigned int to;
        const Edge* pre;
    } ed[maxl * 2 + 1], *cur = ed;
    const Edge* head[maxl + 1] {};

    void addEdge(const unsigned int from, const unsigned int to)
    {
        cur->to = to;
        cur->pre = head[from];
        head[from] = cur;
        ++cur;
    }
} trie, fil;
struct Query
{
    unsigned int p, ans;
} qry[maxm + 10];
vector<Query*> seq[maxn + 1];
unsigned int ch[maxl + 10][26], fa[maxl + 10], rev[maxn + 10], fail[maxl + 10];
unsigned int siz[maxl + 10], b[maxl + 10], dfn[maxl + 1], cnt;

namespace ftree
{
#define lowbit(x) ((x) & -(x))
    int val[maxl + 1];

    void modify(unsigned int p, const int delt)
    {
        if (!p)
            return;
        for (; p <= cnt + 1; p += lowbit(p))
            val[p] += delt;
    }
    unsigned int query(unsigned int p)
    {
        int ret = 0;
        for (; p; p -= lowbit(p))
            ret += val[p];
        return ret;
    }
    inline unsigned int query(const unsigned int l, const unsigned int r)
    {
        return query(r) - query(l - 1);
    }

#undef lowbit
}
void insert(const char* str)
{
    const size_t len = strlen(str);
    unsigned int p = 0, id = 1;
    for (unsigned int i = 0; i < len; ++i)
    {
        if (str[i] == 'P')
        {
            b[p] = id;
            rev[id] = p;
            ++id;
            continue;
        }
        else if (str[i] == 'B')
        {
            p = fa[p];
            continue;
        }
        const unsigned int c = str[i] - 'a';
        if (!ch[p][c])
        {
            ch[p][c] = ++cnt;
            trie.addEdge(p, ch[p][c]);
            fa[ch[p][c]] = p;
        }
        p = ch[p][c];
    }
}
void make()
{
    queue<unsigned int> q;
    for (unsigned int i = 0; i < 26; ++i)
        if (ch[0][i])
        {
            fil.addEdge(0, ch[0][i]);
            q.push(ch[0][i]);
        }
    while (!q.empty())
    {
        const unsigned int cur = q.front();
        q.pop();
        for (unsigned int i = 0; i < 26; ++i)
        {
            if (!ch[cur][i])
                ch[cur][i] = ch[fail[cur]][i];
            else
            {
                fil.addEdge(ch[fail[cur]][i], ch[cur][i]);
                fail[ch[cur][i]] = ch[fail[cur]][i];
                q.push(ch[cur][i]);
            }
        }
    }
}
void dfsFail(const unsigned int x)
{
    static unsigned int sta = 0;
    siz[x] = 1;
    dfn[x] = ++sta;
    for (auto i = fil.head[x]; i; i = i->pre)
    {
        dfsFail(i->to);
        siz[x] += siz[i->to];
    }
}
void dfsTrie(const unsigned int x)
{
    ftree::modify(dfn[x], 1);
    if (b[x])
    {
        for (auto i : seq[b[x]])
        {
            const unsigned int p = rev[i->p];
            i->ans = ftree::query(dfn[p], dfn[p] + siz[p] - 1);
        }
    }
    for (auto i = trie.head[x]; i; i = i->pre)
        dfsTrie(i->to);
    ftree::modify(dfn[x], -1);
}

int main()
{
#ifndef APTEST
    ios_base::sync_with_stdio(false);
#endif
    static char str[maxl + 1];
    unsigned int m;
    cin >> str >> m;
    for (Query* i = qry; i < qry + m; ++i)
    {
        unsigned int k;
        cin >> i->p >> k;
        seq[k].push_back(i);
    }
    insert(str);
    make();
    dfsFail(0);
    dfsTrie(0);
    for (Query* i = qry; i < qry + m; ++i)
        cout << i->ans << endl;
    return 0;
}
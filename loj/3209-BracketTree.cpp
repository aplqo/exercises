#ifdef APTEST
#include "debug_tools/program.h"
#endif
#include <fstream>
#include <iostream>
using namespace std;
const unsigned int maxn = 5e5;

struct Edge
{
    unsigned int to;
    const Edge* pre;
} ed[maxn + 1];
const Edge* head[maxn + 1];
char str[maxn + 10];
unsigned int lst[maxn + 1], fa[maxn + 1];
unsigned long long w[maxn + 1], ans;

inline void addEdge(const unsigned int from, const unsigned int to)
{
    static Edge* cur = ed;
    new (cur) Edge { to, head[from] };
    head[from] = cur++;
}
void dfs(const unsigned int x, const unsigned long long sum = 0)
{
    if (str[x] == '(')
    {
        lst[x] = x;
        w[x] = 0;
    }
    else
    {
        const unsigned int p = lst[fa[x]];
        lst[x] = lst[fa[p]];
        w[x] = p ? 1 + w[fa[p]] : 0;
    }
    const unsigned long long ns = w[x] + sum;
    ans ^= x * ns;
    for (const Edge* i = head[x]; i; i = i->pre)
        dfs(i->to, ns);
}
int main()
{
#ifndef APTEST
    ios_base::sync_with_stdio(false);
#endif
#if !defined APTEST && !defined APDEBUG
    ifstream in("brackets.in");
    ofstream out("brackets.out");
#else
    istream& in = cin;
    ostream& out = cout;
#endif
    unsigned int n;
    in >> n >> (str + 1);
    for (unsigned int i = 2; i <= n; ++i)
    {
        in >> fa[i];
        addEdge(fa[i], i);
    }
    dfs(1);
    out << ans << "\n";
    return 0;
}
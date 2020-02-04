#ifdef APDEBUG
#include "debug_tools/program.h"
#endif
#include <algorithm>
#include <cctype>
#include <cstdio>
#include <iostream>
#include <numeric>
using namespace std;
const int maxn = 1e5, maxm = 1e5;

struct edge
{
    unsigned int to, val;
    edge* pre = nullptr;
} ed[maxm * 2];
edge* head[maxn + 10];
unsigned int fl[maxn + 10];
unsigned int n, m;

template <class T>
inline void read(T& dat)
{
    dat = 0;
    char c = getchar();
    while (!isdigit(c))
        c = getchar();
    while (isdigit(c))
    {
        dat = dat * 10 + c - '0';
        c = getchar();
    }
}
inline unsigned int read()
{
    char c = getchar();
    while (!isalpha(c))
        c = getchar();
    return c == 'S' ? 0x00 : 0x02;
}
inline void AddEdge(unsigned int from, unsigned int to, unsigned int val)
{
    static edge* cur = ed;
    cur->to = to;
    cur->val = val;
    cur->pre = head[from];
    head[from] = cur;
    ++cur;
}
bool dfs(unsigned int x)
{
    for (edge* i = head[x]; i; i = i->pre)
    {
        if (!fl[i->to])
        {
            fl[i->to] = fl[x] ^ i->val;
            if (!dfs(i->to))
                return false;
        }
        else if (fl[i->to] ^ (fl[x] ^ i->val))
            return false;
    }
    return true;
}

int main()
{
    read(n);
    read(m);
    for (unsigned int i = 0; i < m; ++i)
    {
        unsigned int a, b, v;
        v = read();
        read(a);
        read(b);
        AddEdge(a, b, v);
        AddEdge(b, a, v);
    }
    unsigned int a = 0;
    for (unsigned int i = 1; i <= n; ++i)
        if (!fl[i])
        {
            fl[i] = 1;
            if (!dfs(i))
            {
                cout << "0" << endl;
                return 0;
            }
            ++a;
        }

    cout.put('1');
    for (unsigned int i = 0; i < a; ++i)
        cout.put('0');
    cout << endl;
    return 0;
}
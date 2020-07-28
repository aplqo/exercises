#ifdef APTEST
#include "debug_tools/program.h"
#endif
#include <algorithm>
#include <cstring>
#include <iostream>
#include <iterator>
#include <queue>
using namespace std;
const unsigned int maxn = 100000, maxm = 100000;

struct edge
{
    unsigned int to;
    const edge* pre;
} ed[maxn + 1], *cur = ed;
const edge* head[maxn + 1];
unsigned int degree[maxn + 1], seq[maxn + 10];

void addEdge(const unsigned int from, const unsigned int to)
{
    cur->to = to;
    cur->pre = head[from];
    head[from] = cur;
    ++degree[to];
    ++cur;
}
void topological(const unsigned int n)
{
    priority_queue<unsigned int> q;
    for (unsigned int i = 1; i <= n; ++i)
        if (!degree[i])
            q.push(i);
    unsigned int* pos = seq;
    while (!q.empty())
    {
        const unsigned int c = q.top();
        q.pop();
        *(pos++) = c;
        for (const edge* i = head[c]; i; i = i->pre)
        {
            --degree[i->to];
            if (!degree[i->to])
                q.push(i->to);
        }
    }
}
void solve()
{
    unsigned int n, m;
    cin >> n >> m;
    for (unsigned int i = 0; i < m; ++i)
    {
        unsigned int x, y;
        cin >> x >> y;
        addEdge(y, x);
    }
    topological(n);
    if (any_of(degree + 1, degree + 1 + n, [](const unsigned int i) { return i; }))
        cout << "Impossible!";
    else
        copy_n(reverse_iterator<unsigned int*>(seq + n), n, ostream_iterator<unsigned int>(cout, " "));
    cout << endl;
}

int main()
{
#ifndef APTEST
    ios_base::sync_with_stdio(false);
#endif
    unsigned int d;
    cin >> d;
    for (unsigned int i = 0; i < d; ++i)
    {
        solve();
        memset(degree, 0, sizeof(degree));
        memset(head, 0, sizeof(head));
        cur = ed;
    }
    return 0;
}
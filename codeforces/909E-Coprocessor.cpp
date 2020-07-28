#ifdef APTEST
#include "debug_tools/program.h"
#endif
#include <algorithm>
#include <iostream>
#include <iterator>
#include <queue>
using namespace std;
const unsigned int maxn = 1e5, maxm = 1e5;

struct edge
{
    unsigned int to;
    const edge* pre;
} ed[maxm + 1];
const edge* head[maxn + 1];
unsigned int degree[maxn + 1];
bool co[maxn + 1];
queue<unsigned int> mq, cq;

void addEdge(const unsigned int from, const unsigned int to)
{
    static edge* cur = ed;
    cur->to = to;
    cur->pre = head[from];
    head[from] = cur;
    ++cur;
    ++degree[to];
}
inline void add(const unsigned int i)
{
    if (!degree[i])
        (co[i] ? &cq : &mq)->push(i);
}
void redu(queue<unsigned int>& q)
{
    while (!q.empty())
    {
        const unsigned int c = q.front();
        q.pop();
        for (const edge* i = head[c]; i; i = i->pre)
        {
            const unsigned int to = i->to;
            --degree[to];
            add(to);
        }
    }
}
unsigned int topological(const unsigned int n)
{
    unsigned int ret = 0;
    for (unsigned int i = 0; i < n; ++i)
        add(i);
    while (!mq.empty() || !cq.empty())
    {
        redu(mq);
        if (!cq.empty())
        {
            ++ret;
            redu(cq);
        }
    }
    return ret;
}
int main()
{
#ifndef APTEST
    ios_base::sync_with_stdio(false);
#endif
    unsigned int n, m;
    cin >> n >> m;
    copy_n(istream_iterator<unsigned int>(cin), n, co);
    for (unsigned int i = 0; i < m; ++i)
    {
        unsigned int t1, t2;
        cin >> t1 >> t2;
        addEdge(t2, t1);
    }
    cout << topological(n) << endl;
    return 0;
}
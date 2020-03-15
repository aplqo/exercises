#ifdef APTEST
#include "debug_tools/program.h"
#endif
#include <algorithm>
#include <cstring>
#include <iostream>
#include <queue>
#include <stack>
using namespace std;
const int maxn = 1000000, maxm = 10;

struct node
{
    node() = default;
    inline node(unsigned int num, node* pre)
        : num(num)
        , pre(pre)
        , vis(stamp)
    {
    }
    unsigned int num;
    unsigned int vis = 0;
    node* pre = nullptr;

    static unsigned int stamp;
} f[maxn + 10];
unsigned int node::stamp = 0;
unsigned int digit[maxm + 10];

inline void bfs(const unsigned int n, const unsigned int m)
{
    queue<unsigned int> q;
    for (unsigned int* i = digit; i < digit + m; ++i)
    {
        const unsigned int to = *i % n;
        if (!*i)
            continue;
        if (f[to].vis != node::stamp || f[to].num > *i)
            f[to] = node(*i, nullptr);
    }
    for (unsigned int i = 0; i < min<unsigned int>(n, 10); ++i)
        if (f[i].vis == node::stamp)
            q.push(i);
    while (!q.empty())
    {
        const unsigned int cur = q.front();
        q.pop();
        node* const pn = f + cur;
        for (unsigned int* i = digit; i < digit + m; ++i)
        {
            const unsigned int to = (cur * 10 + *i) % n;
            if (f[to].vis == node::stamp)
                continue;
            f[to] = node(*i, pn);
            q.push(to);
        }
    }
}
inline void print()
{
    stack<unsigned int> tmp;
    for (node* i = f; i; i = i->pre)
        tmp.push(i->num);
    while (!tmp.empty())
    {
        cout << tmp.top();
        tmp.pop();
    }
    cout << endl;
}
inline void solve()
{
    unsigned int n, m;
    cin >> n >> m;
    ++node::stamp;
    for_each(digit, digit + m, [](unsigned int& i) { cin >> i; });
    sort(digit, digit + m);
    bfs(n, m);
    if (f[0].vis == node::stamp)
        print();
    else
        cout << "0" << endl;
}
int main()
{
    ios_base::sync_with_stdio(false);
    unsigned int t;
    cin >> t;
    for (unsigned int i = 0; i < t; ++i)
        solve();
    return 0;
}

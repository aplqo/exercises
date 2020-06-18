#ifdef APTEST
#include "debug_tools/program.h"
#endif
#include <algorithm>
#include <climits>
#include <iostream>
#include <tuple>
#include <utility>
using namespace std;
using result = pair<unsigned int, int>;
const unsigned int maxn = 100000;
constexpr int lowest = INT_MIN / 2;

struct edge
{
    unsigned int to;
    int val = 1;
    edge *pre = nullptr, *rev = nullptr;
} ed[maxn * 2 + 1];
edge* head[maxn + 1];

edge* addEdge(const unsigned int from, const unsigned int to)
{
    static edge* cur = ed;
    cur->to = to;
    cur->pre = head[from];
    head[from] = cur;
    return cur++;
}
result dfsPath(const unsigned int x, const unsigned int f)
{
    result ret(x, 0);
    const auto update = [&ret](result i, const int v) {
        if (ret.second < i.second + v)
        {
            ret = i;
            ret.second += v;
        }
    };
    for (edge* i = head[x]; i; i = i->pre)
        if (i->to != f)
            update(dfsPath(i->to, x), i->val);
    return ret;
}
tuple<unsigned int, unsigned int, int> radDfs(const unsigned int n)
{
    const result v1 = dfsPath(1, 0);
    const result v2 = dfsPath(v1.first, 0);
    return make_tuple(v1.first, v2.first, v2.second);
}
bool setVal(const unsigned int x, const unsigned int f, const unsigned int dest)
{
    bool ret = false;
    for (edge* i = head[x]; i; i = i->pre)
        if (i->to != f && setVal(i->to, x, dest))
        {
            i->val = -1;
            i->rev->val = -1;
            ret = true;
        }
    return ret || x == dest;
}
int treeDp(const unsigned int x, const unsigned int f, int& ans)
{
    int ret = lowest;
    unsigned int cnt = 0;
    for (edge* i = head[x]; i; i = i->pre)
    {
        if (i->to == f)
            continue;
        ++cnt;
        const int dy = treeDp(i->to, x, ans) + i->val;
        ans = max({ ans, ret + dy, dy });
        ret = max(ret, dy);
    }
    return cnt ? ret : 0;
}

unsigned int solve(const unsigned int n)
{
    const auto [p1, p2, len] = radDfs(n);
    setVal(p1, 0, p2);
    int ans = 0;
    treeDp(1, 0, ans);
    return 2 * n - len - ans;
}

int main()
{
#ifndef APTEST
    ios_base::sync_with_stdio(false);
#endif
    unsigned int n, k;
    cin >> n >> k;
    for (unsigned int i = 1; i < n; ++i)
    {
        unsigned int a, b;
        cin >> a >> b;
        edge *e1 = addEdge(a, b), *e2 = addEdge(b, a);
        e1->rev = e2;
        e2->rev = e1;
    }
    if (k == 1)
        cout << (2 * n - 1 - get<2>(radDfs(n))) << endl;
    else
        cout << solve(n) << endl;
    return 0;
}
#ifdef APDEBUG
#include "debug_tools/program.h"
#endif
#include <algorithm>
#include <iostream>
#include <limits>
#include <queue>
using namespace std;
typedef int num;
const int maxn = (5e3 + 10) * 2;
constexpr num ninf = numeric_limits<num>::min();

struct edge
{
    int to;
    num val;
    edge* pre = nullptr;
} ed[maxn];
edge* head[maxn];
num ans[maxn];
int dep[maxn];
bool vis[maxn];
int n, m;

inline void AddEdge(int from, int to, num val)
{
    static edge* cur = ed;
    cur->to = to;
    cur->val = val;
    cur->pre = head[from];
    head[from] = cur;
    ++cur;
}
inline bool spfa(int s)
{
    queue<int> q;
    q.push(s);
    dep[s] = 1;
    while (!q.empty())
    {
        int cur = q.front();
        q.pop();
        vis[cur] = false;
        for (edge* i = head[cur]; i; i = i->pre)
            if (ans[i->to] < ans[cur] + i->val)
            {
                ans[i->to] = ans[cur] + i->val;
                ++dep[i->to];
                if (dep[i->to] > n)
                    return false;
                if (!vis[i->to])
                {
                    q.push(i->to);
                    vis[i->to] = true;
                }
            }
    }
    return true;
}

int main()
{
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);
    cin >> n >> m;
    for (int i = 0; i < m; ++i)
    {
        int c1, c2;
        num y;
        cin >> c1 >> c2 >> y;
        AddEdge(c1, c2, -y);
    }
    for (int i = 1; i <= n; ++i)
        AddEdge(0, i, (ninf / 2));
    fill(ans + 1, ans + 1 + n, ninf);
    if (!spfa(0))
    {
        cout << "NO" << endl;
        return 0;
    }
    for (num* i = ans + 1; i < ans + 1 + n; ++i)
        cout << *i << " ";
    return 0;
}
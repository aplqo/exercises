#ifdef APDEBUG
#include "debug_tools/program.h"
#endif
#include <algorithm>
#include <climits>
#include <iostream>
#include <queue>
using namespace std;
const int maxn = 1010, maxm = 10010 * 2 + maxn;
typedef int num;
const num inf = INT_MAX;

struct edge
{
    edge()
    {
        pre = NULL;
    }
    int to;
    num val;
    edge* pre;
} ed[maxm];
edge* head[maxn];
num ans[maxn];
int n, dep[maxn];
bool vis[maxn];

inline void AddEdge(int from, int to, num val)
{
    static edge* cur = ed;
    cur->to = to;
    cur->pre = head[from];
    cur->val = val;
    head[from] = cur;
    ++cur;
}
inline void init()
{
    fill(ans, ans + 1 + n, inf);
    fill(vis, vis + n + 1, false);
    fill(dep, dep + n + 1, 0);
}
inline bool spfa(int s)
{
    queue<int> q;
    q.push(s);
    ++dep[s];
    ans[s] = 0;
    while (!q.empty())
    {
        int cur = q.front();
        q.pop();
        vis[cur] = false;
        for (edge* i = head[cur]; i; i = i->pre)
        {
            if (ans[cur] + i->val < ans[i->to])
            {
                ans[i->to] = ans[cur] + i->val;
                if (!vis[i->to])
                {
                    q.push(i->to);
                    vis[i->to] = true;
                    ++dep[i->to];
                    if (dep[i->to] > n + 1)
                        return false;
                }
            }
        }
    }
    return true;
}

int main()
{
    ios_base::sync_with_stdio(false);
    int ml, md;
    cin >> n >> ml >> md;
    for (int i = 0; i < ml; ++i)
    {
        int a, b;
        num d;
        cin >> a >> b >> d;
        AddEdge(a, b, d);
    }
    for (int i = 0; i < md; ++i)
    {
        int a, b;
        num d;
        cin >> a >> b >> d;
        AddEdge(b, a, -d);
    }
    for (int i = 1; i <= n; ++i)
        AddEdge(0, i, 0);
    fill(ans, ans + n + 1, inf);
    if (!spfa(0))
    {
        cout << "-1" << endl;
        return 0;
    }
    for (int i = 1; i < n - 1; ++i)
        AddEdge(i + 1, i, 0);
    init();
    spfa(1);
    if (ans[n] == inf)
        cout << "-2" << endl;
    else
        cout << ans[n] << endl;
    return 0;
}
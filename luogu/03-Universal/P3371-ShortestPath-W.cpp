#ifdef APDEBUG
#define COLOR
#define APINPROG
#include "debug_tools/time.cpp"
#endif
#include <algorithm>
#include <iostream>
#include <queue>
using namespace std;
typedef int numType;
const numType inf = (numType(1) << 31) - 1;
const int maxn = 1e4 + 5, maxm = 5e5 + 5;

struct edge
{
    int to;
    numType val;
    edge* pre = nullptr;
} ed[maxm];
edge* head[maxn];
numType ans[maxn];
bool vis[maxn];
int n, m, s;

inline void AddEdge(int a, int b, numType v)
{
    static edge* cur = ed;
    cur->to = b;
    cur->val = v;
    cur->pre = head[a];
    head[a] = cur;
    ++cur;
}
inline void spfa()
{
    queue<int> q;
    q.push(s);
    fill(ans, ans + n + 1, inf);
    vis[s] = true;
    ans[s] = 0;
    while (!q.empty())
    {
        int cur = q.front();
        q.pop();
        vis[cur] = false;
        for (edge* i = head[cur]; i != nullptr; i = i->pre)
        {
            if (ans[i->to] > ans[cur] + i->val)
            {
                ans[i->to] = ans[cur] + i->val;
                if (!vis[i->to])
                {
                    q.push(i->to);
                    vis[i->to] = true;
                }
            }
        }
    }
}
int main()
{
    ios_base::sync_with_stdio(false);
    cin >> n >> m >> s;
    for (int i = 0; i < m; ++i)
    {
        int u, v, w;
        cin >> u >> v >> w;
        AddEdge(u, v, w);
    }
    spfa();
    for (numType* i = ans + 1; i < ans + 1 + n; ++i)
        cout << *i << " ";
    return 0;
}
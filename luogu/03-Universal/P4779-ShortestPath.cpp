#ifdef APDEBUG
#define COLOR
#define APINPROG
#include "debug_tools/time.cpp"
#endif
#include <algorithm>
#include <functional>
#include <iostream>
#include <limits>
#include <queue>
#include <utility>
using namespace std;
typedef pair<int, int> pairType;
typedef priority_queue<pairType, vector<pairType>, greater<pairType>> que;
typedef int num;
const int maxn = 1e5 + 5, maxm = 2e5 + 5;

struct edge
{
    int to;
    num val;
    edge* pre = nullptr;
} ed[maxm];
edge* head[maxn];
num ans[maxn];
bool vis[maxn];
int n, m, s;

inline void AddEdge(int x, int y, num va)
{
    static edge* cur = ed;
    cur->to = y;
    cur->val = va;
    cur->pre = head[x];
    head[x] = cur;
    ++cur;
}
void dijkstra(int s)
{
    que q;
    q.push(make_pair(0, s));
    fill(ans, ans + n + 1, numeric_limits<num>::max());
    ans[s] = 0;
    while (!q.empty())
    {
        pairType cur = q.top();
        q.pop();
        if (vis[cur.second])
            continue;
        vis[cur.second] = true;
        for (edge* i = head[cur.second]; i; i = i->pre)
        {
            if (ans[i->to] > cur.first + i->val)
            {
                ans[i->to] = cur.first + i->val;
                q.push(make_pair(ans[i->to], i->to));
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
        int u, v;
        num w;
        cin >> u >> v >> w;
        AddEdge(u, v, w);
    }
    dijkstra(s);
    for (num* i = ans + 1; i < ans + 1 + n; ++i)
        cout << *i << " ";
    return 0;
}
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
typedef unsigned int numType;
typedef pair<numType, int> pairType;
const numType inf = numeric_limits<numType>::max();
const int maxn = 55, maxm = 10005, maxk = 55;

struct edge
{
    int to;
    numType c;
    edge* pre = nullptr;
} lin[maxm];
edge* head[maxn];
numType ans[maxn][maxk];
bool vis[maxn], inque[maxn];
int n, m, k;
int s = 1, t;

inline void AddEdge(int a, int b, numType c)
{
    static edge* cur = lin;
    cur->to = b;
    cur->c = c;
    cur->pre = head[a];
    head[a] = cur;
    ++cur;
}
inline void dijkstra()
{
    priority_queue<pairType, vector<pairType>, greater<pairType>> q;
    for_each(ans, ans + n + 1, [](numType(&t)[maxk]) -> void { fill(t, t + k + 1, inf); });
    q.push(make_pair(0u, s));
    ans[s][0] = 0;
    while (!q.empty())
    {
        pairType cur = q.top();
        q.pop();
        if (vis[cur.second])
            continue;
        vis[cur.second] = true;
        for (edge* i = head[cur.second]; i != nullptr; i = i->pre)
        {
            if (ans[i->to][0] > cur.first + i->c)
                ans[i->to][0] = cur.first + i->c;
            q.push(make_pair(ans[i->to][0], i->to));
        }
    }
}
inline void fun(int k)
{
    queue<int> q;
    fill(inque, inque + n, false);
    q.push(s);
    inque[s] = true;
    ans[s][k] = 0;
    while (!q.empty())
    {
        int cur = q.front();
        q.pop();
        inque[cur] = false;
        for (edge* i = head[cur]; i != nullptr; i = i->pre)
        {
            numType na = min(ans[cur][k - 1] + (i->c) / 2, ans[cur][k] + i->c);
            if (na < ans[i->to][k])
            {
                ans[i->to][k] = na;
                if (!inque[i->to])
                {
                    inque[i->to] = true;
                    q.push(i->to);
                }
            }
        }
    }
}
int main()
{
    ios_base::sync_with_stdio(false);
    cin >> n >> m >> k;
    t = n;
    for (int i = 0; i < m; ++i)
    {
        int a, b;
        numType c;
        cin >> a >> b >> c;
        AddEdge(a, b, c);
        AddEdge(b, a, c);
    }
    dijkstra();
    if (k >= m)
    {
        cout << ans[t][0] / 2 << endl;
        return 0;
    }
    for (int i = 1; i <= k; ++i)
        fun(i);
    cout << ans[t][k];
    return 0;
}
#include <algorithm>
#include <fstream>
using namespace std;
const unsigned int maxn = 1e5;

struct Edge
{
    unsigned int to;
    const Edge* pre;
} ed[maxn * 2 + 10];
const Edge* head[maxn + 10];
unsigned int fa[maxn + 10], dep[maxn + 10];
unsigned int vis[maxn + 10];

inline void addEdge(const unsigned int from, const unsigned int to)
{
    static Edge* cur = ed;
    new (cur) Edge { to, head[from] };
    head[from] = cur++;
    new (cur) Edge { from, head[to] };
    head[to] = cur++;
}
void dfs(const unsigned int x, const unsigned int f)
{
    fa[x] = f;
    dep[x] = dep[f] + 1;
    for (const Edge* i = head[x]; i; i = i->pre)
        if (i->to != f)
            dfs(i->to, x);
}
void markPath(unsigned int a, unsigned int b, const unsigned int mk)
{
    while (a != b)
    {
        if (dep[a] > dep[b])
            swap(a, b);
        vis[b] = mk;
        b = fa[b];
    }
    vis[b] = mk;
}
bool checkPath(unsigned int a, unsigned int b, const unsigned int mk)
{
    while (a != b)
    {
        if (dep[a] > dep[b])
            swap(a, b);
        if (vis[b] == mk)
            return true;
        b = fa[b];
    }
    return vis[b] == mk;
}
int main(int, char* argv[])
{
    ifstream in(argv[1]);
    ofstream ans(argv[2]);
    unsigned int n, q;
    in >> n;
    for (unsigned int i = 1; i < n; ++i)
    {
        unsigned int u, v;
        in >> u >> v;
        addEdge(u, v);
    }
    dfs(1, 0);
    in >> q;
    for (unsigned int i = 0; i < q; ++i)
    {
        unsigned int a, b, c, d;
        in >> a >> b >> c >> d;
        markPath(a, b, i + 1);
        ans << (checkPath(c, d, i + 1) ? "YES" : "NO") << "\n";
    }
    return 0;
}

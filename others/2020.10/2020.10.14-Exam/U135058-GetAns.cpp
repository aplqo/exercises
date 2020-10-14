#include <algorithm>
#include <cstring>
#include <fstream>
#include <iostream>
using namespace std;
const unsigned int maxn = 1e6, maxm = 2e7, maxc = 1e6;

struct Edge
{
    unsigned int to, val;
    const Edge* pre;
} ed[maxn * 2 + 10];
const Edge* head[maxn + 10];
unsigned int fa[maxn + 10], dep[maxn + 10], val[maxn + 10];

inline void addEdge(const unsigned int from, const unsigned int to, const unsigned int val)
{
    static Edge* cur = ed;
    new (cur) Edge { to, val, head[from] };
    head[from] = cur++;
    new (cur) Edge { from, val, head[to] };
    head[to] = cur++;
}
void dfs(const unsigned int x, const unsigned int f)
{
    dep[x] = dep[f] + 1;
    fa[x] = f;
    for (const Edge* i = head[x]; i; i = i->pre)
        if (i->to != f)
        {
            val[i->to] = i->val;
            dfs(i->to, x);
        }
}
bool check(unsigned int x, unsigned int y, const unsigned int mc)
{
    static bool cnt[maxn + 10];
    fill(cnt, cnt + mc + 1, 0);
    while (x != y)
    {
        if (dep[x] > dep[y])
            swap(x, y);
        cnt[val[y]] ^= 1;
        y = fa[y];
    }
    return count(cnt, cnt + mc + 1, true) < 2;
}
int main(int, char* argv[])
{
    ifstream in(argv[1]);
    ofstream out(argv[2]);
    unsigned int n, m, mc = 0, ans = 0;
    in >> n >> m;
    for (unsigned int i = 1; i < n; ++i)
    {
        unsigned int x, y, w;
        in >> x >> y >> w;
        addEdge(x, y, w);
        mc = max(mc, w);
    }
    dfs(1, 0);
    unsigned long long a, b;
    in >> a >> b;
    for (unsigned int i = 0; i < m; ++i)
    {
        ans += check(a % n + 1, b % n + 1, mc);
#ifdef Decode
        clog << a % n + 1 << " " << b % n + 1 << "\n";
#endif
        a = (a * 666073) % 1000000007ull;
        b = (b * 233) % 998244353ull;
    }
    out << ans << "\n";
    return 0;
}
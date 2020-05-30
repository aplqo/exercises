#include <algorithm>
#include <cstring>
#include <fstream>
#include <numeric>
using namespace std;
const unsigned int maxn = 1e5, maxm = 3e5;

struct edge
{
    unsigned int to;
    edge* pre = nullptr;
} ed[maxm + 1];
edge* head[maxn + 1];
bool vis[maxn], isHelp[maxn];
unsigned int seq[maxn];
bool ans[maxn + 1];

void addEdge(const unsigned int from, const unsigned int to)
{
    static edge* cur = ed;
    cur->to = to;
    cur->pre = head[from];
    head[from] = cur;
    ++cur;
}
void dfs(const unsigned int x, const unsigned int rt)
{
    if (isHelp[x] || vis[x])
        return;
    isHelp[x] |= x != rt;
    vis[x] = true;
    for (edge* i = head[x]; i; i = i->pre)
        dfs(i->to, rt);
}
unsigned int test(const unsigned int n)
{
    memset(vis, 0, sizeof(vis));
    memset(isHelp, 0, sizeof(isHelp));
    for (unsigned int* i = seq; i < seq + n; ++i)
        dfs(*i, *i);
    return count(isHelp, isHelp + n, true);
}
void solve(const unsigned int n)
{
    iota(seq, seq + n, 0);
    do
    {
        ans[test(n)] = true;
    } while (next_permutation(seq, seq + n));
}
int main(int argc, char* argv[])
{
    ifstream in(argv[1]);
    ofstream fans(argv[2]);
    unsigned int n, m;
    in >> n >> m;
    for (unsigned int i = 0; i < m; ++i)
    {
        unsigned int x, y;
        in >> x >> y;
        addEdge(x - 1, y - 1);
    }
    in.close();
    solve(n);
    for (unsigned int i = 0; i <= n; ++i)
        if (ans[i])
            fans << i << endl;
    return 0;
}
#include <fstream>
#include <utility>
using namespace std;
const unsigned int maxn = 1e5;

struct edge
{
    unsigned int to;
    long long val;
    edge* pre;
} ed[maxn * 2 + 1];
edge* head[maxn + 1];
unsigned int degree[maxn + 1];

void addEdge(const unsigned int from, const unsigned int to, const long long val)
{
    static edge* cur = ed;
    cur->to = to;
    cur->val = val;
    cur->pre = head[from];
    head[from] = cur;
    ++degree[to];
    ++cur;
}
pair<long long, int> dfs(const unsigned int x, const unsigned int f)
{
    long long s = 0;
    int c = 1;
    for (edge* i = head[x]; i; i = i->pre)
    {
        if (i->to == f)
            continue;
        const auto t = dfs(i->to, x);
        c += t.second;
        s += t.first + t.second * i->val;
    }
    return make_pair(s, c);
}

int main(int, char* argv[])
{
    ifstream in(argv[1]);
    ofstream fans(argv[2]);
    unsigned int n;
    in >> n;
    for (unsigned int i = 1; i < n; ++i)
    {
        unsigned int u, v;
        long long w;
        i >> w >> u >> v;
        addEdge(u, v, w);
        addEdge(v, u, w);
    }
    long long ans = 0;
    for (unsigned int i = 1; i <= n; ++i)
        if (degree[i] == 1)
            ans += dfs(i, 0).first;
    fans << ans << endl;
    return 0;
}

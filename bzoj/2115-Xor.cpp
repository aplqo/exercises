#ifdef APTEST
#include "debug_tools/program.h"
#endif
#include <iostream>
using namespace std;
const unsigned int maxn = 50000, maxm = 100000, maxl = 63;

struct Edge
{
    unsigned int from, to;
    unsigned long long val;
    const Edge* pre;
} ed[maxm * 2 + 1];
const Edge* head[maxn + 1];
class LinearBasis
{
public:
    void insert(unsigned long long x);
    unsigned long long query(unsigned long long a) const;

private:
    unsigned long long vec[maxl + 1] = {};
} lb;
unsigned long long dis[maxn + 1];

void addEdge(const unsigned int from, const unsigned int to, const unsigned long long val)
{
    static Edge* cur = ed;
    cur->to = to;
    cur->val = val;
    cur->pre = head[from];
    head[from] = cur++;
}
void LinearBasis::insert(unsigned long long x)
{
    for (int i = maxl; i >= 0 && x; --i)
    {
        if (!(x & (1ull << i)))
            continue;
        if (vec[i])
            x ^= vec[i];
        else
        {
            for (unsigned int j = 0; j < i; ++j)
                if (x & (1ull << j))
                    x ^= vec[j];
            for (unsigned int j = i + 1; j <= maxl; ++j)
                if (vec[j] & (1ull << i))
                    vec[j] ^= x;
            vec[i] = x;
            break;
        }
    }
}
unsigned long long LinearBasis::query(unsigned long long x) const
{
    for (int i = maxl; i >= 0; --i)
        if ((x ^ vec[i]) > x)
            x ^= vec[i];
    return x;
}
void dfs(const unsigned int x, const unsigned int f)
{
    static bool vis[maxn + 1];
    vis[x] = true;
    for (const Edge* i = head[x]; i; i = i->pre)
    {
        if (i->to == f)
            continue;
        if (!vis[i->to])
        {
            dis[i->to] = dis[x] ^ i->val;
            dfs(i->to, x);
        }
        else
            lb.insert(dis[x] ^ dis[i->to] ^ i->val);
    }
}
int main()
{
#ifndef APTEST
    ios_base::sync_with_stdio(false);
#endif
    unsigned int n, m;
    cin >> n >> m;
    for (unsigned int i = 0; i < m; ++i)
    {
        unsigned int x, y;
        unsigned long long a;
        cin >> x >> y >> a;
        addEdge(x, y, a);
        addEdge(y, x, a);
    }
    dfs(1, 0);
    cout << lb.query(dis[n]) << endl;
    return 0;
}
#ifdef APTEST
#include "debug_tools/program.h"
#endif
#include <climits>
#include <iostream>
#include <random>
#include <unordered_map>
using namespace std;
const unsigned int maxn = 1e6, maxm = 2e7;
constexpr unsigned long long moda = 1000000007, modb = 998244353, rata = 666073, ratb = 233;

struct Edge
{
    unsigned int to;
    unsigned long long rnd;
    const Edge* pre;
} ed[maxn * 2 + 10];
const Edge* head[maxn + 10];
unsigned long long prefix[maxn + 10];
unordered_map<unsigned int, unsigned long long> toWeight;
unordered_map<unsigned long long, unsigned int> fromWeight;

unsigned long long getWeight(const unsigned int val)
{
    static ranlux48 rnd((random_device())());
    static uniform_int_distribution<unsigned long long> dis(0, ULLONG_MAX);
    if (const auto it = toWeight.find(val); it != toWeight.end())
        return it->second;
    else
    {
        unsigned long long v;
        do
        {
            v = dis(rnd);
        } while (fromWeight.count(v));
        toWeight[val] = v;
        fromWeight[v] = val;
        return v;
    }
}
inline void addEdge(const unsigned int from, const unsigned int to, const unsigned int val)
{
    static Edge* cur = ed;
    const unsigned long long w = getWeight(val);
    new (cur) Edge { to, w, head[from] };
    head[from] = cur++;
    new (cur) Edge { from, w, head[to] };
    head[to] = cur++;
}
void dfs(const unsigned int x, const unsigned int f)
{
    for (const Edge* i = head[x]; i; i = i->pre)
        if (i->to != f)
        {
            prefix[i->to] = prefix[x] ^ i->rnd;
            dfs(i->to, x);
        }
}
bool check(const unsigned int x, const unsigned int y)
{
    const unsigned long long w = prefix[x] ^ prefix[y];
    return !w || fromWeight.count(w);
}

int main()
{
#ifndef APTEST
    ios_base::sync_with_stdio(false);
#endif
    unsigned int n, m, ans = 0;
    cin >> n >> m;
    fromWeight.reserve(n);
    toWeight.reserve(n);
    for (unsigned int i = 1; i < n; ++i)
    {
        unsigned int u, v, w;
        cin >> u >> v >> w;
        addEdge(u, v, w);
    }
    dfs(1, 0);
    {
        unsigned long long a, b;
        cin >> a >> b;
        for (unsigned int i = 0; i < m; ++i)
        {
            ans += check(a % n + 1, b % n + 1);
            a = (a * rata) % moda;
            b = (b * ratb) % modb;
        }
    }
    cout << ans << "\n";
    return 0;
}
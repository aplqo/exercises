#ifdef APTEST
#include "debug_tools/program.h"
#endif
#include <algorithm>
#include <iostream>
#include <iterator>
#include <queue>
#include <vector>
using namespace std;
const unsigned int maxm = 1e6, maxk = 10;

#ifdef Use2Mod
constexpr unsigned long long mod1 = 1e9 + 7, mod2 = 1e9 + 9;

struct HashValue
{
    unsigned long long hash1, hash2;

    inline HashValue operator*(const HashValue& r) const
    {
        return HashValue { (hash1 + r.hash1) % mod1, (hash2 + r.hash2) % mod2 };
    }
    inline HashValue operator*(const unsigned int x) const
    {
        return HashValue { (hash1 * x) % mod1, (hash2 * x) % mod2 };
    }
    inline HashValue operator+(const unsigned int x) const
    {
        return HashValue { (hash1 + x) % mod1, (hash2 + x) % mod2 };
    }
    inline bool operator==(const HashValue& r) const
    {
        return hash1 == r.hash2 && hash2 == r.hash2;
    }
};
constexpr HashValue zero { 0, 0 };
#else
typedef unsigned long long HashValue;
constexpr HashValue zero = 0;
#endif
unsigned int bfn[maxm + 10];
unsigned int fromBfn[maxm + 10], fromVal[maxm + 10], from[maxm + 10];

unsigned int get(queue<unsigned int> q[], const unsigned int k)
{
    auto ptr = q, lst = q;
    for (unsigned int i = 0; i < k; ++i)
        if (ptr->empty())
            ++ptr;
    lst = ptr;
    for (; ptr < q + k; ++ptr)
        if (!ptr->empty() && fromBfn[ptr->front()] < fromBfn[lst->front()])
            lst = ptr;
    const unsigned int ret = lst->front();
    lst->pop();
    return ret;
}
void bfs(const unsigned int k, const unsigned int m)
{
    static HashValue pathHash[maxm + 10];
    static bool vis[maxm + 10], touch[maxm + 10];
    queue<unsigned int> q[maxk + 10];
    unsigned int sta = 0, cnt = 0;
    HashValue lst = zero;
    const unsigned int ck = min(k, m + 1);
    const auto expand = [&q, &cnt](const unsigned int to, const unsigned int fr, const unsigned int v) {
        touch[to] = true;
        fromVal[to] = v;
        fromBfn[to] = bfn[fr];
        from[to] = fr;
        pathHash[to] = pathHash[fr] * 10 + v;
        q[v].push(to);
        ++cnt;
    };
    for (unsigned int i = 1; i < ck; ++i)
        expand(i % m, 0, i);
    while (cnt)
    {
        const unsigned int cur = get(q, k);
        --cnt;
        if (vis[cur])
            continue;
        vis[cur] = true;
        bfn[cur] = pathHash[cur] == lst ? sta : ++sta;
        lst = pathHash[cur];
        for (unsigned int i = 0; i < ck; ++i)
        {
            const unsigned int to = (cur * 10 + i) % m;
            if (!touch[to] || (bfn[cur] == fromBfn[to] && fromVal[to] > i))
                expand(to, cur, i);
        }
    }
}
void printSolution()
{
    vector<unsigned int> q;
    q.reserve(maxm);
    unsigned int pos = 0;
    do
    {
        q.push_back(fromVal[pos]);
        pos = from[pos];
    } while (pos);
    copy(q.crbegin(), q.crend(), ostream_iterator<unsigned int>(cout));
    cout.put('\n');
}
int main()
{
#ifndef APTEST
    ios_base::sync_with_stdio(false);
#endif
    unsigned int m, k;
    cin >> k >> m;
    bfs(k, m);
    printSolution();
    return 0;
}
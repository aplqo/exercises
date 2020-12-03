#ifdef APTEST
#include "debug_tools/program.h"
#endif
#include <algorithm>
#include <iostream>
#include <queue>
using namespace std;
const unsigned int maxn = 60000, maxm = 100000;

class Bitset
{
public:
    Bitset() = default;
    Bitset(const Bitset&) = delete;
    Bitset& operator=(const Bitset&) = delete;

    inline void allocate(const unsigned int siz);
    inline void operator|=(const Bitset& r);
    void set(const unsigned int pos);
    unsigned int count() const;

private:
    typedef unsigned long long Word;
    constexpr static unsigned int word = sizeof(Word) * 8, wmsk = 0x3f, wshf = 6;
    static Word mem[maxn * maxn / word + maxn], *cur;

    Word *begin, *end;
};
Bitset::Word Bitset::mem[maxn * maxn / Bitset::word + maxn], *Bitset::cur = Bitset::mem;
struct Edge
{
    unsigned int to;
    const Edge* pre;
} ed[maxm + 10];
const Edge* head[maxn + 10];
Bitset reach[maxn + 10];
unsigned int degree[maxn + 10], seq[maxn + 10], idx[maxn + 10];

inline void addEdge(const unsigned int from, const unsigned int to)
{
    static Edge* cur = ed;
    new (cur) Edge { to, head[from] };
    head[from] = cur++;
    ++degree[to];
}
inline void Bitset::allocate(const unsigned int n)
{
    const unsigned int s = (n >> wshf) + 1;
    begin = cur;
    cur += s;
    end = cur;
}
inline void Bitset::operator|=(const Bitset& r)
{
    const unsigned int sz = r.end - r.begin;
    for (unsigned int j = 0; j < sz; ++j)
        begin[j] |= r.begin[j];
}
inline void Bitset::set(const unsigned int p)
{
    begin[p >> wshf] |= Word(1) << (p & wmsk);
}
unsigned int Bitset::count() const
{
    unsigned int ret = 0;
    for (const Word* i = begin; i < end; ++i)
        ret += __builtin_popcountll(*i);
    return ret;
}

void topological(const unsigned int n)
{
    queue<unsigned int> q;
    for (unsigned int i = 1; i <= n; ++i)
        if (!degree[i])
            q.push(i);
    unsigned int sta = 0;
    while (!q.empty())
    {
        const unsigned int cur = q.front();
        q.pop();
        const unsigned int cid = sta++;
        idx[cur] = cid;
        seq[cid] = cur;
        for (const Edge* i = head[cur]; i; i = i->pre)
        {
            const unsigned int to = i->to;
            --degree[to];
            if (!degree[to])
                q.push(to);
        }
    }
}
unsigned long long count(const unsigned int n)
{
    unsigned long long ret = 0;
    topological(n);
    for (unsigned int i = 1; i <= n; ++i)
        reach[i].allocate(idx[i]);
    for (const unsigned int* i = seq; i < seq + n; ++i)
    {
        ret += reach[*i].count();
        reach[*i].set(i - seq);
        for (const Edge* j = head[*i]; j; j = j->pre)
            reach[j->to] |= reach[*i];
    }
    return ret;
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
        unsigned int a, b;
        cin >> a >> b;
        addEdge(a, b);
    }
    cout << count(n) - m << "\n";
    return 0;
}

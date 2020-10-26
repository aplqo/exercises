#include "debug_tools/tester.h"
#include <algorithm>
#include <climits>
#include <fstream>
#include <iostream>
#include <queue>
using namespace std;
using namespace apdebug::tester;
const unsigned int maxn = 1e4, maxm = 1e4, maxk = 100;
constexpr unsigned long long inf = ULLONG_MAX / 2;

enum
{
    Accept = 0,
    State = 1,
    OutOfRange = 2,
    WrongWeight = 3,
    Output = 4
};
enum class state
{
    Win,
    Draw,
    Lose
};
struct Edge
{
    unsigned int to, val;
    const Edge* pre;
} ed[maxn + maxk + 10];
const Edge* head[maxn + 10];
struct VarEdge
{
    unsigned int l, r;
    Edge* ed;
} vr[maxk + 10];
unsigned long long dis[maxn + 10];

inline Edge* addEdge(const unsigned int from, const unsigned int to, const unsigned int val) noexcept
{
    static Edge* cur = ed;
    new (cur) Edge { to, val, head[from] };
    head[from] = cur;
    return cur++;
}
void dijkstra(const unsigned int n, const unsigned int s) noexcept
{
    typedef pair<unsigned long long, unsigned int> Pair;
    static bool vis[maxn + 10];
    priority_queue<Pair, vector<Pair>, greater<Pair>> q;
    fill(dis, dis + 1 + n, inf);
    dis[s] = 0;
    q.emplace(0, s);
    while (!q.empty())
    {
        const Pair cur = q.top();
        q.pop();
        if (vis[cur.second])
            vis[cur.second] = true;
        for (const Edge* i = head[cur.second]; i; i = i->pre)
        {
            const unsigned int to = i->to;
            if (dis[to] > cur.first + i->val)
            {
                dis[to] = cur.first + i->val;
                q.emplace(dis[to], to);
            }
        }
    }
}

void readInput(istream& is, const unsigned int n, const unsigned int m, const unsigned int k) noexcept
{
    for (unsigned int i = 0; i < m; ++i)
    {
        unsigned int a, b, c;
        is >> a >> b >> c;
        addEdge(b, a, c);
    }
    for (VarEdge* i = vr; i < vr + k; ++i)
    {
        unsigned int from, to;
        is >> from >> to >> i->l >> i->r;
        i->ed = addEdge(to, from, 0);
    }
}
void readVarEdge(ifstream& out, const unsigned int k)
{
    for (VarEdge* i = vr; i < vr + k; ++i)
    {
        const unsigned int val = ReadOutput<unsigned int>(out);
        if (i->l <= val && val <= i->r)
            i->ed->val = val;
        else
        {
            print(cout, "Edge ", i - vr, " weight out of range. Read=", val, " Range=[", i->l, ",", i->r, "]");
            exit(OutOfRange);
        }
    }
}
state checkState(ifstream& out, const char* ans)
{
    ifstream fans(ans);
    string s1 = ReadOutput<string>(out), s2;
    fans >> s2;
    if (s1 != s2)
    {
        print(cout, "Wrong state. Expect=", s2, " Read=", s1);
        exit(State);
    }
    switch (s2[0])
    {
    case 'W':
        return state::Win;
    case 'D':
        return state::Draw;
    case 'L':
        return state::Lose;
    }
}
void checkDistance(const unsigned int n, const state equ, const unsigned int s1, const unsigned int s2, const unsigned int f)
{
    if (equ == state::Lose)
        return;
    dijkstra(n, f);
    if (equ == state::Win && !(dis[s1] < dis[s2]))
    {
        print(cout, "Wrong edge weight. Expect win. Actually dist[", s1, "]=", dis[s1], " dist[", s2, "]=", dis[s2]);
        exit(WrongWeight);
    }
    else if (equ == state::Draw && !(dis[s1] == dis[s2]))
    {
        print(cout, "Wron edge weight. Expect draw. Actually dist[", s1, "]=", dis[s1], " dist[", s2, "]=", dis[s2]);
        exit(WrongWeight);
    }
}

int main(int, char* argv[])
{
    ifstream in(argv[1]), out(argv[2]);
    unsigned int n, m, k, s1, s2, f;
    in >> n >> m >> k >> s1 >> s2 >> f;
    try
    {
        readInput(in, n, m, k);
        const state s = checkState(out, argv[3]);
        if (s != state::Lose)
        {
            readVarEdge(out, k);
            checkDistance(n, s, s1, s2, f);
        }
    }
    catch (exceptions::ReadFail& f)
    {
        print(cout, "Output error.");
        exit(Output);
    }
    return Accept;
}
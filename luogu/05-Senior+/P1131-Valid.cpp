#include "debug_tools/tester.h"
#include <algorithm>
#include <cstdlib>
#include <fstream>
#include <iostream>
using namespace std;
using apdebug::tester::print;
using apdebug::tester::ReadOutput;
const unsigned int maxn = 500000, maxv = 1000000;

struct edge
{
    unsigned int to;
    edge *pre, *rev;
} ed[maxn * 2 + 10];
edge* head[maxn + 1];
bool vis[maxn + 1];

edge* addEdge(const unsigned from, const unsigned int to, const unsigned long long)
{
    static edge* cur = ed;
    cur->to = to;
    cur->pre = head[from];
    head[from] = cur;
    return cur++;
}
void dfs(const unsigned int x, const edge* const r)
{
    vis[x] = true;
    for (const edge* i = head[x]; i; i = i->pre)
    {
        if (i == r)
            continue;
        if (vis[i->to])
        {
            print(cout, "Fail: Found cycle in edge ", x, "<->", i->to);
            exit(1);
        }
        dfs(i->to, i->rev);
    }
}
int main(int argc, char* argv[])
{
    ifstream in(argv[1]);
    unsigned int n, s;
    in >> n >> s;
    if (n > maxn)
    {
        print(cout, "Fail: N too large. Read=", n, " ( n<=", maxn, " )");
        return 1;
    }
    const auto check = [n](const unsigned int v) {
        if (v > 0 && v <= n)
            return;
        print(cout, "Fail: Invavid vertex number. Read=", v);
        exit(1);
    };
    check(s);
    try
    {
        for (unsigned int i = 1; i < n; ++i)
        {
            unsigned int u, v;
            long long d;
            u = ReadOutput<unsigned int>(in);
            v = ReadOutput<unsigned int>(in);
            check(u);
            check(v);
            d = ReadOutput<long long>(in);
            if (d > maxv)
            {
                print(cout, "Fail: d too large! Read=", d, " ( d<=", maxv, " )");
                return 1;
            }
            else if (d < 0)
            {
                print(cout, "Fail: Native d. Read=", d);
                return 1;
            }
            edge *a = addEdge(u, v, d), *b = addEdge(v, u, d);
            a->rev = b;
            b->rev = a;
        }
    }
    catch (apdebug::tester::exceptions::ReadFail)
    {
        print(cout, "Fail: Input too short!");
        return 1;
    }
    dfs(s, 0);
    if (auto t = find(vis + 1, vis + 1 + n, false); t != vis + 1 + n)
    {
        print(cout, "Fail: Graph is not connected! Vertex ", t - vis, " is not in tree.");
        return 1;
    }
    return 0;
}
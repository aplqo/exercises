#include "debug_tools/generator.h"
#include <algorithm>
#include <cstring>
#include <fstream>
#include <random>
using namespace std;
using apdebug::generator::GetRandom;
using apdebug::generator::Init;
using apdebug::generator::MayBe;
const int maxn = 1500, maxk = 10000;

struct edge
{
    unsigned int from;
    unsigned int to;
    bool sel = false;
    edge* pre;
} ed[maxn * 2 + 1];
edge* head[maxn + 1];
unsigned int k[maxn + 1], cnt[maxn + 1];

void dfs(const unsigned int x, const unsigned int f)
{
    for (edge* i = head[x]; i; i = i->pre)
    {
        if (i->to == f)
            continue;
        ++cnt[x];
        i->sel = true;
        dfs(i->to, x);
    }
}
void addEdge(const unsigned int from, const unsigned int to)
{
    static edge* cur = ed;
    cur->from = from;
    cur->to = to;
    cur->pre = head[from];
    head[from] = cur;
    ++cur;
}
void createTree(const unsigned int n, const unsigned int mk)
{
    for (unsigned int i = 2; i <= n; ++i)
    {
        const unsigned int t = GetRandom(1u, i - 1);
        addEdge(i, t);
        addEdge(t, i);
    }
    dfs(GetRandom(1u, n), 0);
    mt19937_64 rnd;
    Init(rnd);
    uniform_int_distribution<unsigned int> disk(1, mk);
    generate(k + 1, k + 1 + n, [&disk, &rnd]() { return disk(rnd); });
}
void writeTree(ofstream& f, const unsigned int n)
{
    for (unsigned int i = 1; i <= n; ++i)
    {
        f << i << " " << k[i] << " " << cnt[i] << " ";
        for (edge* j = head[i]; j; j = j->pre)
            if (j->sel)
                f << j->to << " ";
        f << endl;
    }
}
int main(int argc, char* argv[])
{
    Init();
    const unsigned int n = GetRandom(1, MayBe(atoi(argv[1]), maxn));
    ofstream in(argv[3]);
    in << n << endl;
    createTree(n, MayBe(atoi(argv[2]), maxk));
    writeTree(in, n);
    return 0;
}
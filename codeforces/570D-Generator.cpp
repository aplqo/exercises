#include "debug_tools/generator.h"
#include <algorithm>
#include <cstring>
#include <fstream>
#include <iterator>
using namespace std;
using namespace apdebug::generator;
const int maxn = 500000, maxm = 500000;

struct edge
{
    unsigned int to;
    const edge* pre;
} ed[maxn + 10];
const edge* head[maxn + 1];
unsigned int dep[maxn + 1] = { 0, 1 }, mdep[maxn + 1];

void addEdge(const unsigned int from, const unsigned int to)
{
    static edge* cur = ed;
    cur->to = to;
    cur->pre = head[from];
    head[from] = cur;
    ++cur;
}
unsigned int dfs(const unsigned int x)
{
    mdep[x] = dep[x];
    for (const edge* i = head[x]; i; i = i->pre)
        mdep[x] = max(mdep[x], dfs(i->to));
    return mdep[x];
}
void createTree(ofstream& in, const unsigned int n)
{
    for (unsigned int i = 2; i <= n; ++i)
    {
        const unsigned int f = GetRandom(1u, i - 1);
        in << f << " ";
        dep[i] = dep[f] + 1;
        addEdge(f, i);
    }
    in << endl;
    dfs(1);
}
void createLetter(ofstream& in, const char* cset, const unsigned int n)
{
    uniform_int_distribution<unsigned int> dis(0, strlen(cset) - 1);
    generate_n(ostream_iterator<char>(in), n, [&dis, &cset]() { return cset[dis(default_rnd)]; });
    in << endl;
}
void createQuery(ofstream& in, const unsigned int n, const unsigned int q)
{
    uniform_int_distribution<unsigned int> disp(1, n);
    for (unsigned int i = 0; i < q; ++i)
    {
        const unsigned int p = disp(default_rnd);
        in << p << " " << GetRandom(1u, n) << endl;
    }
}
int main(int, char* argv[])
{
    Init();
    const unsigned int n = GetRandom(1, MayBe(atoi(argv[1]), maxn));
    const unsigned int q = GetRandom(1, MayBe(atoi(argv[2]), maxm));
    ofstream in(argv[4]);
    in << n << " " << q << endl;
    createTree(in, n);
    createLetter(in, argv[3], n);
    createQuery(in, n, q);
    return 0;
}
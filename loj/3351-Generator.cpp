#include "debug_tools/generator.h"
#include <algorithm>
#include <fstream>
#include <iterator>
#include <random>
#include <unordered_set>
using namespace std;
using namespace apdebug::generator;
const int maxn = 1e5, maxd = 500, maxu = 2e5, maxq = 50000, maxh = 1e9;

void createChange(ofstream& in, const unsigned int n, const unsigned int u, const unsigned int md)
{
    unordered_set<unsigned int>* ed = new unordered_set<unsigned int>[n];
    uniform_int_distribution<unsigned int> disv(0, n - 1);
    for (unsigned int i = 0; i < n; ++i)
        ed[i].reserve(md);
    for (unsigned int i = 0; i < u; ++i)
    {
        unsigned int u, v;
        do
        {
            u = disv(default_rnd);
            v = disv(default_rnd);
        } while (u == v || (ed[u].count(v) && (ed[u].size() > md || ed[v].size() > md)));
        in << u << " " << v << "\n";
        if (ed[u].count(v))
        {
            ed[u].erase(v);
            ed[v].erase(u);
        }
        else
        {
            ed[u].insert(v);
            ed[v].insert(u);
        }
    }
    delete[] ed;
}
void createQuery(ofstream& in, const unsigned int n, const unsigned int u, const unsigned int q)
{
    uniform_int_distribution<unsigned int> disv(0, n - 1), disd(1, u);
    for (unsigned int i = 0; i < q; ++i)
    {
        const unsigned int u = disv(default_rnd);
        unsigned int v;
        do
        {
            v = disv(default_rnd);
        } while (v == u);
        in << u << " " << v << " " << disd(default_rnd) << "\n";
    }
}

int main(int, char* argv[])
{
    Init();
    const unsigned int n = GetRandom(2, MayBe(atoi(argv[1]), maxn));
    const unsigned int u = GetRandom(1, MayBe(atoi(argv[2]), maxu));
    const unsigned int q = GetRandom(1, MayBe(atoi(argv[3]), maxq));
    ofstream in(argv[5]);
    in << n << " " << maxd << " " << u << " " << q << "\n";
    createArray(0, MayBe(atoi(argv[4]), maxh), n, ostream_iterator<unsigned int>(in, " "));
    in.put('\n');
    createChange(in, n, u, maxd);
    createQuery(in, n, u, q);
    return 0;
}

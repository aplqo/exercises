#include "debug_tools/generator.h"
#include <algorithm>
#include <cstring>
#include <fstream>
#include <iterator>
#include <unordered_set>
using namespace std;
using namespace apdebug::generator;
const int maxn = 5000;

void createGraph(ofstream& in, const unsigned int n, const unsigned int m)
{
    uniform_int_distribution<unsigned int> dis(1, n);
    unordered_set<unsigned int>* ed = new unordered_set<unsigned int>[n + 1];
    for (unsigned int i = 0; i < m; ++i)
    {
        unsigned int u, v;
        do
        {
            u = dis(default_rnd);
            v = dis(default_rnd);
        } while (u == v || ed[u].count(v));
        ed[u].insert(v);
        ed[v].insert(u);
    }
    for (auto i = ed + 1; i < ed + 1 + n; ++i)
    {
        in << i->size() << " ";
        copy(i->cbegin(), i->cend(), ostream_iterator<unsigned int>(in, " "));
        in.put('\n');
    }
}

int main(int, char* argv[])
{
    Init();
    const unsigned int n = GetRandom(2, MayBe(atoi(argv[1]), maxn));
    const unsigned int m = GetRandom(1u, min(MayBe<unsigned int>(atoi(argv[2]), n * (n - 1) / 2), n * (n - 1) / 2));
    ofstream in(argv[3]);
    in << n << "\n";
    createGraph(in, n, m);
    return 0;
}
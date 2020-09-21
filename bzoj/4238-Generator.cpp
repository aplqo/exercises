#include "debug_tools/generator.h"
#include <cstring>
#include <fstream>
#include <random>
using namespace std;
using namespace apdebug::generator;
const int maxn = 1e5, maxm = 2e5;

void createGraph(ofstream& os, const unsigned int n, const unsigned int m)
{
    uniform_int_distribution<unsigned int> dis(1, n);
    for (unsigned int i = 0; i < m; ++i)
    {
        unsigned int u, v;
        do
        {
            u = dis(default_rnd);
            v = dis(default_rnd);
        } while (u == v);
        os << u << " " << v << "\n";
    }
}
int main(int, char* argv[])
{
    Init();
    const unsigned int n = GetRandom(2, MayBe(atoi(argv[1]), maxn));
    const unsigned int m = GetRandom(1, MayBe(atoi(argv[2]), maxm));
    ofstream in(argv[3]);
    in << n << " " << m << "\n";
    createGraph(in, n, m);
    return 0;
}
#include "debug_tools/generator.h"
#include <algorithm>
#include <cstring>
#include <fstream>
#include <random>
using namespace std;
using namespace apdebug::generator;
const int maxn = 5e4, maxm = 5e4, maxk = 2e4, maxt = 10, maxw = 2000, maxc = 2e4;

void createTrie(ofstream& in, const unsigned int k, const unsigned int mw)
{
    uniform_int_distribution<unsigned int> disw(1, mw);
    for (unsigned int i = 2; i <= k; ++i)
        in << GetRandom(1u, i - 1) << " " << i << " " << disw(default_rnd) << "\n";
}
void createTree(ofstream& in, const unsigned int n, const unsigned int k, const unsigned int ml)
{
    uniform_int_distribution<unsigned int> disp(1, k), disl(0, ml);
    for (unsigned int i = 2; i <= n; ++i)
        in << GetRandom(1u, i - 1) << " " << i << " " << disl(default_rnd) << " " << disp(default_rnd) << "\n";
}
void createGraph(ofstream& in, const unsigned int m, const unsigned int k, const unsigned int n, const unsigned int ml)
{
    uniform_int_distribution<unsigned int> disv(1, n), disk(1, k), disl(0, ml);
    createTree(in, n, k, ml);
    for (unsigned int i = n - 1; i < m; ++i)
        in << disv(default_rnd) << " " << disv(default_rnd) << " " << disl(default_rnd) << " " << disk(default_rnd) << "\n";
}
int main(int, char* argv[])
{
    Init();
    const unsigned int t = GetRandom(1, MayBe(atoi(argv[1]), maxt));
    const unsigned int mw = MayBe(atoi(argv[5]), maxw), mc = MayBe(atoi(argv[6]), maxc), mm = MayBe(atoi(argv[2]), maxm);
    uniform_int_distribution<unsigned int> disn(2, MayBe(atoi(argv[2]), maxn)), disk(1, MayBe(atoi(argv[4]), maxk));
    ofstream in(argv[7]);
    in << t << "\n";
    for (unsigned int i = 0; i < t; ++i)
    {
        const unsigned int n = disn(default_rnd), m = GetRandom(n - 1, max(mm, n - 1)), k = disk(default_rnd);
        in << n << " " << m << " " << k << "\n";
        createGraph(in, m, k, n, mc);
        createTrie(in, k, mw);
    }
    return 0;
}
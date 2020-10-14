#include "debug_tools/generator.h"
#include <cstring>
#include <fstream>
#include <random>
using namespace std;
using namespace apdebug::generator;
const int maxn = 1e5, maxm = 2e5, maxc = 1e6;

void createTree(ofstream& in, const unsigned int n, const unsigned int c)
{
    uniform_int_distribution<unsigned int> disc(1, c);
    for (unsigned int i = 2; i <= n; ++i)
        in << GetRandom(1u, i - 1) << " " << i << " " << disc(default_rnd) << "\n";
}
void createGraph(ofstream& in, const unsigned int n, const unsigned int m, const unsigned int c)
{
    uniform_int_distribution<unsigned int> disp(1, n), disc(1, c);
    for (unsigned int i = 0; i < m; ++i)
        in << disp(default_rnd) << " " << disp(default_rnd) << " " << disc(default_rnd) << "\n";
}
int main(int, char* argv[])
{
    Init();
    const unsigned int n = GetRandom(1, MayBe(atoi(argv[2]), maxn));
    const unsigned int m = GetRandom(n - 1, max<unsigned int>(n - 1, MayBe(atoi(argv[2]), maxm)));
    const unsigned int c = MayBe(atoi(argv[3]), maxc);
    ofstream in(argv[4]);
    in << n << " " << m << "\n";
    createTree(in, n, c);
    createGraph(in, n, m - (n - 1), c);
    return 0;
}
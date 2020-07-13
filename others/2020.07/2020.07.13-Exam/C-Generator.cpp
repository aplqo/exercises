#include "debug_tools/generator.h"
#include <algorithm>
#include <fstream>
#include <random>
using namespace std;
using apdebug::generator::default_rnd;
using apdebug::generator::GetRandom;
using apdebug::generator::Init;
using apdebug::generator::MayBe;
const int maxn = 5e5, maxm = 1e5;

void createTree(ofstream& f, const unsigned int n)
{
    for (unsigned int i = 2; i <= n; ++i)
    {
        const unsigned int t = GetRandom(1u, i - 1);
        f << t << " " << t << " " << i << " " << i << endl;
    }
}
void createEdge(ofstream& f, const unsigned int n, const unsigned int m)
{
    uniform_int_distribution<unsigned int> dis(1, n);
    for (unsigned int i = 0; i < m; ++i)
    {
        for (unsigned int j = 0; j < 2; ++j)
        {
            unsigned int l = dis(default_rnd), r = dis(default_rnd);
            f << min(l, r) << " " << max(l, r) << " ";
        }
        f << endl;
    }
}
int main(int, char* argv[])
{
    Init();
    const unsigned int n = GetRandom(1, MayBe(atoi(argv[1]), maxn));
    const unsigned int m = GetRandom(n - 1, MayBe<unsigned int>(atoi(argv[2]), maxm));
    ofstream f(argv[3]);
    f << n << " " << m << " " << GetRandom(1u, n) << endl;
    createTree(f, n);
    createEdge(f, n, m - (n - 1));
    return 0;
}
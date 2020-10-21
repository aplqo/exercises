#include "debug_tools/generator.h"
#include <algorithm>
#include <cstring>
#include <fstream>
#include <iterator>
#include <random>
using namespace std;
using namespace apdebug::generator;
const int maxn = 2e5, maxm = 4e5, maxq = 2e5;
typedef uniform_int_distribution<unsigned int> distri;

inline unsigned int genNeq(distri& d, const unsigned int v)
{
    unsigned int ret;
    do
    {
        ret = d(default_rnd);
    } while (ret == v);
    return ret;
}
void createTree(ofstream& in, const unsigned int n)
{
    for (unsigned int i = 2; i <= n; ++i)
        in << i << " " << GetRandom(1u, i - 1) << "\n";
}
void createEdges(ofstream& in, const unsigned int n, const unsigned int m)
{
    distri dis(1, n);
    for (unsigned int i = 0; i < m; ++i)
    {
        const unsigned int v = dis(default_rnd);
        in << v << " " << genNeq(dis, v) << "\n";
    }
}
int main(int, char* argv[])
{
    Init();
    const unsigned int n = GetRandom(2, MayBe(atoi(argv[1]), maxn));
    const unsigned int m = GetRandom(n - 1, max<unsigned int>(n - 1, MayBe(atoi(argv[2]), maxm)));
    const unsigned int q = GetRandom(1, MayBe(atoi(argv[3]), maxq));
    ofstream in(argv[4]);
    in << n << " " << m << " " << q << "\n";
    createTree(in, n);
    createEdges(in, n, m + q - (n - 1));
    return 0;
}
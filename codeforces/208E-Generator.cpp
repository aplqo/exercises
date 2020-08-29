#include "debug_tools/generator.h"
#include <algorithm>
#include <fstream>
#include <functional>
#include <numeric>
using namespace std;
using namespace apdebug::generator;
const int maxn = 1e5, maxm = 1e5;

void createTree(ofstream& in, const unsigned int n)
{
    static unsigned int shf[maxn + 10];
    iota(shf, shf + 1 + n, 0);
    shuffle(shf + 1, shf + 1 + n, default_rnd);
    for (unsigned int i = 1; i <= n; ++i)
        in << shf[GetRandom(0u, i - 1)] << " ";
    in << "\n";
}
void createQuery(ofstream& in, const unsigned int n, const unsigned int q)
{
    uniform_int_distribution<unsigned int> disp(1, n), disv(1, n);
    for (unsigned int i = 0; i < q; ++i)
        in << disv(default_rnd) << " " << disp(default_rnd) << "\n";
}
int main(int, char* argv[])
{
    Init();
    const unsigned int n = GetRandom(1, MayBe(atoi(argv[1]), maxn));
    const unsigned int q = GetRandom(1, MayBe(atoi(argv[2]), maxm));
    ofstream in(argv[3]);
    in << n << "\n";
    createTree(in, n);
    in << q << "\n";
    createQuery(in, n, q);
    return 0;
}
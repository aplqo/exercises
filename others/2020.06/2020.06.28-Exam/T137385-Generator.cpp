#include "debug_tools/generator.h"
#include <cstring>
#include <fstream>
#include <random>
using namespace std;
using apdebug::generator::default_rnd;
using apdebug::generator::GetRandom;
using apdebug::generator::Init;
using apdebug::generator::MayBe;
const int maxn = 1e5, maxm = 1e5;

void dfsGen(ofstream& f, const unsigned int l, const unsigned int r)
{
    if (l == r)
        return;
    const unsigned int m = GetRandom(l, r - 1);
    f << m << endl;
    dfsGen(f, l, m);
    dfsGen(f, m + 1, r);
}
void createQuery(ofstream& f, const unsigned int n, const unsigned int m)
{
    uniform_int_distribution<unsigned int> dis(1, n);
    for (unsigned int i = 0; i < m; ++i)
    {
        const unsigned int l = dis(default_rnd);
        f << l << " " << dis(default_rnd, decltype(dis)::param_type(l, n)) << endl;
    }
}

int main(int argc, char* argv[])
{
    Init();
    const unsigned int n = GetRandom(1, MayBe(atoi(argv[1]), maxn));
    const unsigned int m = GetRandom(1, MayBe(atoi(argv[2]), maxm));
    ofstream in(argv[3]);
    in << n << " " << m << endl;
    dfsGen(in, 1, n);
    createQuery(in, n, m);
    return 0;
}
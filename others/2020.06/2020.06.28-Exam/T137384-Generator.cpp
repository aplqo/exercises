#include "debug_tools/generator.h"
#include <algorithm>
#include <cstring>
#include <fstream>
using namespace std;
using apdebug::generator::default_rnd;
using apdebug::generator::GetRandom;
using apdebug::generator::Init;
using apdebug::generator::MayBe;
const int maxn = 1000, maxq = 5e5, maxv = 1e6;

void createVal(ofstream& f, const unsigned int n, const unsigned int mv)
{
    uniform_int_distribution<unsigned int> disv(0, mv);
    for (unsigned int i = 1; i <= n; ++i)
    {
        for (unsigned int j = 1; j <= i; ++j)
            f << disv(default_rnd) << " ";
        f << endl;
    }
}
void createQuery(ofstream& f, const unsigned int n, const unsigned int q)
{
    uniform_int_distribution<unsigned int> disl(1, n);
    for (unsigned int i = 0; i < q; ++i)
    {
        const unsigned int l = disl(default_rnd);
        f << l << " " << GetRandom(1u, l) << endl;
    }
}
int main(int argc, char* argv[])
{
    Init();
    const unsigned int n = GetRandom(1, MayBe(atoi(argv[1]), maxn));
    const unsigned int q = GetRandom(1, MayBe(atoi(argv[2]), maxq));
    ofstream in(argv[4]);
    in << n << " " << q << endl;
    createVal(in, n, MayBe(atoi(argv[3]), maxv));
    createQuery(in, n, q);
    return 0;
}
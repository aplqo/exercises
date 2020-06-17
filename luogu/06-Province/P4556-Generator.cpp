#include "debug_tools/generator.h"
#include <cstring>
#include <fstream>
#include <random>
using namespace std;
using apdebug::generator::GetRandom;
using apdebug::generator::Init;
using apdebug::generator::MayBe;
const int maxn = 1e5, maxm = 1e5, maxz = 1e5;

mt19937_64 rnd;

void createTree(ofstream& f, const unsigned int n)
{
    for (unsigned int i = 2; i <= n; ++i)
        f << GetRandom(1u, i - 1) << " " << i << endl;
}
void createQuery(ofstream& f, const unsigned int n, const unsigned int q, const unsigned int mz)
{
    uniform_int_distribution<unsigned int> disz(1, mz), disp(1, n);
    for (unsigned int i = 0; i < q; ++i)
        f << disp(rnd) << " " << disp(rnd) << " " << disz(rnd) << endl;
}

int main(int argc, char* argv[])
{
    Init();
    Init(rnd);
    const unsigned int n = GetRandom(1, MayBe(atoi(argv[1]), maxn));
    const unsigned int m = GetRandom(1, MayBe(atoi(argv[2]), maxm));
    ofstream in(argv[4]);
    in << n << " " << m << endl;
    createTree(in, n);
    createQuery(in, n, m, MayBe(atoi(argv[3]), maxz));
    return 0;
}
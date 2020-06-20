#include "debug_tools/generator.h"
#include <algorithm>
#include <cstring>
#include <fstream>
#include <iterator>
#include <random>
using namespace std;
using apdebug::generator::GetRandom;
using apdebug::generator::Init;
using apdebug::generator::MayBe;
const int maxn = 3e5, maxm = 3e5;

mt19937_64 rnd;

void createTree(ofstream& f, const unsigned int n)
{
    for (unsigned int i = 2; i <= n; ++i)
        f << GetRandom(1u, i - 1) << " " << i << endl;
}
void createPath(ofstream& f, const unsigned int n, const unsigned int m)
{
    uniform_int_distribution<unsigned int> dis(1, n);
    for (unsigned int i = 0; i < m; ++i)
        f << dis(rnd) << " " << dis(rnd) << endl;
}
void createW(ofstream& f, const unsigned int n)
{
    uniform_int_distribution<unsigned int> dis(0, n);
    generate_n(ostream_iterator<unsigned int>(f, " "), n, [&dis]() { return dis(rnd); });
    f << endl;
}
int main(int argc, char* argv[])
{
    Init();
    Init(rnd);
    const unsigned int n = GetRandom(2, MayBe(atoi(argv[1]), maxn));
    const unsigned int m = GetRandom(1, MayBe(atoi(argv[2]), maxm));
    ofstream in(argv[3]);
    in << n << " " << m << endl;
    createTree(in, n);
    createW(in, n);
    createPath(in, n, m);
    return 0;
}
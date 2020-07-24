#include "debug_tools/generator.h"
#include <algorithm>
#include <cstring>
#include <fstream>
#include <iterator>
#include <random>
using namespace std;
using namespace apdebug::generator;
const int maxn = 40000, maxm = 50000, maxa = 1e9;

void createSeq(ofstream& f, const unsigned int n, const unsigned int mv)
{
    uniform_int_distribution<unsigned int> dis(1, mv);
    generate_n(ostream_iterator<unsigned int>(f, " "), n, [&dis]() { return dis(default_rnd); });
    f << endl;
}
void createQuery(ofstream& f, const unsigned int q, const unsigned int n)
{
    uniform_int_distribution<unsigned int> dis(1, n);
    for (unsigned int i = 0; i < q; ++i)
        f << dis(default_rnd) << " " << dis(default_rnd) << endl;
}
int main(int, char* argv[])
{
    Init();
    const unsigned int n = GetRandom(1, MayBe(atoi(argv[1]), maxn));
    const unsigned int m = GetRandom(1, MayBe(atoi(argv[2]), maxm));
    ofstream in(argv[4]);
    in << n << " " << m << endl;
    createSeq(in, n, MayBe(atoi(argv[3]), maxa));
    createQuery(in, m, n);
    return 0;
}
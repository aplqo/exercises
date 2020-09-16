#include "debug_tools/generator.h"
#include <algorithm>
#include <cstring>
#include <fstream>
#include <functional>
#include <iterator>
#include <random>
using namespace std;
using namespace apdebug::generator;
const int maxn = 5000, maxm = 5000, maxk = 10;

void createKey(ofstream& in, const unsigned int k)
{
    uniform_int_distribution<unsigned int> dis(0, 1);
    generate_n(ostream_iterator<unsigned int>(in, " "), k, bind(dis, ref(default_rnd)));
}
void createVertex(ofstream& in, const unsigned int n, const unsigned int k)
{
    for (unsigned int i = 0; i < n; ++i)
    {
        createKey(in, k);
        in << "\n";
    }
}
void createEdge(ofstream& in, const unsigned int n, const unsigned int m, const unsigned int k)
{
    uniform_int_distribution<unsigned int> disn(1, n);
    for (unsigned int i = 0; i < m; ++i)
    {
        in << disn(default_rnd) << " " << disn(default_rnd) << " ";
        createKey(in, k);
        in << "\n";
    }
}
int main(int, char* argv[])
{
    Init();
    const unsigned int n = GetRandom(1, MayBe(atoi(argv[1]), maxn));
    const unsigned int m = GetRandom(1, MayBe(atoi(argv[2]), maxm));
    const unsigned int k = GetRandom(1, MayBe(atoi(argv[3]), maxk));
    ofstream in(argv[4]);
    in << n << " " << m << " " << k << "\n";
    createVertex(in, n, k);
    createEdge(in, n, m, k);
    return 0;
}
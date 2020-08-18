#include "debug_tools/generator.h"
#include <algorithm>
#include <fstream>
#include <functional>
#include <iterator>
#include <string>
using namespace std;
using namespace apdebug::generator;
const int maxn = 500000, maxd = 20, maxw = 1000;

void createCost(ofstream& in, const unsigned int n, const unsigned int mw)
{
    uniform_int_distribution<unsigned int> disw(1, mw);
    generate_n(ostream_iterator<unsigned int>(in, " "), n, bind(disw, default_rnd));
    in << endl;
}
void createKey(ofstream& in, const unsigned int n)
{
    static bool v[maxn + 10];
    uniform_int_distribution<unsigned int> dis(0, 1);
    generate(v, v + n, bind(dis, default_rnd));
    in << count(v, v + n, true) << endl;
    for (unsigned int i = 0; i < n; ++i)
        if (v[i])
            in << (i + 1) << " ";
    in << endl;
}
void createTree(ofstream& in, const unsigned int n)
{
    for (unsigned int i = 2; i <= n; ++i)
        in << i << " " << GetRandom(1u, i - 1) << endl;
}
int main(int, char* argv[])
{
    Init();
    const unsigned int n = GetRandom(1, MayBe(atoi(argv[1]), maxn));
    ofstream in(argv[4]);
    in << n << " " << GetRandom(1, MayBe(atoi(argv[2]), maxd)) << endl;
    createCost(in, n, MayBe(atoi(argv[3]), maxw));
    createKey(in, n);
    createTree(in, n);
    return 0;
}
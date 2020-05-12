#include "debug_tools/generator.h"
#include <algorithm>
#include <fstream>
#include <limits>
#include <numeric>
#include <random>
#include <string>
using namespace std;
using namespace apdebug::generator;
const int maxn = 200, maxt = 20, maxs = 20;
const unsigned long long maxk = numeric_limits<unsigned long long>::max();
const char ch[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789 ";

mt19937_64 rnd;

void createData(const unsigned int n, ofstream& f)
{
    unsigned int d[maxn + 10];
    iota(d, d + n, 1);
    shuffle(d, d + n, rnd);
    for (unsigned int* i = d; i < d + n; ++i)
        f << *i << " ";
    f << endl;
}
void createString(const unsigned int l, const unsigned int k, ofstream& f)
{
    constexpr static unsigned int cn = sizeof(ch) - 2;
    uniform_int_distribution<unsigned int> dis(0, cn);
    f << k << " ";
    for (unsigned int i = 0; i < l; ++i)
        f << ch[dis(rnd)];
    f << endl;
}
int main(int argc, char* argv[])
{
    Init();
    rnd.seed(random_device()());
    ofstream in(argv[5]);
    const unsigned int t = GetRandom(1, MayBe(stoi(argv[2]), maxt));
    const unsigned long long mk = MayBe(stoull(argv[4]), maxk);
    uniform_int_distribution<unsigned int> diss(1, MayBe(stoi(argv[3]), maxs));
    uniform_int_distribution<unsigned int> disn(1, MayBe(stoi(argv[1]), maxn));
    for (unsigned int i = 0; i < t; ++i)
    {
        const unsigned int n = disn(rnd);
        uniform_int_distribution<unsigned long long> disk(1, mk);
        uniform_int_distribution<unsigned int> disl(1, n);
        in << n << endl;
        createData(n, in);
        unsigned int s = diss(rnd);
        for (unsigned int i = 0; i < s; ++i)
            createString(disl(rnd), disk(rnd), in);
        in << "0" << endl;
    }
    in << "0" << endl;
    return 0;
}
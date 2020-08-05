#include "debug_tools/generator.h"
#include <algorithm>
#include <cstring>
#include <fstream>
#include <iterator>
#include <random>
#include <set>
using namespace std;
using namespace apdebug::generator;
const int maxn = 100000, maxd = 1000000000;

void createSeq(ofstream& in, const unsigned int n, const unsigned int md)
{
    unsigned int lst = GetRandom(0u, md - n);
    in << lst << endl;
    for (unsigned int i = 1; i < n; ++i)
    {
        const unsigned int p = GetRandom(lst + 1, md + i - n);
        in << p << endl;
        lst = p;
    }
}
int main(int, char* argv[])
{
    Init();
    const unsigned int n = GetRandom(1, MayBe(atoi(argv[1]), maxn));
    ofstream in(argv[3]);
    if (n == 1)
    {
        in << "1 0" << endl;
        return 0;
    }
    in << n << " " << GetRandom(1u, n / 2) << endl;
    createSeq(in, n, MayBe(atoi(argv[2]), maxd));
    return 0;
}
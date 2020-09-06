#include "debug_tools/generator.h"
#include <cstring>
#include <fstream>
#include <random>
using namespace std;
using namespace apdebug::generator;
const int maxn = 2000;

void createTree(ofstream& in, const unsigned int n)
{
    for (unsigned int i = 2; i <= n; ++i)
        in << GetRandom(1u, i - 1) << " ";
    in << "\n";
}
int main(int, char* argv[])
{
    Init();
    const unsigned int n = GetRandom(2, MayBe(atoi(argv[1]), maxn));
    ofstream in(argv[2]);
    in << n << "\n";
    createTree(in, n);
    return 0;
}
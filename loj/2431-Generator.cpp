#include "debug_tools/generator.h"
#include <cstring>
#include <fstream>
using namespace std;
using namespace apdebug::generator;
const int maxn = 5000;

void createTree(ofstream& in, const unsigned int n)
{
    for (unsigned int i = 2; i <= n; ++i)
        in << i << " " << GetRandom(1u, i - 1) << "\n";
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
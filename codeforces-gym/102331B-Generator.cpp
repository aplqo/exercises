#include "debug_tools/generator.h"
#include <algorithm>
#include <fstream>
#include <iterator>
using namespace std;
using namespace apdebug::generator;
const unsigned long long maxv = (1ull << 60) - 1;
const int maxn = 3e5;

int main(int, char* argv[])
{
    Init();
    const unsigned int n = GetRandom(1, MayBe(atoi(argv[1]), maxn));
    const unsigned long long mv = GetRandom(1ull, MayBe(stoull(argv[2]), maxv));
    ofstream in(argv[3]);
    in << n << " " << GetRandom(0ull, mv) << "\n";
    createArray(0ull, mv, n, ostream_iterator<unsigned long long>(in, " "));
    in.put('\n');
    return 0;
}
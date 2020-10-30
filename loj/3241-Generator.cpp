#include "debug_tools/generator.h"
#include <algorithm>
#include <cstring>
#include <fstream>
#include <iterator>
using namespace std;
using namespace apdebug::generator;
const int maxn = 5e4;
const char* ch = "abcdefghijklmnopqrstuvwxyz";

void createStr(ofstream& in, const unsigned int n, const char* ch)
{
    const size_t len = strlen(ch);
    uniform_int_distribution<size_t> dis(0, len - 1);
    generate_n(ostream_iterator<char>(in), n, [&dis, ch]() { return ch[dis(default_rnd)]; });
    in.put('\n');
}
void createTree(ofstream& in, const unsigned int n)
{
    for (unsigned int i = 2; i <= n; ++i)
        in << i << " " << GetRandom(1u, i - 1) << "\n";
}
int main(int, char* argv[])
{
    Init();
    const unsigned int n = GetRandom(1, MayBe(atoi(argv[1]), maxn));
    ofstream in(argv[3]);
    in << n << "\n";
    createStr(in, n, MayBe<const char*>(argv[2], ch));
    createTree(in, n);
    return 0;
}
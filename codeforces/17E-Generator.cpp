#include "debug_tools/generator.h"
#include <algorithm>
#include <cstring>
#include <fstream>
#include <iterator>
using namespace std;
using namespace apdebug::generator;
const int maxn = 2e6;

void createString(ofstream& in, const unsigned int n, const char ch[], const size_t sch)
{
    uniform_int_distribution<size_t> dis(0, sch - 1);
    generate_n(ostream_iterator<char>(in), n, [&dis, ch]() { return ch[dis(default_rnd)]; });
    in << "\n";
}
int main(int, char* argv[])
{
    Init();
    const unsigned int n = GetRandom(1, MayBe(atoi(argv[1]), maxn));
    ofstream in(argv[3]);
    in << n << "\n";
    createString(in, n, argv[2], strlen(argv[2]));
    return 0;
}
#include "debug_tools/generator.h"
#include <algorithm>
#include <cstring>
#include <fstream>
#include <iterator>
#include <random>
using namespace std;
using namespace apdebug::generator;
const int maxn = 500000;

void createStr(ofstream& f, const unsigned int l, const char ch[])
{
    uniform_int_distribution<unsigned int> dis(0, strlen(ch) - 1);
    generate_n(ostream_iterator<char>(f), l, [&dis, ch]() { return ch[dis(default_rnd)]; });
    f << endl;
}
int main(int, char* argv[])
{
    Init();
    const unsigned int n = GetRandom(1, MayBe(atoi(argv[1]), maxn));
    ofstream in(argv[3]);
    in << n << endl;
    createStr(in, n, argv[2]);
    return 0;
}
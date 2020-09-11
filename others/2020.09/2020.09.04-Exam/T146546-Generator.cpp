#include "debug_tools/generator.h"
#include <algorithm>
#include <cstring>
#include <fstream>
#include <functional>
#include <iterator>
using namespace std;
using namespace apdebug::generator;
const int maxn = 30;

void createSeq(ofstream& in, const unsigned int n)
{
    uniform_int_distribution<unsigned int> disp(0, 1);
    generate_n(ostream_iterator<unsigned int>(in), n, bind(disp, ref(default_rnd)));
    in << "\n";
}
int main(int, char* argv[])
{
    Init();
    const unsigned int n = GetRandom(1, MayBe(atoi(argv[1]), maxn));
    ofstream in(argv[2]);
    in << n << "\n";
    for (unsigned int i = 0; i < n; ++i)
        createSeq(in, n);
    return 0;
}
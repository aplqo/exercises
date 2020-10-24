#include "debug_tools/generator.h"
#include <algorithm>
#include <cstring>
#include <fstream>
#include <functional>
#include <iterator>
using namespace std;
using namespace apdebug::generator;
const int maxn = 1e6, maxk = 1e9;

void createSeq(ofstream& in, const unsigned int n, const unsigned int ma)
{
    uniform_int_distribution<unsigned int> disa(0, ma);
    generate_n(ostream_iterator<unsigned int>(in, " "), n, bind(disa, ref(default_rnd)));
    in.put('\n');
}
int main(int, char* argv[])
{
    Init();
    const unsigned int n = GetRandom(1, MayBe(atoi(argv[1]), maxn));
    const unsigned int ma = MayBe(atoi(argv[2]), maxk);
    ofstream in(argv[3]);
    in << n << " " << GetRandom(0u, ma) << "\n";
    createSeq(in, n, ma);
    return 0;
}
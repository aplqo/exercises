#include "debug_tools/generator.h"
#include <algorithm>
#include <fstream>
#include <functional>
#include <iterator>
#include <random>
using namespace std;
using namespace apdebug::generator;
const int maxn = 5000;

void createX(ofstream& in, const unsigned int n, const unsigned int mx)
{
    uniform_int_distribution<unsigned int> disx(0, min(mx, n - 1));
    generate_n(ostream_iterator<unsigned int>(in, " "), n, bind(disx, ref(default_rnd)));
    in.put('\n');
}
int main(int, char* argv[])
{
    Init();
    const unsigned int n = GetRandom(1, MayBe(atoi(argv[1]), maxn));
    ofstream in(argv[3]);
    in << n << "\n";
    createX(in, n, MayBe<unsigned int>(atoi(argv[2]), n - 1));
    return 0;
}
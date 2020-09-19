#include "debug_tools/generator.h"
#include <algorithm>
#include <cstring>
#include <fstream>
#include <functional>
#include <iterator>
using namespace std;
using namespace apdebug::generator;
const int maxn = 25, maxm = 25;

void createVal(ofstream& in, const unsigned int m)
{
    uniform_int_distribution<unsigned int> dis('0', '1');
    generate_n(ostream_iterator<char>(in, " "), m, bind(dis, ref(default_rnd)));
    in << "\n";
}
int main(int, char* argv[])
{
    Init();
    const unsigned int n = GetRandom(1, MayBe(atoi(argv[2]), maxn));
    const unsigned int m = GetRandom(1, MayBe(atoi(argv[3]), maxm));
    ofstream in(argv[4]);
    in << argv[1][GetRandom<size_t>(0, strlen(argv[1]) - 1)] << " " << n << " " << m << "\n";
    for (unsigned int i = 0; i < n; ++i)
        createVal(in, m);
    createVal(in, m);
    return 0;
}
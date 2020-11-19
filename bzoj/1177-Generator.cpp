#include "debug_tools/generator.h"
#include <algorithm>
#include <cstring>
#include <fstream>
#include <functional>
#include <iterator>
#include <random>
using namespace std;
using namespace apdebug::generator;
const int maxn = 1500, maxv = 500;

void createMatrix(ofstream& in, const unsigned int n, const unsigned int m, const unsigned int mw)
{
    uniform_int_distribution<unsigned int> dis(0, mw);
    for (unsigned int i = 0; i < n; ++i)
    {
        generate_n(ostream_iterator<unsigned int>(in, " "), m, bind(dis, ref(default_rnd)));
        in.put('\n');
    }
}
int main(int, char* argv[])
{
    Init();
    const unsigned int n = GetRandom(1, MayBe(atoi(argv[1]), maxn)), mm = n > 2 ? 1 : 3;
    const unsigned int m = GetRandom(mm, max<unsigned int>(mm, MayBe(atoi(argv[2]), maxn)));
    ofstream in(argv[4]);
    in << n << " " << m << " " << GetRandom(1u, max(n, m) / 3) << "\n";
    createMatrix(in, n, m, MayBe(atoi(argv[3]), maxv));
    return 0;
}
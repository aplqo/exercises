#include "debug_tools/generator.h"
#include <algorithm>
#include <cstring>
#include <fstream>
#include <iterator>
#include <numeric>
#include <set>
using namespace std;
using namespace apdebug::generator;
const int maxn = 30, maxm = 30, maxc = 10;
const unsigned int maxs = 250;

void createC(ofstream& in, const unsigned int c, const unsigned int sum)
{
    set<unsigned int> buf;
    buf.insert(sum);
    uniform_int_distribution<unsigned int> dis(1, sum - 1);
    while (buf.size() != c)
        buf.insert(dis(default_rnd));
    adjacent_difference(buf.begin(), buf.end(), ostream_iterator<unsigned int>(in, " "));
    in << "\n";
}
int main(int, char* argv[])
{
    Init();
    const unsigned int n = GetRandom(1, MayBe(atoi(argv[1]), maxn));
    const unsigned int m = GetRandom(1, MayBe(atoi(argv[2]), maxm));
    const unsigned int c = GetRandom(1u, min<unsigned int>(MayBe(atoi(argv[3]), maxc), n * m));
    ofstream in(argv[4]);
    in << n << " " << m << " " << c << "\n";
    createC(in, c, GetRandom(c, min(n * m, maxs)));
    return 0;
}
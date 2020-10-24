#include "debug_tools/generator.h"
#include <algorithm>
#include <cstring>
#include <fstream>
#include <functional>
#include <iterator>
using namespace std;
using namespace apdebug::generator;
const int maxn = 200000, maxm = 200000;

void createString(ofstream& in, const unsigned int n, const char* ch)
{
    const size_t l = strlen(ch);
    uniform_int_distribution<size_t> dis(0, l - 1);
    generate_n(ostream_iterator<char>(in), n, [&dis, ch]() { return ch[dis(default_rnd)]; });
    in.put('\n');
}
void createQuery(ofstream& in, const unsigned int n, const unsigned int m)
{
    uniform_int_distribution<unsigned int> disp(1, n);
    for (unsigned int i = 0; i < m; ++i)
    {
        const unsigned int p1 = disp(default_rnd), p2 = disp(default_rnd);
        in << p1 << " " << p2 << " " << GetRandom(1u, n + 1 - max(p1, p2)) << "\n";
    }
}

int main(int, char* argv[])
{
    Init();
    const unsigned int n = GetRandom(1, MayBe(atoi(argv[1]), maxn));
    const unsigned int m = GetRandom(1, MayBe(atoi(argv[2]), maxm));
    ofstream in(argv[4]);
    in << n << " " << m << "\n";
    createString(in, n, argv[3]);
    createQuery(in, n, m);
    return 0;
}
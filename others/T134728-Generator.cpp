#include "debug_tools/generator.h"
#include <algorithm>
#include <cstring>
#include <fstream>
#include <functional>
#include <iterator>
#include <random>
#include <set>
using namespace std;
using apdebug::generator::GetRandom;
using apdebug::generator::Init;
using apdebug::generator::MayBe;
const int maxn = 100000, maxm = 100000;
const long long maxp = 1e10;

set<unsigned long long> s;
unsigned long long h[maxn], p[maxm];
mt19937_64 rnd;

int main(int argc, char* argv[])
{
    Init();
    Init(rnd);
    const unsigned int n = GetRandom(1, MayBe(atoi(argv[1]), maxn));
    const unsigned int m = GetRandom(1, MayBe(atoi(argv[2]), maxm));
    ofstream in(argv[4]);
    uniform_int_distribution<unsigned long long> dis(1, MayBe(atoll(argv[3]), maxp));
    const auto f = [&dis]() -> unsigned long long {
        unsigned long long t;
        do
        {
            t = dis(rnd);
        } while (s.find(t) != s.end());
        s.insert(t);
        return t;
    };
    in << n << " " << m << endl;

    generate(h, h + n, f);
    sort(h, h + n);
    copy(h, h + n, ostream_iterator<unsigned long long>(in, " "));
    in << endl;
    s.clear();

    generate(p, p + m, f);
    sort(p, p + m);
    copy(p, p + m, ostream_iterator<unsigned long long>(in, " "));
    in << endl;
    return 0;
}
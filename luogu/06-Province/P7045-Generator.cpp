#include "debug_tools/generator.h"
#include <algorithm>
#include <cstring>
#include <fstream>
#include <functional>
#include <iterator>
#include <numeric>
#include <random>
using namespace std;
using namespace apdebug::generator;
const int maxn = 5e4, maxq = 1e5, maxt = 1e5;

unsigned int qs[maxt + 10], typ[maxn + 10];

void createN(const unsigned int t, unsigned int mq)
{
    static unsigned int buf[maxt + 10];
    {
        uniform_int_distribution<unsigned int> dis(1, mq);
        generate_n(buf + 1, t - 1, bind(dis, ref(default_rnd)));
    }
    buf[t] = mq;
    sort(buf, buf + t + 1);
    for (unsigned int i = 1; i <= t; ++i)
        qs[i - 1] = buf[i] - buf[i - 1];
}
void createType(ofstream& in, const unsigned int n, const unsigned int mc)
{
    uniform_int_distribution<unsigned int> dis(0, mc);
    generate_n(typ, n, bind(dis, ref(default_rnd)));
    copy(typ, typ + n, ostream_iterator<unsigned int>(in, " "));
    in.put('\n');
}
bool checkSolution(const unsigned int n)
{
    static unsigned int cnt[maxn + 10];
    memset(cnt, 0, sizeof(cnt));
    for_each(typ, typ + n, [](const unsigned int i) { ++cnt[i]; });
    for (unsigned int i = 0; i <= n; ++i)
        if (cnt[i] > n + 1 - cnt[i])
            return false;
    return true;
}

int main(int, char* argv[])
{
    Init();
    const unsigned int t = GetRandom(1, MayBe(atoi(argv[1]), maxt)), mc = MayBe(atoi(argv[3]), maxn);
    ofstream in(argv[4]);
#ifdef SOLUTION
    ofstream ans(argv[5]);
#endif
    in << t << "\n";
    createN(t, GetRandom(t, max(t, MayBe<unsigned int>(atoi(argv[2]), maxq))));
    for (unsigned int i = 0; i < t; ++i)
    {
        const unsigned int n = (qs[i] + 2) / 2;
        in << n << " " << qs[i] << "\n";
        createType(in, n, min(mc, n));
#ifdef SOLUTION
        ans << (checkSolution(n) ? "Y" : "N") << "\n";
#endif
    }
    return 0;
}
#include "debug_tools/generator.h"
#include <algorithm>
#include <cctype>
#include <fstream>
#include <iterator>
using namespace std;
using namespace apdebug::generator;
const int maxn = 300000, maxm = 300000;
const long long maxv = 1e18;

long long maybe(const char* s, const long long def)
{
    return isdigit(s[0]) ? atoll(s) : def;
}
void createH(ofstream& in, const unsigned int n, const long long mnh, const long long mxh)
{
    uniform_int_distribution<long long> dis(mnh, mxh);
    generate_n(ostream_iterator<long long>(in, " "), n, [&dis]() { return dis(default_rnd); });
    in << endl;
}
void createTree(ofstream& in, const unsigned int n, const long long mnv, const long long mxv)
{
    uniform_int_distribution<unsigned int> dist(0, 1);
    uniform_int_distribution<long long> disav(mnv, mxv), dismv(max(mnv, 1ll), mxv);
    for (unsigned int i = 2; i <= n; ++i)
    {
        const unsigned int op = dist(default_rnd);
        in << GetRandom(1u, i - 1) << " " << op << " ";
        in << (op == 1 ? dismv(default_rnd) : disav(default_rnd)) << endl;
    }
}
void createKnight(ofstream& in, const unsigned int m, const unsigned int n, const long long mns, const long long mxs)
{
    uniform_int_distribution<unsigned int> disp(1, n);
    uniform_int_distribution<long long> diss(mns, mxs);
    for (unsigned int i = 0; i < m; ++i)
        in << diss(default_rnd) << " " << disp(default_rnd) << endl;
}
int main(int, char* argv[])
{
    Init();
    const unsigned int n = GetRandom(1, MayBe(atoi(argv[1]), maxn));
    const unsigned int m = GetRandom(1, MayBe(atoi(argv[2]), maxm));
    ofstream in(argv[9]);
    in << n << " " << m << endl;
    createH(in, n, maybe(argv[3], -maxv), maybe(argv[4], maxv));
    createTree(in, n, maybe(argv[5], -maxv), maybe(argv[6], maxv));
    createKnight(in, m, n, maybe(argv[7], -maxv), maybe(argv[8], maxv));
    return 0;
}
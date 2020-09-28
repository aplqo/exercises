#ifdef APTEST
#include "debug_tools/program.h"
#endif
#include <algorithm>
#include <iostream>
#include <iterator>
#include <numeric>
#include <utility>
#include <vector>
using namespace std;
const unsigned int maxn = 1e5;
constexpr unsigned long long mod = 1e9 + 7;

struct Poly
{
    unsigned long long c, a;
};
vector<Poly> pol;

unsigned long long quickPow(unsigned long long a, unsigned long long e)
{
    unsigned long long ret = 1;
    for (; e; e >>= 1)
    {
        if (e & 0x01)
            ret = (ret * a) % mod;
        a = (a * a) % mod;
    }
    return ret;
}
static unsigned long long getExp()
{
    const unsigned long long e = accumulate(pol.begin(), pol.end(), 0ull, [](unsigned long long v, const Poly& p) {
        return p.a + v;
    });
    for (Poly& i : pol)
        i.a = e - i.a;
    return e;
}
pair<unsigned long long, unsigned long long> Log(unsigned long long a, const unsigned long long lim, const unsigned long long x)
{
    unsigned long long ret = 0;
    while (a % x == 0 && ret < lim)
    {
        ++ret;
        a /= x;
    }
    return make_pair(ret, a);
}
unsigned long long solve(const unsigned long long x)
{
    sort(pol.begin(), pol.end(), [](const Poly& a, const Poly& b) { return a.a > b.a; });
    while (pol.size() > 1)
    {
        const Poly v = pol.back();
        pol.pop_back();
        if (pol.back().a == v.a)
            pol.back().c += v.c;
        else
        {
            if (v.c < x)
                return v.a;
            const auto [ex, re] = Log(v.c, pol.back().a - v.a, x);
            if (v.a + ex < pol.back().a)
                return v.a + ex;
            else
                pol.back().c += re;
        }
    }
    return pol.back().a + Log(pol.back().c, maxn, x).first;
}
int main()
{
#ifndef APTEST
    ios_base::sync_with_stdio(false);
#endif
    unsigned int n;
    unsigned long long x;
    cin >> n >> x;
    pol.reserve(n);
    for (unsigned int i = 0; i < n; ++i)
    {
        unsigned long long v;
        cin >> v;
        pol.push_back(Poly { 1, v });
    }
    const unsigned long long sum = getExp();
    cout << quickPow(x, min(sum, solve(x))) << "\n";
    return 0;
}
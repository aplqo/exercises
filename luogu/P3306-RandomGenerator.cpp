#ifdef APTEST
#include "debug_tools/judge.h"
#endif
#include <climits>
#include <cmath>
#include <iostream>
#include <unordered_map>
constexpr unsigned long long inf = ULLONG_MAX;

struct Pol
{
    unsigned long long a, b;
};

unsigned long long quickPow(unsigned long long a, unsigned long long e, const unsigned long long mod)
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
Pol go(const Pol ini, const unsigned long long step, const unsigned long long mod)
{
    Pol ret = ini;
    for (unsigned int i = 1; i < step; ++i)
    {
        ret.a = (ret.a * ini.a) % mod;
        ret.b = (ret.b * ini.a + ini.b) % mod;
    }
    return ret;
}
unsigned long long bsgs(const Pol ini, const unsigned int p, const unsigned int x1, const unsigned int mod)
{
    std::unordered_multimap<unsigned long long, unsigned int> mem;
    const unsigned int blk = ceil(sqrt(mod));
    {
        const unsigned long long inva = quickPow(ini.a, mod - 2, mod);
        mem.insert({ p, 0 });
        unsigned long long acc = p;
        for (unsigned int i = 1; i <= blk; ++i)
        {
            acc = (inva * (acc + mod - ini.b)) % mod;
            mem.insert({ acc, i });
        }
    }
    {
        const Pol nxt = go(ini, blk, mod);
        unsigned long long acc = x1;
        for (unsigned int i = 0; i <= blk; ++i)
        {
            if (auto it = mem.equal_range(acc); it.second != it.first)
            {
                unsigned long long ret = inf;
                for (auto i = it.first; i != it.second; ++i)
                    ret = std::min<unsigned long long>(ret, i->second);
                return ret + i * blk + 1;
            }
            acc = (acc * nxt.a + nxt.b) % mod;
        }
    }
    return inf;
}

int main()
{
    std::ios::sync_with_stdio(false);
    unsigned int t;
    std::cin >> t;
    for (unsigned int i = t; i; --i)
    {
        unsigned int p, a, b, x1, t;
        std::cin >> p >> a >> b >> x1 >> t;
        if (!a)
        {
            if (x1 == t)
                std::cout << "1\n";
            else if (b == t)
                std::cout << "2\n";
            else
                std::cout << "-1\n";
        }
        else
        {
            const unsigned long long v = bsgs(Pol { a, b }, t, x1, p);
            if (v == inf)
                std::cout << "-1\n";
            else
                std::cout << v << "\n";
        }
    }
    return 0;
}
#ifdef APTEST
#include "debug_tools/program.h"
#endif
#include <climits>
#include <cmath>
#include <iostream>
#include <numeric>
#include <unordered_map>
using namespace std;

unsigned long long mod;
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
unsigned long long bsgs(unsigned long long b, const unsigned long long n)
{
    const unsigned long long step = ceil(sqrt(mod));
    unordered_map<unsigned long long, unsigned long long> mem;
    {
        unsigned long long acc = n;
        mem.reserve(step);
        for (unsigned long long i = 0; i < step; ++i)
        {
            mem[acc] = i;
            acc = (acc * b) % mod;
        }
    }
    {
        unsigned long long acc = 1;
        const unsigned long long s = quickPow(b, step);
        for (unsigned long long i = 0; i < 2 * step; ++i)
        {
            if (auto it = mem.find(acc); it != mem.end() && i * step >= it->second)
                return i * step - it->second;
            acc = (acc * s) % mod;
        }
    }
    return ULLONG_MAX;
}

int main()
{
#ifndef APTEST
    ios_base::sync_with_stdio(false);
#endif
    unsigned long long b, n;
    cin >> mod >> b >> n;
    if (gcd(b, mod) != 1)
        cout << "no solution" << endl;
    else
    {
        const unsigned long long v = bsgs(b, n);
        if (v == ULLONG_MAX)
            cout << "no solution" << endl;
        else
            cout << v << endl;
    }
    return 0;
}
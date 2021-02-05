#ifdef APTEST
#include "debug_tools/judge.h"
#endif
#include <iostream>
#include <string>
const unsigned long long mod = 1e9 + 7, inv2 = 500000004, sqrt2 = 59713600, invSqrt2 = 29856800, phi = mod - 1;
constexpr unsigned int x1 = 1 + sqrt2, x2 = 1 + mod - sqrt2, a = (invSqrt2 * inv2) % mod;

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
unsigned int read()
{
    std::string str;
    unsigned long long ret = 0;
    std::cin >> str;
    for (char i : str)
        ret = (ret * 10 + (i - '0')) % phi;
    return ret;
}

int main()
{
    std::ios::sync_with_stdio(false);
    const unsigned int n = read();
    std::cout << ((quickPow(x1, n) + mod - quickPow(x2, n)) * a) % mod << "\n";
    return 0;
}
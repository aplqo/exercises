#ifdef APTEST
#include "debug_tools/judge.h"
#endif
#include <algorithm>
#include <iostream>
#include <numeric>
const unsigned int maxn = 1e6;

unsigned int mindiv[maxn + 10];
unsigned int phiFactor[maxn + 10], *endF = phiFactor, primitiveRoot[maxn + 10], *endRoot = primitiveRoot;

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
static void eular(const unsigned int x)
{
    static unsigned int primes[maxn + 10], *pcur = primes;
    for (unsigned long long i = 2; i <= x; ++i)
    {
        if (!mindiv[i])
        {
            mindiv[i] = i;
            *(pcur++) = i;
        }
        for (const unsigned int* j = primes; j < pcur && *j <= mindiv[i] && *j * i <= x; ++j)
            mindiv[*j * i] = *j;
    }
}
unsigned int eularPhi(unsigned int x)
{
    unsigned int ret = x;
    while (x > 1)
    {
        ret = ret / mindiv[x] * (mindiv[x] - 1);
        const unsigned int p = mindiv[x];
        while (mindiv[x] == p)
            x /= p;
    }
    return ret;
}
void factor(unsigned int x)
{
    endF = phiFactor;
    while (x > 1)
    {
        const unsigned int p = mindiv[x];
        *(endF++) = p;
        while (mindiv[x] == p)
            x /= p;
    }
}
bool check(const unsigned int a, const unsigned int phi, const unsigned int mod)
{
    if (std::gcd(a, mod) != 1)
        return false;
    for (const unsigned int* i = phiFactor; i < endF; ++i)
        if (quickPow(a, phi / *i, mod) == 1)
            return false;
    return true;
}
bool existPrimitiveRoot(unsigned int x)
{
    if (x == 2 || x == 4)
        return true;
    if (!(x & 0x01))
        x >>= 1;
    const unsigned int p = mindiv[x];
    if (!(p & 0x01))
        return false;
    while (x > 1)
        if (mindiv[x] != p)
            return false;
        else
            x /= p;
    return true;
}
void findPrimitiveRoot(const unsigned int x)
{
    endRoot = primitiveRoot;
    if (!existPrimitiveRoot(x))
        return;
    const unsigned int phi = eularPhi(x);
    factor(phi);
    for (unsigned int i = 1;; ++i)
        if (check(i, phi, x))
        {
            *(endRoot++) = i;
            break;
        }
    unsigned long long acc = primitiveRoot[0];
    for (unsigned int i = 2; i < phi; ++i)
    {
        acc = (acc * primitiveRoot[0]) % x;
        if (std::gcd(i, phi) == 1)
            *(endRoot++) = acc;
    }
}

int main()
{
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);
    std::cout.tie(nullptr);
    eular(maxn);
    unsigned int t;
    std::cin >> t;
    for (; t; --t)
    {
        unsigned int n, d;
        std::cin >> n >> d;
        findPrimitiveRoot(n);
        std::sort(primitiveRoot, endRoot);
        std::cout << endRoot - primitiveRoot << "\n";
        for (const unsigned int* i = primitiveRoot + d - 1; i < endRoot; i += d)
            std::cout << *i << " ";
        std::cout << "\n";
    }
    return 0;
}
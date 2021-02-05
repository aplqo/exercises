#ifdef APTEST
#include "debug_tools/judge.h"
#endif
#include <iostream>
const unsigned int maxn = 1e6;

unsigned int mindiv[maxn * 2 + 10 + 10] { 1, 1 };

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
static void eular(const unsigned int n)
{
    static unsigned int primes[maxn * 2 + 10], *pcur = primes;
    for (unsigned long long i = 2; i <= n; ++i)
    {
        if (!mindiv[i])
        {
            *(pcur++) = i;
            mindiv[i] = i;
        }
        for (const unsigned int* j = primes; j < pcur && *j * i <= n && *j <= mindiv[i]; ++j)
            mindiv[*j * i] = *j;
    }
}
void factor(const unsigned int n, unsigned int cnt[])
{
    for (unsigned int i = n; i; --i)
        if (mindiv[i] != i)
        {
            cnt[mindiv[i]] += cnt[i];
            cnt[i / mindiv[i]] += cnt[i];
            cnt[i] = 0;
        }
}
unsigned long long product(const unsigned int n, const unsigned long long mod, unsigned int cnt[])
{
    unsigned long long ret = 1;
    for (unsigned int i = 2; i <= n; ++i)
        ret = (ret * quickPow(i, cnt[i], mod)) % mod;
    return ret;
}

unsigned long long catalan(const unsigned int n, const unsigned long long mod)
{
    static unsigned int prod1[maxn * 2 + 10], prod2[maxn * 2 + 10];
    std::fill(prod1 + 1, prod1 + n * 2 + 1, 1);
    std::fill(prod2 + 1, prod2 + 1 + n, 2);
    ++prod2[n + 1];
    factor(n * 2, prod1);
    factor(n + 1, prod2);
    for (unsigned int i = 1; i <= n * 2; ++i)
        prod1[i] -= prod2[i];
    return product(n * 2, mod, prod1);
}

int main()
{
    unsigned int n, p;
    std::cin >> n >> p;
    eular(n * 2);
    std::cout << catalan(n, p) << "\n";
    return 0;
}
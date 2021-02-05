#ifdef APTEST
#include "debug_tools/judge.h"
#endif
#include <algorithm>
#include <iostream>
#include <numeric>
#include <unordered_map>
const unsigned int maxn = 1e9, maxm = 1e9, maxb = 1e7, maxk = 2000;

long long mu[maxb + 10];
namespace DuJiao
{
    long long sumMu[maxb + 10];
    static void init()
    {
        static unsigned int primes[maxb + 10], *pcur = primes;
        static unsigned int mindiv[maxb + 10];
        mu[1] = 1;
        for (unsigned long long i = 2; i <= maxb; ++i)
        {
            if (!mindiv[i])
            {
                mu[i] = -1;
                mindiv[i] = i;
                *(pcur++) = i;
            }
            for (const unsigned int* j = primes; j < pcur && *j * i <= maxb && *j < mindiv[i]; ++j)
            {
                const unsigned int nxt = *j * i;
                mindiv[nxt] = *j;
                mu[nxt] = -mu[i];
            }
            if (mindiv[i] * i <= maxb)
                mindiv[mindiv[i] * i] = mindiv[i];
        }
        for (unsigned int i = 1; i <= maxb; ++i)
            sumMu[i] = sumMu[i - 1] + mu[i];
    }
    long long dujiao(const unsigned int x)
    {
        static std::unordered_map<unsigned int, long long> mem;
        if (x <= maxb)
            return sumMu[x];
        if (auto it = mem.find(x); it != mem.end())
            return it->second;
        long long ret = 1;
        for (unsigned int i = 2; i <= x;)
        {
            const unsigned int val = x / i, nxt = x / val + 1;
            ret -= (nxt - i) * dujiao(val);
            i = nxt;
        }
        return mem[x] = ret;
    }
}
namespace CoPrime // count of [gcd(i,k)==1]
{
    unsigned int coPrime[maxk + 10];
    static void init(const unsigned int n)
    {
        for (unsigned int i = 1; i <= n; ++i)
            coPrime[i] = coPrime[i - 1] + (std::gcd(i, n) == 1);
    }
    inline long long countCoPrime(const unsigned int n, const unsigned int k)
    {
        return coPrime[k] * static_cast<unsigned long long>(n / k) + coPrime[n % k];
    }
}
struct Hash
{
    inline size_t operator()(const std::pair<unsigned int, unsigned int> v) const
    {
        return (static_cast<unsigned long long>(v.first) << 32) | v.second;
    }
};
long long coProduct(const unsigned int n, const unsigned int t) // sum of u(i)[gcd(i,t)==1]
{
    static std::unordered_map<std::pair<unsigned int, unsigned int>, long long, Hash> mem;
    if (!n)
        return 0;
    if (t == 1)
        return DuJiao::dujiao(n);
    const std::pair<unsigned int, unsigned int> param(n, t);
    if (auto it = mem.find(param); it != mem.end())
        return it->second;
    long long ret = 0;
    for (unsigned int i = 1; i * i <= t; ++i)
    {
        if (t % i)
            continue;
        ret += mu[i] * mu[i] * coProduct(n / i, i);
        if (const long long another = t / i; another != i)
            ret += mu[another] * mu[another] * coProduct(n / another, another);
    }
    return mem[param] = ret;
}
static void init(const unsigned int k)
{
    CoPrime::init(k);
    DuJiao::init();
}

unsigned long long solve(const unsigned int n, const unsigned int m, const unsigned int k)
{
    long long ret = 0;
    for (unsigned int i = 1; i <= n && i <= m;)
    {
        const unsigned int valN = n / i, valM = m / i, nxt = std::min(n / valN, m / valM);
        ret += valN
            * (coProduct(nxt, k) - coProduct(i - 1, k))
            * CoPrime::countCoPrime(valM, k);
        i = nxt + 1;
    }
    return ret;
}

int main()
{
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);
    std::cout.tie(nullptr);
    unsigned long long n, m;
    unsigned int k;
    std::cin >> n >> m >> k;
    init(k);
    std::cout << solve(n, m, k) << "\n";
    return 0;
}
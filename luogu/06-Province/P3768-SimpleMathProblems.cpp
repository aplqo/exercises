#ifdef APTEST
#include "debug_tools/program.h"
#endif
#include <algorithm>
#include <iostream>
#include <unordered_map>
using namespace std;
const unsigned int maxb = 1e7;

class Number
{
public:
    Number() = default;
    Number(const unsigned long long v)
        : val(v % mod) {};
#define decl(x)                                                                                       \
    inline friend Number operator x(const Number l, const Number r) { return (l.val + mod) x r.val; } \
    inline void operator x##=(const Number r) { *this = *this x r; }
    decl(+);
    decl(-);
    decl(*);
#undef decl
    inline Number square() const
    {
        return val * val;
    }
    inline friend ostream& operator<<(ostream& os, const Number v)
    {
        return os << v.val;
    }

    static unsigned long long mod;

private:
    unsigned long long val = 0;
};
unsigned long long Number::mod;
Number inv2, inv6;

Number quickPow(Number a, unsigned long long e)
{
    Number ret = 1;
    for (; e; e >>= 1)
    {
        if (e & 0x01)
            ret *= a;
        a *= a;
    }
    return ret;
}

Number sumG[maxb + 10]; // g(i) = i^2 * phi(i)
unordered_map<unsigned long long, Number> mem;
static void eular(const unsigned int n)
{
    static unsigned int mindiv[maxb + 10], primes[maxb + 10], *pcur = primes;
    static unsigned int phi[maxb + 10];
    phi[1] = 1;
    for (unsigned long long i = 2; i <= n; ++i)
    {
        if (!mindiv[i])
        {
            mindiv[i] = i;
            phi[i] = i - 1;
            *(pcur++) = i;
        }
        for (const unsigned int* j = primes; j < pcur && *j * i <= n && *j < mindiv[i]; ++j)
        {
            const unsigned int v = *j * i;
            mindiv[v] = *j;
            phi[v] = phi[i] * (*j - 1);
        }
        if (mindiv[i] * i <= n)
        {
            const unsigned int v = mindiv[i] * i;
            mindiv[v] = mindiv[i];
            phi[v] = phi[i] * mindiv[i];
        }
    }
    for (unsigned int i = 1; i <= n; ++i)
        sumG[i] = sumG[i - 1] + phi[i] * Number(i).square();
}
inline Number squareSum(const Number n)
{
    return inv6 * n * (n + 1) * (2 * n + 1);
}
inline Number cubeSum(const Number n)
{
    return (inv2 * n * (n + 1)).square();
}
Number dujiao(const unsigned long long n)
{
    if (n <= maxb)
        return sumG[n];
    if (const auto it = mem.find(n); it != mem.end())
        return it->second;
    Number ret = cubeSum(n);
    for (unsigned long long i = 2; i <= n;)
    {
        const unsigned long long v = n / i, nxt = n / v;
        ret -= dujiao(v) * (squareSum(nxt) - squareSum(i - 1));
        i = nxt + 1;
    }
    return mem[n] = ret;
}

static void init()
{
    eular(maxb);
    inv2 = quickPow(2, Number::mod - 2);
    inv6 = quickPow(6, Number::mod - 2);
}
Number sum(const unsigned long long n)
{
    static const auto sumNat = [](const Number v) { return inv2 * v * (v + 1); };
    Number ret;
    for (unsigned long long i = 1; i <= n;)
    {
        const unsigned long long v = n / i, nxt = n / v;
        ret += sumNat(v).square() * (dujiao(nxt) - dujiao(i - 1));
        i = nxt + 1;
    }
    return ret;
}

int main()
{
#ifndef APTEST
    ios_base::sync_with_stdio(false);
#endif
    unsigned long long n;
    cin >> Number::mod >> n;
    init();
    cout << sum(n) << "\n";
    return 0;
}
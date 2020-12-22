#ifdef APTEST
#include "debug_tools/program.h"
#endif
#include <algorithm>
#include <iostream>
#include <unordered_map>
using namespace std;
const unsigned int maxbn = 1u << 20;

long long mu[maxbn + 10];
unsigned long long phi[maxbn + 10], block;

static void eular(const unsigned int n)
{
    static unsigned int mindiv[maxbn + 10], primes[maxbn], *pcur = primes;
    mu[1] = phi[1] = 1;
    for (unsigned long long i = 2; i <= n; ++i)
    {
        if (!mindiv[i])
        {
            mindiv[i] = i;
            *(pcur++) = i;
            mu[i] = -1;
            phi[i] = i - 1;
        }
        for (const unsigned int* j = primes; j < pcur && *j < mindiv[i] && *j * i <= n; ++j)
        {
            const unsigned int nv = *j * i;
            mindiv[nv] = *j;
            mu[nv] = -mu[i];
            phi[nv] = phi[i] * (*j - 1);
        }
        const unsigned long long v = i * mindiv[i];
        if (v <= n)
        {
            mindiv[v] = mindiv[i];
            phi[v] = mindiv[i] * phi[i];
        }
    }
    for (unsigned int i = 2; i <= n; ++i)
    {
        mu[i] += mu[i - 1];
        phi[i] += phi[i - 1];
    }
}

static pair<long long, unsigned long long> dujiao(const unsigned int n)
{
    static unordered_map<unsigned int, unsigned long long> memPhi;
    static unordered_map<unsigned int, long long> memMu;
    if (n <= block)
        return make_pair(mu[n], phi[n]);
    if (const auto it = memPhi.find(n); it != memPhi.end())
        return make_pair(memMu.at(n), it->second);
    long long m = 1;
    unsigned long long p = n * static_cast<unsigned long long>(n + 1) >> 1;
    for (unsigned int i = 2; i <= n;)
    {
        const unsigned int cv = n / i, nxt = n / cv + 1;
        const auto [nm, np] = dujiao(cv);
        m -= nm * (nxt - i);
        p -= np * (nxt - i);
        i = nxt;
    }
    memPhi[n] = p;
    memMu[n] = m;
    return make_pair(m, p);
}

int main()
{
#ifndef APTEST
    ios_base::sync_with_stdio(false);
#endif
    unsigned int t;
    cin >> t;
    block = maxbn;
    eular(block);
    for (unsigned int i = t; i; --i)
    {
        unsigned int n;
        cin >> n;
        const auto [m, p] = dujiao(n);
        cout << p << " " << m << "\n";
    }
    return 0;
}
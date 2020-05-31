/*Luogu team T132119: Prime squence*/
#ifdef APTEST
#include "debug_tools/program.h"
#endif
#include <algorithm>
#include <cmath>
#include <iostream>
#include <utility>
using namespace std;
using num_t = int;
const int maxn = 2000, maxq = 6000;

unsigned int a[maxn + 10];
unsigned int mindiv[maxq + 10], primes[maxq + 10], *pcur = primes;

void eular(const unsigned int ma)
{
    for (unsigned int i = 2; i <= ma; ++i)
    {
        if (!mindiv[i])
        {
            mindiv[i] = i;
            *(pcur++) = i;
        }
        for (unsigned int* j = primes; j < pcur && *j <= mindiv[i] && *j * i <= ma; ++j)
            mindiv[*j * i] = *j;
    }
}
bool isPrime(const unsigned int val)
{
    for (unsigned int* i = primes; i < pcur && *i < val; ++i)
        if (val % *i == 0)
            return false;
    return true;
}
pair<unsigned int, unsigned int> findPair(const unsigned int n)
{
    unsigned int f = 0, s = 0;
    for (unsigned int i = 0; i < n; ++i)
        for (unsigned int j = i + 1; j < n; ++j)
            if (isPrime(a[i] + a[j]) && (f + s < a[i] + a[j]))
            {
                f = a[i];
                s = a[j];
            }
    return make_pair(f, s);
}
unsigned int findOne(const unsigned int n)
{
    unsigned int* e = upper_bound(a, a + n, 1);
    for (unsigned int* i = a + n - 1; i >= e; --i)
        if (isPrime(*i + 1))
            return *i;
    return 0;
}

int main()
{
    unsigned int n;
    cin >> n;
    for_each(a, a + n, [](unsigned int& i) -> void { cin >> i; });
    sort(a, a + n);
    eular(sqrt(a[n - 1] + a[n - 2]));
    if (all_of(a, a + n, [](unsigned int i) -> bool { return i > 1; }))
    {
        cout << "2" << endl;
        const auto [fst, snd] = findPair(n);
        cout << fst << " " << snd << endl;
    }
    else
    {
        unsigned int cnt = upper_bound(a, a + n, 1) - a;
        unsigned int v = findOne(n);
        cout << (cnt + (v ? 1 : 0)) << endl;
        for (unsigned int i = 0; i < cnt; ++i)
            cout << "1 ";
        if (v)
            cout << v;
    }
    return 0;
}
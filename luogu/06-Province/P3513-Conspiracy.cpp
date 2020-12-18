#ifdef APTEST
#include "debug_tools/program.h"
#endif
#include <algorithm>
#include <bitset>
#include <iostream>
#include <numeric>
using namespace std;
const unsigned int maxn = 5000;
typedef bitset<maxn + 1> Bitset;

Bitset gra[maxn + 10];
unsigned int degree[maxn + 10];

unsigned int count(const unsigned int n)
{
    static Bitset clique, indpent;
    static unsigned int seq[maxn + 10];
    iota(seq, seq + n, 1);
    sort(seq, seq + n, [](unsigned int a, unsigned int b) { return degree[a] > degree[b]; });
    const unsigned int* ptr = seq;
    for (; ptr < seq + n; ++ptr)
        if ((clique & gra[*ptr]) == clique)
            clique.set(*ptr);
        else
            break;
    for (const unsigned int* i = ptr; i < seq + n; ++i)
        for (const unsigned int* j = i + 1; j < seq + n; ++j)
            if (gra[*i].test(*j))
                return 0;
    const unsigned int num = ptr - seq;
    unsigned int ret = (ptr - seq < n);
    if (num != 1)
        ret += upper_bound<const unsigned int*>(seq, ptr, num - 1, [](unsigned int d, unsigned int i) { return d > degree[i]; })
            - lower_bound<const unsigned int*>(seq, ptr, num - 1, [](unsigned int i, unsigned int d) { return degree[i] > d; });
    for (const unsigned int* i = ptr; i < seq + n && degree[*i] == num - 1; ++i)
        for (const unsigned int* j = seq; j < ptr; ++j)
            if (!gra[*i].test(*j))
            {
                ret += degree[*j] == num - 1;
                break;
            }
    return ret;
}

int main()
{
#ifndef APTEST
    ios_base::sync_with_stdio(false);
#endif
    unsigned int n;
    cin >> n;
    for (unsigned int i = 1; i <= n; ++i)
    {
        cin >> degree[i];
        for (unsigned int j = degree[i]; j; --j)
        {
            unsigned int p;
            cin >> p;
            gra[i].set(p);
        }
    }
    cout << count(n) << "\n";
    return 0;
}
#ifdef APTEST
#include "debug_tools/program.h"
#endif
#include <algorithm>
#include <cstring>
#include <iostream>
using namespace std;
const unsigned int maxn = 2e7;

char a[maxn * 2 + 10], p[maxn * 2 + 10];
unsigned int z[maxn * 2 + 10];

unsigned long long Hash(const unsigned int a[], const unsigned int len)
{
    unsigned long long ret = 0;
    for (unsigned long long i = 0; i < len; ++i)
        ret ^= (i + 1) * (a[i] + 1);
    return ret;
}
void ZAlgorithm(const char str[], unsigned int z[], const unsigned int len)
{
    unsigned int l = 0, r = 0;
    for (unsigned int i = 1; i < len; ++i)
    {
        if (i < r)
            z[i] = z[i - l];
        if (i + z[i] < r)
            continue;
        z[i] = min(z[i], r - i);
        while (i + z[i] < len && str[z[i]] == str[i + z[i]])
            ++z[i];
        if (i + z[i] > r)
        {
            r = i + z[i];
            l = i;
        }
    }
}

int main()
{
#ifndef APTEST
    ios_base::sync_with_stdio(false);
#endif
    cin >> a;
    cin >> p;
    const size_t lp = strlen(p), la = strlen(a);
    copy(a, a + la, p + lp + 1);
    ZAlgorithm(p, z, lp + la + 1);
    z[0] = lp;
    cout << Hash(z, lp) << "\n"
         << Hash(z + lp + 1, la) << "\n";
    return 0;
}
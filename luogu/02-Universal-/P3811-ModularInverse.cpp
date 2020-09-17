#ifdef APTEST
#include "debug_tools/program.h"
#endif
#include <algorithm>
#include <iostream>
#include <iterator>
using namespace std;
const unsigned int maxn = 3e6;

unsigned long long inv[maxn + 10] = { 0, 1 };

static void inverse(const unsigned int n, const unsigned long long p)
{
    for (unsigned int i = 2; i <= n; ++i)
        inv[i] = (p - (inv[p % i] * (p / i)) % p) % p;
}
int main()
{
#ifndef APTEST
    ios_base::sync_with_stdio(false);
#endif
    unsigned int n;
    unsigned long long p;
    cin >> n >> p;
    inverse(n, p);
    copy(inv + 1, inv + 1 + n, ostream_iterator<unsigned long long>(cout, "\n"));
    return 0;
}
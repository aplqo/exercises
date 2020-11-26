#ifdef APTEST
#include "debug_tools/program.h"
#endif
#include <algorithm>
#include <climits>
#include <iostream>
using namespace std;

unsigned long long enumerate(const unsigned long long n, unsigned int a, unsigned int b, unsigned int c, unsigned int d)
{
    if (b * static_cast<unsigned long long>(c) > d * static_cast<unsigned long long>(a))
    {
        swap(b, d);
        swap(a, c);
    }
    unsigned long long ret = ((n + a - 1) / a) * b;
    for (unsigned long long i = 0; i <= a; ++i)
    {
        const unsigned long long cnt = i * c;
        if (cnt < n)
            ret = min(ret, i * d + ((n - cnt + a - 1) / a) * b);
        else
            ret = min(ret, i * d);
    }
    return ret;
}

int main()
{
#ifndef APTEST
    ios_base::sync_with_stdio(false);
#endif
    unsigned int a, b, c, d;
    unsigned long long n;
    cin >> n >> a >> b >> c >> d;
    cout << enumerate(n, a, b, c, d) << "\n";
    return 0;
}
/*Luogu team T133429: Calculating*/
#ifdef APTEST
#include "debug_tools/program.h"
#endif
#include <iostream>
using namespace std;
using num = unsigned long long;

unsigned int find(unsigned int l, unsigned int r, const num x, const num v)
{
    while (l <= r)
    {
        const unsigned int m = (l + r) >> 1;
        if (x * m >= v)
            r = m - 1;
        else
            l = m + 1;
    }
    return l;
}
num fun(const unsigned int x, const unsigned int y)
{
    const unsigned int e = (y - 1) >> 1;
    num ret = 0;
    unsigned int v = x / y;
    for (unsigned int i = 1; i <= e;)
    {
        const unsigned int ni = find(i + 1, e, x, (v + 1) * y);
        ret += num(v) * (ni - i);
        i = ni;
        v = (ni * x) / y;
    }
    return ret;
}
int main()
{
#ifndef APTEST
    ios_base::sync_with_stdio(false);
#endif
    unsigned int p, q;
    cin >> p >> q;
    cout << fun(p, q) + fun(q, p) << endl;
    return 0;
}
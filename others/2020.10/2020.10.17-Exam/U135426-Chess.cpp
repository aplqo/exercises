#ifdef APTEST
#include "debug_tools/program.h"
#endif
#include <algorithm>
#include <iostream>
using namespace std;

constexpr unsigned int repTable[34] = { 1, 1, 2, 0, 3, 1, 1, 0, 3, 3, 2, 2, 4, 4, 5, 5, 9, 3, 3, 0, 1, 1, 3, 0, 2, 1, 1, 0, 4, 5, 3, 7, 4, 8 };
constexpr unsigned int lessTable[69] = {
    0, 1, 1, 2, 0, 3, 1, 1, 0, 3, 3, 2, 2, 4, 0, 5, 2, 2, 3, 3, 0,
    1, 1, 3, 0, 2, 1, 1, 0, 4, 5, 2, 7, 4, 0, 1, 1, 2, 0, 3, 1,
    1, 0, 3, 3, 2, 2, 4, 4, 5, 5, 2, 3, 3, 0, 1, 1, 3, 0, 2, 1,
    1, 0, 4, 5, 3, 7, 4, 8
};

unsigned int sg(const unsigned long long x)
{
    return x > 68 ? repTable[(x - 69) % 34] : lessTable[x];
}
int main()
{
#ifndef APTEST
    ios_base::sync_with_stdio(false);
#endif
    unsigned int t;
    cin >> t;
    for (unsigned int i = 0; i < t; ++i)
    {
        unsigned int n, acc = 0;
        cin >> n;
        for (unsigned int i = 0; i < n; ++i)
        {
            unsigned long long l;
            cin >> l;
            acc ^= sg(l);
        }
        cout << (acc ? "Alice" : "Bob") << "\n";
    }
    return 0;
}
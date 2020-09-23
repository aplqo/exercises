#ifdef APTEST
#include "debug_tools/program.h"
#endif
#include <algorithm>
#include <iostream>
using namespace std;

int main()
{
#ifndef APTEST
    ios_base::sync_with_stdio(false);
#endif
    unsigned int n, k;
    cin >> n >> k;
    {
        unsigned int acc = 0;
        for (unsigned int i = 0; i < n * 2 - 1; ++i)
        {
            ++acc;
            if (acc == 2 * n - k)
                ++acc;
            cout << acc << " ";
        }
        cout << 2 * n - k << "\n";
    }
    return 0;
}
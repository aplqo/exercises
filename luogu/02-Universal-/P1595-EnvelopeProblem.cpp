#ifdef APTEST
#include "debug_tools/program.h"
#endif
#include <iostream>
using namespace std;

int main()
{
#ifndef APTEST
    ios_base::sync_with_stdio(false);
#endif
    unsigned int n;
    cin >> n;
    unsigned long long lst[2] = { 0, 1 };
    if (n < 3)
    {
        cout << lst[n - 1] << endl;
        return 0;
    }
    for (unsigned int i = 3; i <= n; ++i)
    {
        unsigned long long t = (i - 1) * (lst[0] + lst[1]);
        lst[0] = lst[1];
        lst[1] = t;
    }
    cout << lst[1] << endl;
    return 0;
}
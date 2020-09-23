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
    if (n == 1)
        cout << "-1\n";
    else
        cout << n << " " << n + 1 << " " << n * (n + 1) << "\n";
    return 0;
}
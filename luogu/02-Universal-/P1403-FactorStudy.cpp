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
    unsigned long long ret = 0;
    cin >> n;
    for (unsigned int i = 1; i <= n; ++i)
        ret += n / i;
    cout << ret << endl;
    return 0;
}
/*Luogu team T133429: Calculating*/
#ifdef APTEST
#include "debug_tools/program.h"
#endif
#include <iostream>
using namespace std;
using num = unsigned long long;

int main()
{
#ifndef APTEST
    ios_base::sync_with_stdio(false);
#endif
    num p, q;
    cin >> p >> q;
    if (p != q)
        cout << ((p - 1) * (q - 1) >> 2) << endl;
    else
        cout << ((p - 1) * (q + 1) >> 2) << endl;
    return 0;
}
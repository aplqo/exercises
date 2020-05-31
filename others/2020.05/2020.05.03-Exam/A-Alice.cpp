/*Luogu team T131976: Alice*/
#ifdef APTEST
#include "debug_tools/program.h"
#endif
#include <algorithm>
#include <iostream>
using namespace std;
const int maxn = 100000;

unsigned int a[maxn + 10];

int main()
{
#ifndef APTEST
    ios_base::sync_with_stdio(false);
#endif
    unsigned int n;
    cin >> n;
    for_each(a, a + n, [](unsigned int& i) -> void { cin >> i; });
    sort(a, a + n);
    cout << (unique(a, a + n) - a) << endl;
    return 0;
}
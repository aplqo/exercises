#ifdef APTEST
#include "debug_tools/program.h"
#endif
#include <iostream>
#include <string>
using namespace std;
constexpr unsigned int mod = 10007, inv6 = 1668;

int main()
{
#ifndef APTEST
    ios_base::sync_with_stdio(false);
#endif
    string str;
    cin >> str;
    unsigned long long n = 0;
    for (auto i = str.cbegin(); i != str.cend(); ++i)
        n = (n * 10 + (*i - '0')) % mod;
    n += 2;
    cout << (n * (n - 1) * (n - 2) * inv6) % mod << "\n";
    return 0;
}
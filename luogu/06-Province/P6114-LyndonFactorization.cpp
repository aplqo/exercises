#ifdef APTEST
#include "debug_tools/program.h"
#endif
#include <algorithm>
#include <cstring>
#include <functional>
#include <iostream>
#include <numeric>
#include <vector>
using namespace std;
const unsigned int maxn = 5e6;

vector<unsigned int> duval(const char str[], const size_t len)
{
    vector<unsigned int> ret;
    for (unsigned int i = 0; i < len;)
    {
        unsigned int ps2 = i, ps3 = i + 1;
        while (ps3 < len && str[ps2] <= str[ps3])
        {
            if (str[ps2] == str[ps3])
                ++ps2;
            else
                ps2 = i;
            ++ps3;
        }
        const unsigned int stp = ps3 - ps2;
        while (i <= ps2)
        {
            ret.push_back(i + stp);
            i += stp;
        }
    }
    return ret;
}

int main()
{
#ifndef APTEST
    ios_base::sync_with_stdio(false);
#endif
    static char buf[maxn + 10];
    cin >> buf;
    const auto part = duval(buf, strlen(buf));
    cout << accumulate(part.cbegin(), part.cend(), 0u, bit_xor<unsigned int>()) << "\n";
    return 0;
}
#ifdef APTEST
#include "debug_tools/program.h"
#endif
#include <algorithm>
#include <cstring>
#include <iostream>
#include <sstream>
using namespace std;
const unsigned int maxl = 6;

char str[maxl + 10];
unsigned int dig[maxl + 10];

void toDigit(const char* str, unsigned int res[], const unsigned int len)
{
    for (unsigned int i = 0; i < len; ++i)
        res[i] = str[i] - '0';
}
unsigned int generate(ostream& os, unsigned int x[], const unsigned int len)
{
    unsigned int cnt = count_if(x, x + len, [](unsigned int i) { return i > 0; }), ret = 0;
    while (cnt)
    {
        bool flag = false;
        for (unsigned int i = 0; i < len; ++i)
            if (x[i])
            {
                --x[i];
                if (!x[i])
                    --cnt;
                os.put('1');
                flag = true;
            }
            else if (flag)
                os.put('0');
        os.put(' ');
        ++ret;
    }
    return ret;
}
int main()
{
#ifndef APTEST
    ios_base::sync_with_stdio(false);
#endif
    cin >> str;
    const unsigned int l = strlen(str);
    toDigit(str, dig, l);
    ostringstream os;
    cout << generate(os, dig, l) << "\n";
    cout << os.str() << "\n";
    return 0;
}
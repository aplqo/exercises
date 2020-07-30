#ifdef APTEST
#include "debug_tools/program.h"
#endif
#include <algorithm>
#include <iostream>
using namespace std;
const unsigned int maxn = 500000;

char str[maxn * 2 + 10];

unsigned int manacher(const int n)
{
    static int rad[maxn * 2 + 10] = { -1 };
    int r = 0, c = 0, ret = 0;
    for (int i = 0; i < n; ++i)
    {
        int j = 0;
        if (i < r && rad[2 * c - i] + i - 1 < r)
        {
            rad[i] = rad[2 * c - i];
            continue;
        }
        else if (i < r)
        {
            j = r - i;
            rad[i] = r - i;
        }
        for (; j + i < n; ++j)
            if (str[i + j] == str[i - j])
            {
                r = i + j;
                ++rad[i];
                if (!str[i] && rad[i] % 4 == 0 && rad[i - rad[i] / 2] >= rad[i] / 2)
                    ret = max(ret, rad[i]);
            }
            else
                break;
        c = i;
    }
    return ret;
}
int main()
{
#ifndef APTEST
    ios_base::sync_with_stdio(false);
#endif
    unsigned int len, p = 1;
    cin >> len;
    for (unsigned int i = 0; i < len; ++i, p += 2)
        cin >> str[p];
    cout << manacher(p) << endl;
    return 0;
}
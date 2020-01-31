#ifdef APDEBUG
#include "debug_tools/program.h"
#endif
#include <algorithm>
#include <iostream>
#include <iterator>
using namespace std;
const int maxn = 11000000;

char str[(maxn + 10) * 2];
int rad[(maxn + 10) * 2];

void manacher(const char str[], int len)
{
    int r = 0, c = 0;
    rad[0] = -1;
    for (int i = 0; i < len; ++i)
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
        for (; i + j < len; ++j)
            if (str[i + j] == str[i - j])
            {
                r = i + j;
                ++rad[i];
            }
            else
                break;
        c = i;
    }
}
void read(char str[], int& len)
{
    len = 1;
    while (cin)
    {
        cin >> str[len];
        if (cin.fail())
            break;
        len += 2;
    }
}

int main()
{
    ios_base::sync_with_stdio(false);
    int l = 0;
    read(str, l);
    manacher(str, l);
    cout << *max_element(rad, rad + l) - 1 << endl;
    return 0;
}
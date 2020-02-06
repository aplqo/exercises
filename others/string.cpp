#ifdef APTEST
#include "debug_tools/program.h"
#endif
#include <iostream>
using namespace std;

int main()
{
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);
    unsigned int n, k;
    cin >> n >> k;
    if ((k == 1 && n > 1) || k > n)
    {
        cout << "-1";
        return 0;
    }
    else if (n == 1)
    {
        cout << "a";
        return 0;
    }

    unsigned int lst = 0;
    for (unsigned int i = 0; i < n + 2 - k; ++i)
    {
        cout.put(char(lst + 'a'));
        lst ^= 0x01;
    }
    for (unsigned int i = 2; i < k; ++i)
        cout.put(char(i + 'a'));
    cout.flush();
    return 0;
}